#include "entities/shots/shot_smurf_wild.h"

ShotSmurfWild::ShotSmurfWild() { pic = pbk_enemies.pic("smurf_wild_shot"); }

void ShotSmurfWild::update() {
	x += dx;
	y += dy;

	if (solid_wall(x, y) || x < offset - 20 || x > offset + 660 || y < -20 || y > 500) {
		pic = NULL;
		noCol();
		destroyed = true;
	} else {
		colFromPic();
	}
}
