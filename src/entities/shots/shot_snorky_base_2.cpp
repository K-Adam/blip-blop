#include "entities/shots/shot_snorky_base_2.h"

ShotSnorkyBase2::ShotSnorkyBase2(int vx, int vy) {
	dy = vy;
	dx = vx;
	pic = pbk_enemies[182];
}

void ShotSnorkyBase2::update() {
	x += dx;
	y += dy;

	if (x < offset - 100 || x > offset + 700 || y > 520 || y < -50) destroyed = true;

	if (solid_wall(x, y) != 0) destroyed = true;

	colFromPic();
}
