#include "entities/shots/shot_bb_lf.h"

void ShotBBLF::onHit(int pts) {
	if (!destroyed) {
		ShotBB::onHit(pts);
		destroyed = false;
	}
}

void ShotBBLF::setDir(int d) {
	ShotBB::setDir(d);

	anim_step = 0;
	anim_tick = 0;
	delay = 0;
	type = rand() % 2;
	finished = false;
	pic = NULL;

	if (type == 1)
		speed = 2 + rand() % 2;
	else if (type == 0)
		speed = 4 + rand() % 3;
	else
		speed = 5 + rand() % 3;

	switch (dir) {
		case 0:
			dx = 1 - rand() % 3;
			dy = 3 + rand() % 3;
			break;

		case 1:
			dx = 1 + rand() % 3;
			dy = 2 + rand() % 3;
			break;

		case 2:
			dx = 3 + rand() % 3;
			dy = 1 + rand() % 3;
			break;

		case 3:
			dx = 3 + rand() % 3;
			dy = rand() % 3;
			break;

		case 4:
			dx = 3 + rand() % 3;
			dy = 1 - rand() % 3;
			break;

		case 5:
			dx = 3 + rand() % 3;
			dy = -rand() % 3;
			break;

		case 6:
			dx = 3 + rand() % 3;
			dy = -1 - rand() % 3;
			break;

		case 7:
			dx = rand() % 3;
			dy = -2 - rand() % 3;
			break;

		case 8:
			dx = 1 - rand() % 3;
			dy = -3 - rand() % 3;
			break;

		case 9:
			dx = -rand() % 3;
			dy = -3 - rand() % 3;
			break;

		case 10:
			dx = -3 - rand() % 3;
			dy = -1 - rand() % 3;
			break;

		case 11:
			dx = -3 - rand() % 3;
			dy = -rand() % 3;
			break;

		case 12:
			dx = -3 - rand() % 3;
			dy = 1 - rand() % 3;
			break;

		case 13:
			dx = -3 - rand() % 3;
			dy = rand() % 3;
			break;

		case 14:
			dx = -3 - rand() % 3;
			dy = 1 + rand() % 3;
			break;

		case 15:
			dx = -1 - rand() % 3;
			dy = 2 + rand() % 3;
			break;
	}
}

void ShotBBLF::update() {
	anim_tick += 1;
	anim_tick %= speed;

	if (anim_tick == 0) {
		if (finished) {
			anim_step -= 1;

			if (type == 0) {
				if (anim_step < 0)
					destroyed = true;
				else
					pic = pbk_bb[212 + anim_step];
			} else if (type == 1) {
				if (anim_step < 0)
					destroyed = true;
				else
					pic = pbk_bb[219 + anim_step];
			} else {
				if (anim_step < 0)
					destroyed = true;
				else
					pic = pbk_bb[231 + anim_step];
			}
		} else {
			anim_step += 1;

			if (type == 0) {
				if (anim_step >= 5)
					finished = true;
				else
					pic = pbk_bb[212 + anim_step];
			} else if (type == 1) {
				if (anim_step >= 10)
					finished = true;
				else
					pic = pbk_bb[219 + anim_step];
			} else {
				if (anim_step >= 5)
					finished = true;
				else
					pic = pbk_bb[231 + anim_step];
			}
		}
	}

	x += dx;
	y += dy;

	delay += 1;
	delay %= 17;

	if (delay == 0 && dy > -3) dy--;

	colFromPic();
	updateDestroyed();
}
