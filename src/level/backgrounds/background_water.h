#pragma once

#include "entities/sprite.h"

int water_seed = 0;
int water_seed2 = 5;

class BackgroundWater1 : public Sprite {
   public:
	void update() override {
		pic = pbk_niveau[7 + water_seed];

		if (x < offset - 400) a_detruire = true;
	};
};

class BackgroundWater2 : public Sprite {
	void update() override {
		pic = pbk_niveau[7 + water_seed2];

		if (x < offset - 400) a_detruire = true;
	};
};

class BackgroundWater : public Sprite {
   public:
	BackgroundWater() {
		Sprite* ptr;
		bool b = false;

		for (int i = 0; i < ((((int)level_size) / pbk_niveau[7]->xSize()) + 1); i++) {
			if (b)
				ptr = new BackgroundWater1();
			else
				ptr = new BackgroundWater2();

			ptr->x = x;
			ptr->y = 454;

			x += 376;
			b = !b;

			list_fonds_animes.emplace_back(ptr);
		}
	};

	void update() override {
		ss_etape += 1;
		ss_etape %= 7;

		if (ss_etape == 0) {
			water_seed += 1;
			water_seed %= 10;

			water_seed2 += 1;
			water_seed2 %= 10;
		}
	};
};
