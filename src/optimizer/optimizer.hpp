#pragma once

#include "../debug/assert.hpp"
#include "../debug/print.hpp"

#include "../utils/string.hpp"

#include <fstream>
#include <istream>
#include <string>
#include <vector>

#include "ryml.hpp"

class YamlOptimizer
{
    // NodeInfo struct to store node information
    struct NodeInfo
    {
        std::size_t size;

#ifdef YO_DEBUG
        // Overload the operator<< function to print NodeInfo
        friend std::ostream& operator<<(std::ostream& os, const NodeInfo& info)
        {
            return os << "NodeInfo { size: " << info.size << " }";
        }
#endif // YO_DEBUG
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
            std::string val;
            if (tree_.ref(i).has_val())
                val = {tree_.ref(i).val().data(), tree_.ref(i).val().size()};
            DEBUG_PRINT("{}({}) : {}", i, val, data_[i].size);
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
        // Perform the optimization logic here
        // ...
    }

    // Function to retrieve the currently stored YAML string
    std::string str() const
    {
        std::ostringstream os;

        if (is_utf8)
            os << BOM;

        os << tree_ << std::endl;
        return os.str();
    }

private:
    std::vector<NodeInfo> data_;
    ryml::Tree tree_;
    std::string content_;

    static constexpr std::string_view BOM{"\xEF\xBB\xBF"};
    bool is_utf8 = false;

    void get_info() { get_info_impl(tree_.rootref()); }

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