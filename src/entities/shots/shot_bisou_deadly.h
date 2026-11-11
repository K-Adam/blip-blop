#pragma once

#include "entities/shots/shot.h"

class ShotBisouDeadly : public Shot {
   public:
	Sprite* target;
	int dx;
	int lifespan;

	ShotBisouDeadly() : dx(0), lifespan(235){};

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override {
		static const int ACCEL = 1;

		step(5, 2);

		if (dx > 0)
			dir = FACING_RIGHT;
		else
			dir = FACING_LEFT;

		pic = facing(assets().pics.enemies, "smurfette_kiss")[anim_step];

		x += dx / 13;
		y += dy / 13;

		lifespan -= 1;

		if (lifespan <= 0 || target->destroyed) {
			destroyed = true;
			return;
		}

		int xc = target->x;
		int yc = target->y - 15;

		if (xc > x && dx < 36)
			dx += ACCEL;
		else if (dx > -36)
			dx -= ACCEL;

		if (yc > y && dy < 36)
			dy += ACCEL;
		else if (dy > -36)
			dy -= ACCEL;

		colFromPic();
		updateDestroyed();
	};
};
