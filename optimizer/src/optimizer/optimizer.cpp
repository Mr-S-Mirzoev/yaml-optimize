#include "optimizer/optimizer.h"

#include "debug/assert.h"
#include "debug/print.h"
#include "debug/throw.h"

#include "utils/io.h"
#include "utils/node.h"
#include "utils/string.h"

#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

#include "fmt/format.h"
#include "ryml.hpp"

YamlOptimizer::YamlOptimizer(std::string const& content,
                             OptimizationSettings const& settings)
    : content_(content), settings_(settings)
{
    // Load YAML file using rapidyaml
    tree_ = ryml::parse_in_place(get_clean_content(content_));

    // Initialize data vector with the size of the YAML tree
    data_.resize(tree_.size());

    // Traverse the YAML tree and populate the node information
    get_info();

#ifdef YO_DEBUG
    debug_print_data();
#endif // YO_DEBUG
}

YamlOptimizer::YamlOptimizer(std::istream& is,
                             OptimizationSettings const& settings)
    : YamlOptimizer(io_utils::get_file_content(is), settings)
{
}

void YamlOptimizer::optimize()
{
    for (std::size_t i = 0; i < data_.size() - 1; ++i)
    {
        YO_DEBUG_PRINT("i={}", i);
        if (i >= data_.size())
            break;

        if (settings_.optimization_limit.has_value() &&
            data_[i].size <= settings_.optimization_limit.value())
            continue;

        for (std::size_t j = i + 1; j < data_.size(); ++j)
        {
            YO_DEBUG_PRINT("i={}; j={}", i, j);
            YO_DEBUG_ASSERT_WITH_MSG(
                data_.size() == tree_.size(),
                "Node count must be solvent at all times: {} != {}",
                data_.size(), tree_.size());

            if (i >= data_.size() || j >= data_.size())
                break;

            auto a = tree_.ref(i);
            auto b = tree_.ref(j);

            if (a.is_ref() || b.is_ref())
                continue;

            if (!a.is_map() && !a.is_seq())
                continue;

            if (settings_.optimization_limit.has_value() &&
                data_[j].size <= settings_.optimization_limit.value())
                continue;

            if (!nodes_equal(a, b))
            {
                YO_DEBUG_PRINT(
                    "Diff nodes: \nlhs:\n\"\"\"\n{}\"\"\"\n  "
                    "\nrhs:\n\"\"\"\n{}\"\"\"\n  \ndiff:\n\"\"\"\n{}\"\"\"\n",
                    node_utils::to_string(a), node_utils::to_string(b),
                    node_utils::NodeDiff(a, b).str());
                continue;
            }

            ryml::csubstr anchor;
            if (a.has_val_anchor())
            {
                anchor = {a.val_anchor().data(), a.val_anchor().size()};
            }
            else
            {
                auto anchor_str = fmt::format("anchor_{}", anchor_count_++);
                anchor =
                    tree_.copy_to_arena({anchor_str.data(), anchor_str.size()});
                a.set_val_anchor(anchor);
            }

            // clang-format off
            auto next_valid_id = node_utils::get_next_valid_id_after_content_removal(b, data_.size());
            // clang-format on

            node_utils::set_reference(b, anchor);

            // Reorder the tree nodes (fix indexes to match [0 .. sz - 1] range)
            tree_.reorder();

            YO_DEBUG_PRINT("Removing elements from {} to {}. Removing: \n{}\n",
                           b.id() + 1, next_valid_id, node_utils::to_string(a));

            // Erase the redundant nodes from the data vector
            data_.erase(data_.begin() + b.id() + 1,
                        data_.begin() + next_valid_id);
        }
    }

#ifdef YO_DEBUG
    debug_print_data();
#endif // YO_DEBUG
}

std::string YamlOptimizer::str() const
{
    std::ostringstream os;
    write_to_ostream(os);
    return os.str();
}

void YamlOptimizer::dump(std::string const& filename)
{
    std::ofstream of(filename);

    if (!of.good())
        YO_THROW(YamlOptimizerError,
                 "Failed to create output file with name {}", filename);

    write_to_ostream(of);
}

/* clang-format off */
void YamlOptimizer::get_info() 
{
    get_info_impl(tree_.rootref());
}
/* clang-format on */

