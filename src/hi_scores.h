#pragma once

#include <string>

const int HS_NB_SCORES = 10;
const int HS_NAME_LENGTH = 20;

class HiScores {
   protected:
	std::string names[HS_NB_SCORES];
	int scores[HS_NB_SCORES];

   public:
	HiScores() { init(); }

	const char* getName(int n) const { return n >= 0 && n < HS_NB_SCORES ? names[n].c_str() : nullptr; }
	int getScore(int n) const { return n >= 0 && n < HS_NB_SCORES ? scores[n] : 0; }
	bool isGood(int scr) const { return scores[HS_NB_SCORES - 1] < scr; }

	void init();
	void add(int scr, const char* name);
	bool save(const char* file);
	bool load(const char* file);
};
