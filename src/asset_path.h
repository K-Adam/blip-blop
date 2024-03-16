#pragma once

#include <string>
#include <cstring>

inline std::string asset_path(const char* path) {
	std::string root = "assets/";
	return root + path;
}

inline std::string asset_path_prefix(const char* prefix, const char* path) {
	std::string root = "assets/";
	if (strstr(path, prefix) != path) {
		return root + prefix + "/" + path;
	} else {
		return root + path;
	}
}

inline std::string remove_quotes(const std::string& str) {
	std::string result = str;

	if (!result.empty() && result.front() == '"' && result.back() == '"') {
		result.erase(result.begin());
		result.pop_back();
	}

	return result;
}
