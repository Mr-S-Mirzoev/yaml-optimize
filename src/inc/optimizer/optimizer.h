#pragma once

#include "optimizer/settings.h"

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
    };

public:
    // Constructor
    explicit YamlOptimizer(std::string const& content,
                           OptimizationSettings const& settings = {});
    explicit YamlOptimizer(std::istream& is,
                           OptimizationSettings const& settings = {});

    // Function to optimize the YAML configuration
    void optimize();

    // Function to retrieve the currently stored YAML string
    std::string str() const;

    void dump(std::string const& filename);

private:
    std::vector<NodeInfo> data_;
    ryml::Tree tree_;
    std::string content_;
    std::size_t anchor_count_ = 0;
    OptimizationSettings settings_;

    static constexpr std::string_view BOM{"\xEF\xBB\xBF"};
    bool is_utf8 = false;

    void get_info();
    std::size_t get_info_impl(ryml::ConstNodeRef const& node);

    bool nodes_equal(ryml::ConstNodeRef const& a,
                     ryml::ConstNodeRef const& b) const;
    bool long_types_equal(ryml::ConstNodeRef const& a,
                          ryml::ConstNodeRef const& b) const;

    void write_to_ostream(std::ostream& os) const;

    ryml::substr get_clean_content(std::string& content);
    static std::string get_file_content(std::istream& is);

    static void set_reference(ryml::NodeRef& node, ryml::csubstr anchor);

#ifdef YO_DEBUG
    void debug_print_data() const;
#endif // YO_DEBUG
};
