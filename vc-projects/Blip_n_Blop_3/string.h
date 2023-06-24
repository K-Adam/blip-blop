#pragma once

#include <string>
#include <cstring>

template<size_t MAX_LENGTH>
inline void copyStringToCharArray(const std::string& str, char* charArray)
{
    static_assert(MAX_LENGTH > 0);

    // Copy the string to the char array
    strncpy(charArray, str.c_str(), MAX_LENGTH - 1);

    // Add a null terminator at the end of the char array
    charArray[MAX_LENGTH - 1] = '\0';
}
