#include "resolver.h"

#include "debug/assert.h"
#include "debug/print.h"
#include "debug/throw.h"

#include "utils/io.h"
#include "utils/string.h"

#include <fstream>

#include "ryml.hpp"

YamlResolverError::YamlResolverError(std::string const& what)
    : std::runtime_error(what)
{
}

YamlResolver::YamlResolver(const std::string& content) : content_(content)
{
    tree_ = ryml::parse_in_place(get_clean_content(content_));
}

YamlResolver::YamlResolver(std::istream& is)
    : YamlResolver(io_utils::get_file_content(is))
{
}

// Function to optimize the YAML configuration
void YamlResolver::resolve() { tree_.resolve(); }

// Function to retrieve the currently stored YAML string
std::string YamlResolver::str() const
{
    std::ostringstream os;
    write_to_ostream(os);
    return os.str();
}

void YamlResolver::dump(std::string const& filename)
{
    std::ofstream of(filename);

    if (!of.good())
        YO_THROW(YamlResolverError, "Failed to create output file with name {}",
                 filename);

    write_to_ostream(of);
}

void YamlResolver::write_to_ostream(std::ostream& os) const
{
    if (is_utf8)
        os << BOM;

    os << tree_;
}

ryml::substr YamlResolver::get_clean_content(std::string& content)
{
    std::size_t offset = 0;
    is_utf8 = string_utils::starts_with(content, BOM);
    if (is_utf8)
        offset = BOM.length();

    return {content.data() + offset, content.length() - offset};
}
