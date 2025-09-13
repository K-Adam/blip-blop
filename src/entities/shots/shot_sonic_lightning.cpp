#include "entities/shots/shot_sonic_lightning.h"

ShotSonicLightning::ShotSonicLightning() {
	anim_tick = 0;
	anim_step = 0;
}

void ShotSonicLightning::update() {
	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0) {
		anim_step += 1;
	}
	pic = pbk_enemies[56 + anim_tick];

	if (anim_step > 30) {
		colFromPic();
		if (anim_step > 36) {
			destroyed = true;
		}
	}
}

void ShotSonicLightning::render() {
	if (anim_step == 0 || anim_step > 30) {
		Sprite::render();
	}
}
