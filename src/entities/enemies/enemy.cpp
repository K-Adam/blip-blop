#include "entities/enemies/enemy.h"
#include "level/world.h"
#include "core/assets.h"
#include "entities/make_bonus.h"
#include "entities/splatter.h"

Character* targeted_player;
int wait_for_bonus;
int splatter_num;

Enemy::Enemy() : xmin(offset), blood(0), treasure(5) {}

void Enemy::takeHit(Shot* shot) {
	int dmg = shot->damage();

	if (dmg < hp) {
		shot->onHit(dmg);
		hp -= dmg;
	} else {
		shot->onHit(hp);
		hp = 0;
		anim_step = 0;
		anim_tick = 0;
		col_on = false;

		if (shot->enflame() == 2) {
			status = STATE_CHARRED;
			shot->onKill();
		} else {
			status = STATE_DYING;
			shot->onKill();

			if (pic != NULL) {
				int y_trace = y - ((pic->ySize()) >> 1) - rand() % 15;

				if (bloody_wall(x, y_trace)) {
					splatter_num += 1;
					splatter_num %= 14;

					engrave(x, y_trace, pbk_misc.anim("blood_stain")[splatter_num]);
				}
			}
		}

		if (count()) {
			wait_for_bonus += treasure;

			if (wait_for_bonus >= 50 && okBonus) {
				MakeBonusWeapon(x, y);
				wait_for_bonus = 0;
			}

			game_flag[FLAG_NB_KILL] += 1;
		}
	}
}

void Enemy::splatter(const Shot* shot, const int* dxg, const int* dyg) {
	if (blood > 0 || shot->enflame()) return;

	Splatter* splatter = new Splatter();

	splatter->parent = this;
	int d = splatter->dir = shot->getSplatterDir();
	splatter->deltax = dxg[d];
	splatter->deltay = dyg[d];

	blood = 20;

	list_splatters.emplace_back(splatter);
}

void Enemy::lobShot(int xshot, int yshot, int xdest, int ydest, int& xspeed, int& yspeed) const {
	int nsteps = 0;
	int ddx = xdest - xshot;

	if (yshot < ydest) {
		yspeed = rand() % 3 - 5;

		int ddy = yspeed;

		while (yshot < ydest && nsteps < 40) {
			yshot += ddy * GRAVITY_DELAY;

			if (ddy < GRAVITY_MAX) ddy += 1;

			nsteps += 1;
		};
	} else {
		yspeed = rand() % 2 - 6;

		int ddy = yspeed;

		while (yshot > ydest && nsteps < 40) {
			yshot += ddy * GRAVITY_DELAY;

			if (ddy < GRAVITY_MAX) ddy += 1;

			nsteps += 1;
		};

		ddx += ddx / 2;
	}

	if (nsteps == 0)
		xspeed = 0;
	else {
		xspeed = ddx / (nsteps * GRAVITY_DELAY);

		if (ddx < -20)
			xspeed -= 1;
		else if (ddx > 20)
			xspeed += 1;
	}

	if (xspeed < -6)
		xspeed = -6;
	else if (xspeed > 6)
		xspeed = 6;
}
