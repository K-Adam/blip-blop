#include "entities/shots/shot_thorn.h"

ShotThorn::ShotThorn(const Picture* p, int vx, int vy) {
	pic = p;
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
