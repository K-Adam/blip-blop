#include "entities/shots/shot_gift.h"

ShotGift::ShotGift() {
	pic = pbk_enemies.pic("smurf_jokey_gift");
	noCol();
}

void ShotGift::update() {
	if (!solid_wall(x, y)) fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step == 20) {
			sbk_misc.play(11);
		}
	}

	if (anim_step >= 20) {
		col_on = true;
		pic = pbk_enemies.anim("gift_explode")[anim_step - 20];
	}

	if (anim_step > 35) destroyed = true;

	colFromPic();

	if (anim_step >= 30) noCol();
}
