#pragma once

#include "entities/shots/shot.h"

class ShotStar : public Shot {
   public:
	int life_time;

	ShotStar() : life_time(500){};

	void update() override {
		static const int SPEED = 2;

		fall();

		if (level().terrain.platform_y(x, y) != 0 || level().terrain.platform_y(x, y + 5)) dy = -3 - rnd() % 2;

		step(3, 24);

		pic = assets().pics.enemies.anim("star_shot")[anim_step];

		if (dir == FACING_RIGHT) {
			if (level().terrain.solid_wall(x + SPEED, y))
				dir = FACING_RIGHT;
			else
				x += SPEED;
		} else {
			if (level().terrain.solid_wall(x - SPEED, y))
				dir = FACING_RIGHT;
			else
				x -= SPEED;
		}

		life_time -= 1;

		if (level().terrain.solid_wall(x, y) || life_time <= 0) destroyed = true;

		colFromPic();
		updateDestroyed();
	};
};
