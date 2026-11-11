#include "entities/shots/shot_cake.h"
#include "core/debug.h"

ShotCake::ShotCake() { pic = assets().pics.enemies.pic("smurf_greedy_cake"); }

void ShotCake::update() {
	grav_tick += 1;
	grav_tick %= GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

	y += dy;

	x += dx;

	if (level().terrain.solid_wall(x, y) || x < level().camera.offset - 100 || x > level().camera.offset + 700 || y > 520 || y < -50) destroyed = true;

	colFromPic();
}
