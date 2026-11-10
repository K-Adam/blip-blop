#include "entities/shots/shot_thorn.h"

ShotThorn::ShotThorn(const Picture* p, int vx, int vy, bool through_walls) : through_walls(through_walls) {
	pic = p;
	dy = vy;
	dx = vx;
}

void ShotThorn::update() {
	x += dx;
	y += dy;

	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50 || (!through_walls && solid_wall(x, y))) destroyed = true;

	colFromPic();
}
