#pragma once

#include <string>
#include <vector>

#ifndef TXT_DATA_CPP
extern std::vector<std::string> txt_data;
#endif

bool loadTxtData(const char* file);
