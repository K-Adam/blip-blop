#include "entities/enemies/enemy.h"
#include "level/world.h"
#include "core/assets.h"
#include "entities/make_bonus.h"
#include "entities/splatter.h"

Character* targeted_player;
int wait_for_bonus;
int num_giclure;

Enemy::Enemy() : xmin(offset), blood(0), tresor(5) {}

void Enemy::takeHit(Shot* shot) {
	int blessure = shot->damage();

	if (blessure < hp) {
		shot->aTouche(blessure);
		hp -= blessure;
	} else {
		shot->aTouche(hp);
		hp = 0;
		anim_step = 0;
		anim_tick = 0;
		col_on = false;

		if (shot->enflame() == 2) {
			status = ETAT_CARBONISE;
			shot->aTue();
		} else {
			status = ETAT_MEURE;
			shot->aTue();

			if (pic != NULL) {
				int y_trace = y - ((pic->ySize()) >> 1) - rand() % 15;

				if (bloody_wall(x, y_trace)) {
					num_giclure += 1;
					num_giclure %= 14;

					engrave(x, y_trace, pbk_misc[50 + num_giclure]);
				}
			}
		}

		if (count()) {
			wait_for_bonus += tresor;

			if (wait_for_bonus >= 50 && okBonus) {
				MakeBonusWeapon(x, y);
				wait_for_bonus = 0;
			}

			game_flag[FLAG_NB_KILL] += 1;
		}
	}
}

void Enemy::gicle(const Shot* shot, const int* dxg, const int* dyg) {
	if (blood > 0 || shot->enflame()) return;

	Splatter* gicle = new Splatter();

	gicle->pere = this;
	int d = gicle->dir = shot->getGiclureDir();
	gicle->deltax = dxg[d];
	gicle->deltay = dyg[d];

	blood = 20;

	list_splatters.emplace_back(gicle);
}

void Enemy::tirEnCloche(int xtir, int ytir, int xci, int yci, int& xspeed, int& yspeed) const {
	int netape = 0;
	int ddx = xci - xtir;

	if (ytir < yci) {
		yspeed = rand() % 3 - 5;

		int ddy = yspeed;

		while (ytir < yci && netape < 40) {
			ytir += ddy * GRAVITY_DELAY;

			if (ddy < GRAVITY_MAX) ddy += 1;

			netape += 1;
		};
	} else {
		yspeed = rand() % 2 - 6;

		int ddy = yspeed;

		while (ytir > yci && netape < 40) {
			ytir += ddy * GRAVITY_DELAY;

			if (ddy < GRAVITY_MAX) ddy += 1;

			netape += 1;
		};

		ddx += ddx / 2;
	}

	if (netape == 0)
		xspeed = 0;
	else {
		xspeed = ddx / (netape * GRAVITY_DELAY);

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