bool YamlOptimizer::long_types_equal(const ryml::ConstNodeRef& a,
                                     const ryml::ConstNodeRef& b) const
{
    if (a.is_map())
        return b.is_map();

    if (a.is_seq())
        return b.is_seq();

    if (a.is_keyval())
        return b.is_keyval();

    if (a.is_val())
        return b.is_val();

    return false;
}

bool YamlOptimizer::nodes_equal(const ryml::ConstNodeRef& a,
                                const ryml::ConstNodeRef& b) const
{
    if (data_[a.id()].size != data_[b.id()].size)
    {
        YO_DEBUG_PRINT("Size mismatch: {} vs {}", data_[a.id()].size,
                       data_[b.id()].size);
        return false;
    }

    if (data_[a.id()].hash != data_[b.id()].hash)
    {
        YO_DEBUG_PRINT("Hash mismatch: {} vs {}", data_[a.id()].hash,
                       data_[b.id()].hash);
        return false;
    }

    if (a.type() != b.type())
    {
        YO_DEBUG_PRINT("Type mismatch: {} vs {}", a.type_str(), b.type_str());

        if (!long_types_equal(a, b))
            return false;
    }

    if (!a.is_container())
    {
        if (a.has_key() && a.key() != b.key())
        {
            YO_DEBUG_PRINT("Key mismatch");
            return false;
        }

        if (a.val() != b.val())
        {
            YO_DEBUG_PRINT("Val mismatch");
            return false;
        }

        if (a.has_val_tag() != b.has_val_tag())
        {
            YO_DEBUG_PRINT("Val tag mismatch 1");
            return false;
        }

        if (a.has_val_tag() && a.val_tag() != b.val_tag())
        {
            YO_DEBUG_PRINT("Val tag mismatch 2");
            return false;
        }
    }
    else
    {
        if (a.has_children() != b.has_children())
        {
            YO_DEBUG_PRINT("No child mismatch");
            return false;
        }

        if (a.num_children() != b.num_children())
        {
            YO_DEBUG_PRINT("Num children mismatch: {} vs. {}", a.num_children(),
                           b.num_children());
            return false;
        }

        for (auto a_it = a.begin(), b_it = b.begin();
             a_it != a.end() && b_it != b.end(); ++a_it, ++b_it)
        {
            if (!nodes_equal(*a_it, *b_it))
            {
                YO_DEBUG_PRINT("Child mismatch");
                return false;
            }
        }
    }

    return true;
}

void YamlOptimizer::write_to_ostream(std::ostream& os) const
{
    if (is_utf8)
        os << BOM;

    os << tree_;
}

ryml::substr YamlOptimizer::get_clean_content(std::string& content)
{
    std::size_t offset = 0;
    is_utf8 = string_utils::starts_with(content, BOM);
    if (is_utf8)
        offset = BOM.length();

    return {content.data() + offset, content.length() - offset};
}

std::size_t YamlOptimizer::get_info_impl(const ryml::ConstNodeRef& node)
{
    // Store the size of the current node in the data vector
    std::size_t nodeId{node.id()};
    YO_DEBUG_ASSERT_WITH_MSG(nodeId < data_.size(),
                             "Node id must never exceed nodes count");

    auto& node_size = data_[nodeId].size;

    if (node_size != 0)
        return node_size;

    data_[nodeId].hash = node_utils::RapidYamlNodeHash{}(node);

    if (!node.is_container())
        return node_size = 1;

    if (!node.has_children())
        return node_size;

    // Traverse child nodes recursively
    for (auto child : node.children())
        node_size += get_info_impl(child);

    return node_size;
}

#ifdef YO_DEBUG
void YamlOptimizer::debug_print_data() const
{
    for (int i = 0; i < data_.size(); ++i)
    {
        std::string key;
        if (tree_.ref(i).has_key())
            key = {tree_.ref(i).key().data(), tree_.ref(i).key().size()};
        std::string val;
        if (tree_.ref(i).has_val())
            val = {tree_.ref(i).val().data(), tree_.ref(i).val().size()};
        YO_DEBUG_PRINT("{}({} -> {}) : {}", i, key, val, data_[i].size);
    }
}
#endif // YO_DEBUG
