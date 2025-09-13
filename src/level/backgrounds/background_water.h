#pragma once

#include "entities/sprite.h"

int water_seed = 0;
int water_seed2 = 5;

class BackgroundWater1 : public Sprite {
   public:
	void update() override {
		pic = pbk_level[7 + water_seed];

		if (x < offset - 400) destroyed = true;
	};
};

class BackgroundWater2 : public Sprite {
	void update() override {
		pic = pbk_level[7 + water_seed2];

		if (x < offset - 400) destroyed = true;
	};
};

class BackgroundWater : public Sprite {
   public:
	BackgroundWater() {
		Sprite* ptr;
		bool b = false;

		for (int i = 0; i < ((((int)level_size) / pbk_level[7]->xSize()) + 1); i++) {
			if (b)
				ptr = new BackgroundWater1();
			else
				ptr = new BackgroundWater2();

			ptr->x = x;
			ptr->y = 454;

			x += 376;
			b = !b;

			list_animated_bgs.emplace_back(ptr);
		}
	};

	void update() override {
		anim_tick += 1;
		anim_tick %= 7;

		if (anim_tick == 0) {
			water_seed += 1;
			water_seed %= 10;

			water_seed2 += 1;
			water_seed2 %= 10;
		}
	};
};
