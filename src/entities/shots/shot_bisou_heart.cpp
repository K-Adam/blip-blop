#include "entities/shots/shot_bisou_heart.h"
#include <cmath>
#include <numbers>

ShotBisouHeart::ShotBisouHeart() : ok(false) { phi = rand() % 360; }

void ShotBisouHeart::update() {
	y += 1;

	phi += 5;
	phi %= 360;

	x = xbase + int(10 * std::cos(phi * std::numbers::pi / 180));

	if (!ok) {
		anim_tick += 1;
		anim_tick %= 6;

		if (anim_tick == 0) {
			anim_step += 1;

			if (anim_step >= 2) ok = true;
		}

		pic = assets().pics.enemies.anim("heart_shot")[anim_step];
	}

	if (level().terrain.solid_wall(x, y)) destroyed = true;

	colFromPic();
	updateDestroyed();
}
