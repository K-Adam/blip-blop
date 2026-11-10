#include "entities/shots/shot_smurf_wild.h"

ShotSmurfWild::ShotSmurfWild() { pic = assets().pics.enemies.pic("smurf_wild_shot"); }

void ShotSmurfWild::update() {
	x += dx;
	y += dy;

	if (level().terrain.solid_wall(x, y) || x < level().camera.offset - 20 || x > level().camera.offset + 660 || y < -20 || y > 500) {
		pic = NULL;
		noCol();
		destroyed = true;
	} else {
		colFromPic();
	}
}
