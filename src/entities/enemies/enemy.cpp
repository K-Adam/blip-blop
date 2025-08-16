#include "entities/enemies/enemy.h"
#include "level/world.h"
#include "core/assets.h"
#include "entities/make_bonus.h"
#include "entities/splatter.h"

Character* tete_turc;
int wait_for_bonus;
int num_giclure;

Enemy::Enemy() : xmin(offset), blood(0), tresor(5) {}

void Enemy::estTouche(Shot* tir) {
	int blessure = tir->degats();

	if (blessure < pv) {
		tir->aTouche(blessure);
		pv -= blessure;
	} else {
		tir->aTouche(pv);
		pv = 0;
		etape = 0;
		ss_etape = 0;
		col_on = false;

		if (tir->enflame() == 2) {
			etat = ETAT_CARBONISE;
			tir->aTue();
		} else {
			etat = ETAT_MEURE;
			tir->aTue();

			if (pic != NULL) {
				int y_trace = y - ((pic->ySize()) >> 1) - rand() % 15;

				if (mur_sanglant(x, y_trace)) {
					num_giclure += 1;
					num_giclure %= 14;

					grave(x, y_trace, pbk_misc[50 + num_giclure]);
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

void Enemy::gicle(const Shot* tir, const int* dxg, const int* dyg) {
	if (blood > 0 || tir->enflame()) return;

	Splatter* gicle = new Splatter();

	gicle->pere = this;
	int d = gicle->dir = tir->getGiclureDir();
	gicle->deltax = dxg[d];
	gicle->deltay = dyg[d];

	blood = 20;

	list_giclures.emplace_back(gicle);
}

void Enemy::tirEnCloche(int xtir, int ytir, int xci, int yci, int& xspeed, int& yspeed) const {
	int netape = 0;
	int ddx = xci - xtir;

	if (ytir < yci) {
		yspeed = rand() % 3 - 5;

		int ddy = yspeed;

		while (ytir < yci && netape < 40) {
			ytir += ddy * LATENCE_GRAVITE;

			if (ddy < GRAVITE_MAX) ddy += 1;

			netape += 1;
		};
	} else {
		yspeed = rand() % 2 - 6;

		int ddy = yspeed;

		while (ytir > yci && netape < 40) {
			ytir += ddy * LATENCE_GRAVITE;

			if (ddy < GRAVITE_MAX) ddy += 1;

			netape += 1;
		};

		ddx += ddx / 2;
	}

	if (netape == 0)
		xspeed = 0;
	else {
		xspeed = ddx / (netape * LATENCE_GRAVITE);

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
