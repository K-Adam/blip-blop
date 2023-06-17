
#include <stdio.h>
#include <string.h>
#include <fstream>
#include "hi_scores.h"
#include "json.h"

using json = nlohmann::json;

void HiScores::init()
{
	int	scr = 10000 * HS_NB_SCORES;

	for (int i = 0; i < HS_NB_SCORES; i++) {
		scores[i] = scr;
		strcpy(names[i], "LOADED STUDIO");
		scr -= 10000;
	}
}

void HiScores::add(int scr, const char * name)
{
	int		i = 0;

	while (i < HS_NB_SCORES && scores[i] > scr)
		i++;

	int		j = HS_NB_SCORES - 1;

	while (j > i) {
		scores[j] = scores[j - 1];
		strcpy(names[j], names[j - 1]);

		j--;
	}

	if (i < HS_NB_SCORES) {
		scores[i] = scr;
		strcpy(names[i], name);
	}
}

bool HiScores::save(const char * file)
{
	json result;
	result["items"] = json::array();
	for (int i = 0; i < HS_NB_SCORES; i++) {
		json item;

		item["name"] = names[i];
		item["score"] = scores[i];

		result["items"].push_back(item);
	}

	std::ofstream out(file);
	out << result.dump(2);

	return true;
}

bool HiScores::load(const char * file)
{
	std::ifstream input(file);
	if (!input.good()) {
		return false;
	}

	json data = json::parse(input);

	int index = 0;
	for (auto item : data["items"]) {

		std::string name = item["name"];
		int score = item["score"];

		scores[index] = score;
		size_t length = std::min(name.length(), (size_t)HS_NAME_LENGTH - 1);
		std::copy(name.begin(), name.begin() + length, names[index]);
		names[index][length] = '\0';

		if (++index > HS_NB_SCORES) {
			break;
		}
	}

	return true;
}
