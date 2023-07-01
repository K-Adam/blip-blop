/******************************************************************
 *
 *
 *              -----------------
 *                 TxtData.cpp
 *              -----------------
 *
 *              Permet de charger les textes
 *
 *
 *              Prosper / LOADED -   V 0.1 - 17 Juillet 2000
 *
 *
 *
 ******************************************************************/

#define TXT_DATA_CPP

#include "txt_data.h"

#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "ben_debug.h"
#include <json.h>
#include "string.h"

using json = nlohmann::json;

std::vector<std::string> txt_data;

bool loadTxtData(const char* file) {
	auto dir = asset_path(file);
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		return false;
	}

	json data = json::parse(input);
	auto items = data["items"];

	txt_data.resize(1024);

	std::string buffer;
	for (json::iterator it = items.begin(); it != items.end(); it++) {
		std::string key = it.key();

		unsigned long num = std::stoul(key);

		if (num >= txt_data.size()) {
			txt_data.resize(num + 1);
		}

		txt_data[num] = it.value();
	}

	return true;
}
