#pragma once

#include <string>
#include <string_view>

const int HS_NB_SCORES = 10;
const int HS_NAME_LENGTH = 20;

class HiScores {
   protected:
	std::string names[HS_NB_SCORES];
	int scores[HS_NB_SCORES];

   public:
	HiScores() { init(); }

	std::string_view getName(int n) const { return n >= 0 && n < HS_NB_SCORES ? std::string_view(names[n]) : std::string_view(); }
	int getScore(int n) const { return n >= 0 && n < HS_NB_SCORES ? scores[n] : 0; }
	bool isGood(int scr) const { return scores[HS_NB_SCORES - 1] < scr; }

	void init();
	void add(int scr, const std::string& name);
	bool save(const char* file);
	bool load(const char* file);
};
