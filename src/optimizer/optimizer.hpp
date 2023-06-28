#pragma once

#include "../debug/assert.hpp"
#include "../debug/print.hpp"

#include "../utils/string.hpp"

#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

#include "fmt/format.h"
#include "ryml.hpp"

class OptimizationError : public std::runtime_error
{
public:
    OptimizationError(std::string const& what) : std::runtime_error(what) {}
};

class YamlOptimizer
{
    // NodeInfo struct to store node information
    struct NodeInfo
    {
        std::size_t size;
    };

public:
    // Constructor
    explicit YamlOptimizer(const std::string& content) : content_(content)
    {
        // Load YAML file using rapidyaml
        tree_ = ryml::parse_in_place(get_clean_content(content_));

        // Initialize data vector with the size of the YAML tree
        data_.resize(tree_.size());

        // Traverse the YAML tree and populate the node information
        get_info();

#ifdef YO_DEBUG
        for (int i = 0; i < data_.size(); ++i)
        {
            std::string key;
            if (tree_.ref(i).has_key())
                key = {tree_.ref(i).key().data(), tree_.ref(i).key().size()};
            std::string val;
            if (tree_.ref(i).has_val())
                val = {tree_.ref(i).val().data(), tree_.ref(i).val().size()};
            DEBUG_PRINT("{}({} -> {}) : {}", i, key, val, data_[i].size);
        }
#endif // YO_DEBUG
    }

    explicit YamlOptimizer(std::istream& is)
        : YamlOptimizer(get_file_content(is))
    {
    }

    // Function to optimize the YAML configuration
    void optimize()
    {
        for (std::size_t i = 0; i < data_.size() - 1; ++i)
        {
            for (std::size_t j = i + 1; j < data_.size(); ++j)
            {
                DEBUG_PRINT("i={}; j={}", i, j);
                if (i >= data_.size() || j >= data_.size())
                    break;

                auto a = tree_.ref(i);
                auto b = tree_.ref(j);

                if (a.is_ref() || b.is_ref())
                    continue;

                if (!a.is_map())
                    continue;
                    
                if (!nodes_equal(a, b))
                {
                    std::cout << a << std::endl << std::endl;
                    std::cout << b << std::endl << std::endl;
                    std::cout << std::endl << std::endl;
                    continue;
                }

                ryml::csubstr anchor;
                if (a.has_val_anchor())
                {
                    anchor = {a.val_anchor().data(), a.val_anchor().size()};
                }
                else
                {
                    auto anchor_str = fmt::format("anchor_{}", anchor_id_++);
                    anchor = tree_.copy_to_arena({anchor_str.data(), anchor_str.size()});
                    a.set_val_anchor(anchor);
                }

                auto next_valid_id = b.next_sibling().id();
                ryml::csubstr key;
                if (b.has_key())
                    key = b.key();

                b.clear_children();
                b.set_type(ryml::VALREF);

                // Otherwise `key` is overwritten for some reason
                b.set_key(key);
                b.set_val_ref(anchor);

                DEBUG_PRINT("Count: {}", tree_.size());

                tree_.reorder();

                if (next_valid_id == ryml::NONE)
                    next_valid_id = data_.size();

                DEBUG_PRINT("next_valid_id={}; b.id = {}", next_valid_id, b.id());
                data_.erase(data_.begin() + b.id() + 1, data_.begin() + next_valid_id);
                DEBUG_PRINT("data_.size={}", data_.size());
            }
        }

#ifdef YO_DEBUG
        for (int i = 0; i < data_.size(); ++i)
        {
            std::string key;
            if (tree_.ref(i).has_key())
                key = {tree_.ref(i).key().data(), tree_.ref(i).key().size()};
            std::string val;
            if (tree_.ref(i).has_val())
                val = {tree_.ref(i).val().data(), tree_.ref(i).val().size()};
            DEBUG_PRINT("{}({} -> {}) : {}", i, key, val, data_[i].size);
        }
#endif // YO_DEBUG
    }

