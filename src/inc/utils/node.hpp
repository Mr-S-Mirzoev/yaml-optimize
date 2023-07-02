#pragma once

#include <sstream>
#include <string>

#include "ryml.hpp"

namespace node_utils
{
void set_reference(ryml::NodeRef& node, ryml::csubstr anchor)
{
    ryml::csubstr key;
    if (node.has_key())
        key = node.key();

    node.clear_children();
    node.set_type(ryml::VALREF);

    // Otherwise `key` is overwritten for some reason
    // Most likely due to set_type.
    node.set_key(key);
    node.set_val_ref(anchor);
}

std::size_t get_next_valid_id_after_content_removal(ryml::NodeRef node, std::size_t max_possible_id)
{
    std::size_t next_valid_id = ryml::NONE;

    while (next_valid_id == ryml::NONE && node.has_parent())
    {
        if (node == node.last_sibling())
        {
            node = node.parent();
        }
        else
        {
            next_valid_id = node.next_sibling().id();
            break;
        }
    }

    if (next_valid_id == ryml::NONE)
        next_valid_id = max_possible_id;

    return next_valid_id;
}

std::string to_string(const ryml::ConstNodeRef& node)
{
    std::ostringstream oss;
    oss << node;
    return oss.str();
}

class NodeDiff
{
    ryml::ConstNodeRef lhs_;
    ryml::ConstNodeRef rhs_;

public:
    NodeDiff(const ryml::ConstNodeRef& lhs, const ryml::ConstNodeRef& rhs)
        : lhs_(lhs), rhs_(rhs)
    {
    }

    std::string str() const
    {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, NodeDiff const& diff)
    {
        std::string str_lhs = to_string(diff.lhs_);
        std::string str_rhs = to_string(diff.rhs_);

        std::istringstream iss_lhs(str_lhs);
        std::istringstream iss_rhs(str_rhs);

        std::string line_lhs, line_rhs;
        int line_no = 1;

        while (std::getline(iss_lhs, line_lhs) &&
               std::getline(iss_rhs, line_rhs))
        {
            if (line_lhs != line_rhs)
            {
                os << line_no << "c" << std::endl;
                os << "< " << line_lhs << std::endl;
                os << "---" << std::endl;
                os << "> " << line_rhs << std::endl;
            }

            ++line_no;
        }

        // Print remaining lines if the strings have different line counts
        while (std::getline(iss_lhs, line_lhs))
        {
            os << line_no << "d" << std::endl;
            os << "< " << line_lhs << std::endl;
            ++line_no;
        }

        while (std::getline(iss_rhs, line_rhs))
        {
            os << line_no << "a" << std::endl;
            os << "> " << line_rhs << std::endl;
            ++line_no;
        }

        return os;
    }
};
} // namespace node_utils
