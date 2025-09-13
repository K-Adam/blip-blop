#include "entities/shots/shot_thorn.h"

ShotThorn::ShotThorn(int n_pbk_enemies, int vx, int vy) {
	pic = pbk_enemies[n_pbk_enemies];
	dy = vy;
	dx = vx;
	traverse = false;
}

void ShotThorn::update() {
	x += dx;
	y += dy;

	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50 || (!traverse && solid_wall(x, y))) destroyed = true;

	colFromPic();
}
