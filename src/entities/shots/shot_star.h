#pragma once

#include "entities/shots/shot.h"

class ShotStar : public Shot {
   public:
	int life_time;

	ShotStar() : life_time(500){};

	void update() override {
		static const int SPEED = 2;

		fall();

		if (platform_y(x, y) != 0 || platform_y(x, y + 5)) dy = -3 - rand() % 2;

		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 24;
		}

		pic = pbk_enemies.anim("star_shot")[anim_step];

		if (dir == FACING_RIGHT) {
			if (solid_wall(x + SPEED, y))
				dir = FACING_RIGHT;
			else
				x += SPEED;
		} else {
			if (solid_wall(x - SPEED, y))
				dir = FACING_RIGHT;
			else
				x -= SPEED;
		}

		life_time -= 1;

		if (solid_wall(x, y) || life_time <= 0) destroyed = true;

		colFromPic();
		updateDestroyed();
	};
};
