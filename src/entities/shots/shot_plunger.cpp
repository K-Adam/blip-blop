#include "entities/shots/shot_plunger.h"

ShotPlunger::ShotPlunger(const Picture* p, int vx, int vy) {
	pic = p;
	dy = vy;
	dx = vx;
}

void ShotPlunger::update() {
	x += dx;
	y += dy;

	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	colFromPic();
}
