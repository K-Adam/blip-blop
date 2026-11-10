#include "entities/shots/shot_gift.h"

ShotGift::ShotGift() {
	pic = assets().pics.enemies.pic("smurf_jokey_gift");
	noCol();
}

void ShotGift::update() {
	if (!level().terrain.solid_wall(x, y)) fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step == 20) {
			assets().sounds.misc.play(11);
		}
	}

	if (anim_step >= 20) {
		col_on = true;
		pic = assets().pics.enemies.anim("gift_explode")[anim_step - 20];
	}

	if (anim_step > 35) destroyed = true;

	colFromPic();

	if (anim_step >= 30) noCol();
}
