#include "utils/node.h"
#include "utils/string.h"

#include <functional>
#include <sstream>
#include <string>

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

std::size_t get_next_valid_id_after_content_removal(ryml::NodeRef node,
                                                    std::size_t max_possible_id)
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

NodeDiff::NodeDiff(const ryml::ConstNodeRef& lhs, const ryml::ConstNodeRef& rhs)
    : lhs_(lhs), rhs_(rhs)
{
}

std::string NodeDiff::str() const
{
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, NodeDiff const& diff)
{
    std::string str_lhs = to_string(diff.lhs_);
    std::string str_rhs = to_string(diff.rhs_);

    os << string_utils::StringDiff(str_lhs, str_rhs);

    return os;
}

std::size_t
RapidYamlNodeHash::operator()(const c4::yml::ConstNodeRef& node) const
{
    std::size_t seed = 0;
    std::size_t type = 0;

    if (node.is_map())
    {
        for (auto it = node.begin(); it != node.end(); ++it)
        {
            auto child = it.m_tree->ref(it.m_child_id);
            seed ^= RapidYamlNodeHash{}(child);
        }

        type = 1 << 0;
    }
    else if (node.is_seq())
    {
        for (auto it = node.begin(); it != node.end(); ++it)
            seed ^= RapidYamlNodeHash{}(*it);

        type = 1 << 1;
    }
    else if (node.is_keyval())
    {
        seed ^= std::hash<std::size_t>{}(node.key().size());
        seed ^= std::hash<std::size_t>{}(node.val().size());
        type = 1 << 2;
    }
    else if (node.is_val())
    {
        seed ^= std::hash<std::size_t>{}(node.val().size());
        type = 1 << 3;
    }

    // Use the node type and value to compute the hash
    seed ^= std::hash<std::size_t>{}(type);

    return seed;
}

} // namespace node_utils
