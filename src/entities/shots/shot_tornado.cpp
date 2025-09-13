#include "entities/shots/shot_tornado.h"

ShotTornado::ShotTornado(int vx) : dx(vx), etape_speed(0), nb_tour(0) { dy = 0; }

void ShotTornado::update() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step++;
		if ((anim_step > 5) && (nb_tour < TOUR_MAX)) {
			anim_step = 0;
			nb_tour++;
		}
	}

	lat_grav += 1;
	if (dy > -7) {
		lat_grav %= LATENCE_TORNADE + 5 * dy;
	} else {
		lat_grav = 0;
	}

	if (lat_grav == 0) {
		dy -= 1;
	}

	y += dy;

	if (anim_step == 10) {
		destroyed = true;
		return;
	}

	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	etape_speed++;
	etape_speed %= TORNADE_ACCELERATION;
	if ((etape_speed == 0) && (abs(dx) < SPEED_MAX)) {
		if (dir == FACING_RIGHT) {
			dx++;
		} else {
			dx--;
		}
	}

	x += dx;

	if (dx <= 0) {
		pic = pbk_enemies[80 + anim_step];
	} else {
		pic = pbk_enemies[63 + anim_step];
	}
	if (anim_step <= 8) {
		colFromPic();
	}
}
