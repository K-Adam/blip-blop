#include "entities/shots/shot_snorky_governor.h"

ShotSnorkyGovernor::ShotSnorkyGovernor(Character* cible, int vx, int vy) : joueur_cible(cible), dx(vx), etape_dx(0), etape_dy(0), time(0) { dy = vy; }

void ShotSnorkyGovernor::update() {
	anim_tick++;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 4;
	}

	if (x < offset - 100 || x > offset + 740 || y > 580 || y < -100) destroyed = true;

	time++;

	if ((time < TIME_MAX || ((dx < 2 && dx > -2) && (dy < 2 && dy > -2))) && (joueur_cible->destroyed == false)) {
		if (x < joueur_cible->x && dx < SPEED_MAX) {
			etape_dx += 1;

			if (etape_dx > INERTIE_TIR) {
				dx += 1;
				etape_dx = 0;
			}
		}

		else if (x > joueur_cible->x && dx > (-SPEED_MAX)) {
			etape_dx -= 1;

			if (etape_dx < (-INERTIE_TIR)) {
				dx -= 1;
				etape_dx = 0;
			}
		}

		if (y < joueur_cible->y && dy < SPEED_MAX) {
			etape_dy += 1;

			if (etape_dy > INERTIE_TIR) {
				dy += 1;
				etape_dy = 0;
			}
		}

		else if (y > joueur_cible->y && dy > (-SPEED_MAX)) {
			etape_dy -= 1;

			if (etape_dy < (-INERTIE_TIR)) {
				dy -= 1;
				etape_dy = 0;
			}
		}
	}

	x += dx;
	y += dy;

	if (dx <= 0) {
		pic = pbk_enemies[233 + anim_step];
	} else {
		pic = pbk_enemies[236 - anim_step];
	}

	if (solid_wall(x + dx, y + dy) != 0) destroyed = true;

	colFromPic();
}
