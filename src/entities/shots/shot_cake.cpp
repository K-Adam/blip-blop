#include "entities/shots/shot_cake.h"
#include "core/debug.h"

ShotCake::ShotCake() { pic = pbk_enemies[194]; }

void ShotCake::update() {
	grav_tick += 1;
	grav_tick %= GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

	y += dy;

	x += dx;

	if (solid_wall(x, y) || x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	colFromPic();
}
