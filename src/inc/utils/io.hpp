#pragma once

#include <istream>
#include <string>

namespace io_utils
{
std::string get_file_content(std::istream& is)
{
    std::string out;

    is.seekg(0, std::ios::end);
    out.reserve(static_cast<std::string::size_type>(is.tellg()));
    is.seekg(0, std::ios::beg);

    using IterType = std::istreambuf_iterator<char>;
    out.assign(IterType(is), IterType());

    return out;
}
} // namespace io_utils
