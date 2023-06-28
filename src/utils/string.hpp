#include <algorithm>
#include <string_view>

namespace string_utils
{

// Function to check if a string starts with a given substring
bool starts_with(std::string_view str, std::string_view start)
{
    if (start.length() > str.length())
        return false;

    return std::equal(start.begin(), start.end(), str.begin());
}

// Function to check if a string ends with a given substring
bool ends_with(std::string_view str, std::string_view end)
{
    if (end.length() > str.length())
        return false;

    return std::equal(end.rbegin(), end.rend(), str.rbegin());
}

} // namespace string_utils
