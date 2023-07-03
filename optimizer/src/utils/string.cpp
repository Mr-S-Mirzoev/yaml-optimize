#include "utils/string.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string_view>

namespace string_utils
{

bool starts_with(std::string_view str, std::string_view start)
{
    if (start.length() > str.length())
        return false;

    return std::equal(start.begin(), start.end(), str.begin());
}

bool ends_with(std::string_view str, std::string_view end)
{
    if (end.length() > str.length())
        return false;

    return std::equal(end.rbegin(), end.rend(), str.rbegin());
}

StringDiff::StringDiff(const std::string& lhs, const std::string& rhs)
    : lhs_(lhs), rhs_(rhs)
{
}

std::string StringDiff::str() const
{
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, StringDiff const& diff)
{
    std::istringstream iss_lhs(diff.lhs_);
    std::istringstream iss_rhs(diff.rhs_);

    std::string line_lhs, line_rhs;
    int line_no = 1;

    while (std::getline(iss_lhs, line_lhs) && std::getline(iss_rhs, line_rhs))
    {
        if (line_lhs != line_rhs)
        {
            os << line_no << "c" << std::endl;
            os << "< " << line_lhs << std::endl;
            os << "---" << std::endl;
            os << "> " << line_rhs << std::endl;
        }

        ++line_no;
    }

    // Print remaining lines if the strings have different line counts
    while (std::getline(iss_lhs, line_lhs))
    {
        os << line_no << "d" << std::endl;
        os << "< " << line_lhs << std::endl;
        ++line_no;
    }

    while (std::getline(iss_rhs, line_rhs))
    {
        os << line_no << "a" << std::endl;
        os << "> " << line_rhs << std::endl;
        ++line_no;
    }

    return os;
}

} // namespace string_utils
