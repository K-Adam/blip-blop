#pragma once

#include <cstdlib>

#include "entities/sprite.h"

// Body parts thrown by the exploding kiss enemy, drift is the sideways speed when facing right
class GoreBisouBoom : public Sprite {
   public:
	GoreBisouBoom(const char* right, const char* left, int drift) : right_(right), left_(left), drift_(drift) {}

	void update() override {
		fall();

		if (anim_step < 5) {
			anim_tick += 1;
			anim_tick %= 8;

			if (anim_tick == 0) anim_step += 1;

			pic = pbk_enemies.anim(dir == FACING_RIGHT ? right_ : left_)[anim_step];
		}

		if ((platform_y(x, y) != 0 && anim_step >= 5) || solid_wall(x, y)) {
			engrave(x, y, pic);
			destroyed = true;
			sbk_misc.play(6 + (rand() % 2));
		} else {
			x += dir == FACING_RIGHT ? drift_ : -drift_;
		}

		updateDestroyed();
	};

   private:
	const char* right_;
	const char* left_;
	int drift_;
};
