#pragma once

#include "entities/sprite.h"

class BackgroundFan : public Sprite {
   public:
	void update() override {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 4;
		}

		pic = pbk_level[3 + anim_step];

		if (x < offset - 100) destroyed = true;
	};
};
