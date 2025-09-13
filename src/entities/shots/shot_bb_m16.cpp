#include "entities/shots/shot_bb_m16.h"

void ShotBBM16::update() {
	moveAlongAngle();

	if (solid_wall(x, y) || x < offset - 20 || x > offset + 690 || y < -20 || y > 500) {
		pic = NULL;
		noCol();
		destroyed = true;
	} else {
		pic = pbk_bb[72 + dir];
		colFromPic();
	}
}
