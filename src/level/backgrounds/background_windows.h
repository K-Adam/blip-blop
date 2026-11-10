#pragma once

#include "entities/sprite.h"

class BackgroundWindows : public Sprite {
   public:
	void update() override {
		step(180, 4);

		int i = 0;

		switch (anim_step) {
			case 1:
			case 2:
				i = 2;
				break;
			case 3:
				i = 3;
				break;
		}

		pic = pbk_level.anim("windows")[i];

		updateDestroyed();
	};
};
