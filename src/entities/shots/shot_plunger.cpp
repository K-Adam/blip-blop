#include "entities/shots/shot_plunger.h"

ShotPlunger::ShotPlunger(int n_pbk_ennemis, int vx, int vy) {
	pic = pbk_enemies[n_pbk_ennemis];
	dy = vy;
	dx = vx;
}

void ShotPlunger::update() {
	x += dx;
	y += dy;

	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	colFromPic();
}
