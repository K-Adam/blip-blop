#include "entities/shots/shot_bb_lf.h"

#include <cmath>
#include <numbers>

void ShotBBLF::onHit(int pts) {
	if (!destroyed) {
		ShotBB::onHit(pts);
		destroyed = false;
	}
}

void ShotBBLF::aim(double angle) {
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

	// A puff leaves along the aim with a little sideways scatter
	setAngle(angle, 3 + rand() % 3);
	double a = angle * std::numbers::pi / 180;
	double across = 1 - rand() % 3;
	accelerate(std::sin(a) * across, std::cos(a) * across);
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
					pic = pbk_bb.anim("explosion_a")[anim_step];
			} else if (type == 1) {
				if (anim_step < 0)
					destroyed = true;
				else
					pic = pbk_bb.anim("explosion_b")[anim_step];
			} else {
				if (anim_step < 0)
					destroyed = true;
				else
					pic = pbk_bb.anim("explosion_c")[anim_step];
			}
		} else {
			anim_step += 1;

			if (type == 0) {
				if (anim_step >= 5)
					finished = true;
				else
					pic = pbk_bb.anim("explosion_a")[anim_step];
			} else if (type == 1) {
				if (anim_step >= 10)
					finished = true;
				else
					pic = pbk_bb.anim("explosion_b")[anim_step];
			} else {
				if (anim_step >= 5)
					finished = true;
				else
					pic = pbk_bb.anim("explosion_c")[anim_step];
			}
		}
	}

	moveAlongAngle();

	delay += 1;
	delay %= 17;

	// Flames rise whatever the aim
	if (delay == 0 && vy() > -3) accelerate(0, -1);

	colFromPic();
	updateDestroyed();
}
