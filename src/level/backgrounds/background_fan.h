#pragma once

#include "entities/sprite.h"

class BackgroundFan : public Sprite {
   public:
	void update() override {
		step(3, 4);

		pic = assets().pics.level.anim("fan_spin")[anim_step];

		if (x < level().camera.offset - 100) destroyed = true;
	};
};
