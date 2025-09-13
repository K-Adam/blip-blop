#pragma once

#include "entities/shots/shot.h"

class ShotBisouDeadly : public Shot {
   public:
	Sprite* cible;
	int dx;
	int duree_vie;

	ShotBisouDeadly() : dx(0), duree_vie(235){};

	int damage() const override { return 1; };

	int enflame() const override { return 0; };

	void setDir(int d) override { dir = d; };

	void update() override {
		static const int ACCEL = 1;

		anim_tick += 1;
		anim_tick %= 5;

		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 2;
		}

		if (dx > 0)
			dir = FACING_RIGHT;
		else
			dir = FACING_LEFT;

		if (dir == FACING_RIGHT)
			pic = pbk_enemies[311 + anim_step];
		else
			pic = pbk_enemies[313 + anim_step];

		x += dx / 13;
		y += dy / 13;

		duree_vie -= 1;

		if (duree_vie <= 0 || cible->destroyed) {
			destroyed = true;
			return;
		}

		int xc = cible->x;
		int yc = cible->y - 15;

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
