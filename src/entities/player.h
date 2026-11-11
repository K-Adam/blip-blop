#pragma once

class Player {
   protected:
	int score = 0;
	int nb_killed = 0;

   public:
	inline void setKilled(int n) { nb_killed = n; };

	inline void addKilled() { nb_killed += 1; };

	inline virtual int getScore() { return score; };

	inline virtual int getKilled() { return nb_killed; };

	inline virtual void setScore(int ns) { score = ns; };

	inline virtual void addPoints(int p) { score += p; };
};
