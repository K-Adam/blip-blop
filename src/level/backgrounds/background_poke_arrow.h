#pragma once

#include "entities/sprite.h"

class BackgroundPokeArrow : public Sprite {
   public:
	void update() override {
		static const int anim_go[] = {0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0};

		pic = pbk_level.anim("arrow_cargo_storage")[animate(anim_go, 11, 3)];
	};
};
