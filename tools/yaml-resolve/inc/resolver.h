#pragma once

#include <istream>
#include <string>

#include "ryml.hpp"

class YamlResolverError : public std::runtime_error
{
public:
    YamlResolverError(std::string const& what);
};

class YamlResolver
{
public:
    // Constructor
    explicit YamlResolver(const std::string& content);

    explicit YamlResolver(std::istream& is);

    // Function to optimize the YAML configuration
    void resolve();

    // Function to retrieve the currently stored YAML string
    std::string str() const;

    void dump(std::string const& filename);

private:
    ryml::Tree tree_;
    std::string content_;

    static constexpr std::string_view BOM{"\xEF\xBB\xBF"};
    bool is_utf8 = false;

    void write_to_ostream(std::ostream& os) const;

    ryml::substr get_clean_content(std::string& content);
};
