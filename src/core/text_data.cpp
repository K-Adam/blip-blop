#include "core/text_data.h"

#include <fstream>

#include "core/debug.h"
#include <json.h>
#include "core/asset_path.h"

using json = nlohmann::json;

bool TextData::load(const char* file) {
	auto dir = asset_path(file);
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		return false;
	}

	json data = json::parse(input);
	auto items = data["items"];

	lines_.resize(1024);

	std::string buffer;
	for (json::iterator it = items.begin(); it != items.end(); it++) {
		std::string key = it.key();

		unsigned long num = std::stoul(key);

		if (num >= lines_.size()) {
			lines_.resize(num + 1);
		}

		lines_[num] = it.value();
	}

	return true;
}