    // Function to retrieve the currently stored YAML string
    std::string str() const
    {
        std::ostringstream os;
        write_to_ostream(os);
        return os.str();
    }

    void dump(std::string const& filename)
    {
        std::ofstream of(filename);

        if (!of.good())
            throw OptimizationError(fmt::format(
                "Failed to create output file with name {}", filename));

        write_to_ostream(of);
    }

private:
    std::vector<NodeInfo> data_;
    ryml::Tree tree_;
    std::string content_;
    std::size_t anchor_id_ = 0;

    static constexpr std::string_view BOM{"\xEF\xBB\xBF"};
    bool is_utf8 = false;

    void get_info() { get_info_impl(tree_.rootref()); }

    bool long_types_equal(const ryml::ConstNodeRef& a, const ryml::ConstNodeRef& b) const
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

    bool nodes_equal(const ryml::ConstNodeRef& a,
                     const ryml::ConstNodeRef& b) const
    {
        if (data_[a.id()].size != data_[b.id()].size)
        {
            DEBUG_PRINT("Size mismatch: {} vs {}", data_[a.id()].size, data_[b.id()].size);
            return false;
        }

        if (a.type() != b.type())
        {
            DEBUG_PRINT("Type mismatch: {} vs {}", a.type_str(), b.type_str());

            if (!long_types_equal(a, b))
                return false;
        }

        if (!a.is_container())
        {
            if (a.has_key())
            {
                if (a.key() != b.key())
                {
                    DEBUG_PRINT("Key mismatch");
                    return false;
                }
            }

            if (a.val() != b.val())
            {
                DEBUG_PRINT("Val mismatch");
                return false;
            }

            if (a.has_val_tag() != b.has_val_tag())
            {
                DEBUG_PRINT("Val tag mismatch 1");
                return false;
            }

            if (a.has_val_tag() && a.val_tag() != b.val_tag())
            {
                DEBUG_PRINT("Val tag mismatch 2");
                return false;
            }
        }
        else
        {
            if (a.has_children() != b.has_children())
            {
                DEBUG_PRINT("No child mismatch");
                return false;
            }

            if (a.num_children() != b.num_children())
            {
                DEBUG_PRINT("Num children mismatch");
                return false;
            }

            for (auto a_it = a.begin(), b_it = b.begin();
                 a_it != a.end() && b_it != b.end();
                 ++a_it, ++b_it)
            {
                if (!nodes_equal(*a_it, *b_it))
                {
                    DEBUG_PRINT("Child mismatch");
                    return false;
                }
            }
        }

        return true;
    }

    void write_to_ostream(std::ostream& os) const
    {
        if (is_utf8)
            os << BOM;

        os << tree_;
    }

    ryml::substr get_clean_content(std::string& content)
    {
        std::size_t offset = 0;
        is_utf8 = string_utils::starts_with(content, BOM);
        if (is_utf8)
            offset = BOM.length();

        return {content.data() + offset, content.length() - offset};
    }

    static std::string get_file_content(std::istream& is)
    {
        std::string out;

        is.seekg(0, std::ios::end);
        out.reserve(static_cast<std::string::size_type>(is.tellg()));
        is.seekg(0, std::ios::beg);

        using IterType = std::istreambuf_iterator<char>;
        out.assign(IterType(is), IterType());

        return out;
    }

    // Private member function to traverse and store node information
    std::size_t get_info_impl(const ryml::ConstNodeRef& node)
    {
        // Store the size of the current node in the data vector
        std::size_t nodeId{node.id()};
        DEBUG_ASSERT(nodeId < data_.size());

        auto& node_size = data_[nodeId].size;

        if (node_size != 0)
            return node_size;

        if (!node.is_container())
            return node_size = 1;

        if (!node.has_children())
            return node_size;

        // Traverse child nodes recursively
        for (auto child : node.children())
            node_size += get_info_impl(child);

        return node_size;
    }
};