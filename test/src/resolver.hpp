#pragma once

#include "../../src/debug/assert.hpp"
#include "../../src/debug/print.hpp"

#include "../../src/utils/string.hpp"

#include <fstream>
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

class YamlResolver
{
public:
    // Constructor
    explicit YamlResolver(const std::string& content) : content_(content)
    {
        tree_ = ryml::parse_in_place(get_clean_content(content_));
    }

    explicit YamlResolver(std::istream& is) : YamlResolver(get_file_content(is))
    {
    }

    // Function to optimize the YAML configuration
    void resolve() { tree_.resolve(); }

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
    ryml::Tree tree_;
    std::string content_;

    static constexpr std::string_view BOM{"\xEF\xBB\xBF"};
    bool is_utf8 = false;

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
};
