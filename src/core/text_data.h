#pragma once

#include <string>
#include <vector>

// Localized strings, indexed by the TXT_ ids
class TextData {
	std::vector<std::string> lines_;

   public:
	bool load(const char* file);
	const std::string& operator[](size_t id) const { return lines_[id]; }
};
