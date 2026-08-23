#include "entities/shots/shot_bb_pm.h"
#include "entities/sprites.h"

void ShotBBPM::update() {
	moveAlongAngle();

	if (solid_wall(x, y) || x < offset - 20 || x > offset + 690 || y < -20 || y > 500) {
		pic = NULL;
		noCol();
		destroyed = true;
	} else {
		pic = pbk_bb[BB::SHOT_PELLET];
		colAlongAngle();
	}
}
