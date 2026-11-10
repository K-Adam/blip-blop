#include "entities/explosion.h"

Explosion::Explosion() : finished(false) {
	type = rand() % 2;

	if (type == 1)
		speed = 2 + rand() % 2;
	else
		speed = 4 + rand() % 3;
}

void Explosion::update() {
	anim_tick += 1;
	anim_tick %= speed;

	if (anim_tick == 0) {
		if (finished) {
			anim_step -= 1;

			if (type == 0) {
				if (anim_step < 0)
					destroyed = true;
				else
					pic = assets().pics.bb.anim("explosion_a")[anim_step];
			} else if (type == 1) {
				if (anim_step < 0)
					destroyed = true;
				else
					pic = assets().pics.bb.anim("explosion_b")[anim_step];
			} else {
				if (anim_step < 0)
					destroyed = true;
				else
					pic = assets().pics.bb.anim("explosion_c")[anim_step];
			}
		} else {
			anim_step += 1;

			if (type == 0) {
				if (anim_step >= 5)
					finished = true;
				else
					pic = assets().pics.bb.anim("explosion_a")[anim_step];
			} else if (type == 1) {
				if (anim_step >= 10)
					finished = true;
				else
					pic = assets().pics.bb.anim("explosion_b")[anim_step];
			} else {
				if (anim_step >= 5)
					finished = true;
				else
					pic = assets().pics.bb.anim("explosion_c")[anim_step];
			}
		}
	}
}
