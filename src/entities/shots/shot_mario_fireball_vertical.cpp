#include "entities/shots/shot_mario_fireball_vertical.h"

ShotMarioFireballVertical::ShotMarioFireballVertical(int vx, int xc) : x_cible(xc), dx(vx) { dy = -12; }

void ShotMarioFireballVertical::update() {
	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -250) destroyed = true;
	if (dy > 0) {
		fireball_tombe();
	} else {
		y += dy;
	}

	if (y < -50) {
		x = x_cible;
		dy = 1;
	}

	if (dy < 0) x += dx;

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if (dy > 12) {
		pic = pbk_enemies[45 + anim_step];
	} else if (dy > 6) {
		pic = pbk_enemies[42 + anim_step];
	} else if (dy > 0) {
		pic = pbk_enemies[39 + anim_step];
	}

	if (dy < -12) {
		pic = pbk_enemies[36 + anim_step];
	} else if (dy < -6) {
		pic = pbk_enemies[33 + anim_step];
	} else if (dy < 0) {
		pic = pbk_enemies[30 + anim_step];
	}

	colFromPic();
}

void ShotMarioFireballVertical::fireball_tombe() {
	lat_grav += 1;
	lat_grav %= LATENCE_FIREBALL;

	if (lat_grav == 0 && dy < GRAVITE_FIREBALL) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITE_FIREBALL;

	y += dy;
}
