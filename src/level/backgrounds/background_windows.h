#pragma once

#include "entities/sprite.h"

class BackgroundWindows : public Sprite {
   public:
	void update() override {
		anim_tick += 1;
		anim_tick %= 180;

		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 4;
		}

		int i = 54;

		switch (anim_step) {
			case 1:
			case 2:
				i = 56;
				break;
			case 3:
				i = 57;
				break;
		}

		pic = pbk_level[i];

		updateDestroyed();
	};
};
