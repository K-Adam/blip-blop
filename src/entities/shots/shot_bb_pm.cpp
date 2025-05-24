#include "entities/shots/shot_bb_pm.h"

void ShotBBPM::update() {
	moveAlongAngle();

	if (mur_opaque(x, y) || x < offset - 20 || x > offset + 690 || y < -20 || y > 500) {
		pic = NULL;
		noCol();
		a_detruire = true;
	} else {
		pic = pbk_bb[88 + dir];
		colFromPic();
	}
}
