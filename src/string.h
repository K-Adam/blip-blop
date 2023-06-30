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

inline std::string asset_path(const char* path)
{
	std::string root = "assets/";
	return root + path;
}

inline std::string asset_path_prefix(const char* prefix, const char* path)
{
	std::string root = "assets/";
	if (strstr(path, prefix) != path) {
		return root + prefix + "/" + path;
	}
	else {
		return root + path;
	}
}

inline std::string remove_quotes(const std::string& str) {
	std::string result = str;

	if (!result.empty() && result.front() == '"' && result.back() == '"') {
		result.erase(result.begin());  // Remove the first character
		result.pop_back();  // Remove the last character
	}

	return result;
}
