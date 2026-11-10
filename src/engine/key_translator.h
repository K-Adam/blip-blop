#pragma once

#include <string>

std::string DIK_to_string(int n, bool french);

// Stable English names for the settings file, unlike the localized DIK_to_string
std::string codeName(unsigned int code);
constexpr unsigned int UNKNOWN_CODE = ~0u;

// 0 for "", UNKNOWN_CODE when the name matches nothing
unsigned int codeFromName(const std::string& name);
