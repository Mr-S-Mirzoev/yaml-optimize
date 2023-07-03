#pragma once

#include <string>

namespace string_utils
{

// Function to check if a string starts with a given substring
bool starts_with(std::string_view str, std::string_view start);

// Function to check if a string ends with a given substring
bool ends_with(std::string_view str, std::string_view end);

class StringDiff
{
    std::string lhs_;
    std::string rhs_;

public:
    StringDiff(const std::string& lhs, const std::string& rhs);

    std::string str() const;

    friend std::ostream& operator<<(std::ostream& os, StringDiff const& diff);
}; // class StringDiff

} // namespace string_utils
