#pragma once

#include "optimizer/settings.h"

#include <istream>
#include <string>
#include <vector>

#include "ryml.hpp"

#define NAMED_PRIVATE_SECTION(name) private:

/**
 * @brief The YamlOptimizer class performs optimization on a YAML configuration.
 *
 * It optimizes the configuration by identifying similar nodes and replacing
 * them with anchors and references.
 */
class YamlOptimizer
{
public:
    /**
     * @brief Constructs a YamlOptimizer object from a YAML string.
     *
     * @param content The YAML string to optimize.
     * @param settings The optimization settings to use.
     */
    explicit YamlOptimizer(std::string const& content,
                           OptimizationSettings const& settings = {});

    /**
     * @brief Constructs a YamlOptimizer object from an input stream.
     *
     * @param is The input stream containing the YAML content to optimize.
     * @param settings The optimization settings to use.
     */
    explicit YamlOptimizer(std::istream& is,
                           OptimizationSettings const& settings = {});

    /**
     * @brief Performs optimization on the YAML configuration.
     *
     * It identifies similar nodes and replaces them with anchors and
     * references.
     */
    void optimize();

    /**
     * @brief Returns the currently stored YAML string.
     *
     * @return The optimized YAML string.
     */
    std::string str() const;

    /**
     * @brief Writes the currently stored YAML string to a file.
     *
     * @param filename The file name to which to write the YAML string.
     */
    void dump(std::string const& filename);

private:
    NAMED_PRIVATE_SECTION(Input data)

    const OptimizationSettings settings_;

    std::string content_;

    static constexpr std::string_view BOM{"\xEF\xBB\xBF"};
    bool is_utf8 = false;

    NAMED_PRIVATE_SECTION(Processed data)

    struct NodeInfo
    {
        std::size_t size;
    };

    std::vector<NodeInfo> data_;
    ryml::Tree tree_;
    std::size_t anchor_count_ = 0;

    NAMED_PRIVATE_SECTION(Tree pre - processing utils)

    void get_info();
    std::size_t get_info_impl(ryml::ConstNodeRef const& node);

    NAMED_PRIVATE_SECTION(Node utils)

    bool nodes_equal(ryml::ConstNodeRef const& a,
                     ryml::ConstNodeRef const& b) const;
    bool long_types_equal(ryml::ConstNodeRef const& a,
                          ryml::ConstNodeRef const& b) const;

    NAMED_PRIVATE_SECTION(IO utils)

    void write_to_ostream(std::ostream& os) const;
    ryml::substr get_clean_content(std::string& content);

#ifdef YO_DEBUG
    NAMED_PRIVATE_SECTION(Debug utils)

    void debug_print_data() const;

#endif // YO_DEBUG
};
