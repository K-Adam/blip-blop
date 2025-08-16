#pragma once

#include "entities/sprite.h"

class BackgroundPokeArrow : public Sprite {
   public:
	void update() override {
		static const int anim_go[] = {42, 43, 44, 45, 46, 47, 46, 45, 44, 43, 42};

		pic = pbk_niveau[anime(anim_go, 11, 3)];
	};
};
