#pragma once

#include "ryml.hpp"

namespace node_utils
{

void set_reference(ryml::NodeRef& node, ryml::csubstr anchor);

std::size_t
get_next_valid_id_after_content_removal(ryml::NodeRef node,
                                        std::size_t max_possible_id);

std::string to_string(const ryml::ConstNodeRef& node);

class NodeDiff
{
    ryml::ConstNodeRef lhs_;
    ryml::ConstNodeRef rhs_;

public:
    NodeDiff(const ryml::ConstNodeRef& lhs, const ryml::ConstNodeRef& rhs);

    std::string str() const;

    friend std::ostream& operator<<(std::ostream& os, NodeDiff const& diff);
};

// Hash function for c4::yml::ConstNodeRef
struct RapidYamlNodeHash
{
    std::size_t operator()(const ryml::ConstNodeRef& node) const;
};

} // namespace node_utils
