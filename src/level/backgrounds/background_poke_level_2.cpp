#include "level/backgrounds/background_poke_level_2.h"
#include "core/debug.h"

const int anim_blinking[] = {2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2,
							 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0};

BackgroundPokeLevel2::BackgroundPokeLevel2() : anim_step_L1(0), anim_step_E1(0), anim_step_V(0), anim_step_E2(0), anim_step_L2(0) {
	pic = pbk_level.anim("sign_2")[0];
}

void BackgroundPokeLevel2::update() {
	anim_step++;
	anim_step %= 560;

	anim_step_L1++;
	anim_step_L1 %= 155;

	anim_step_E1++;
	anim_step_E1 %= 300;

	anim_step_V++;
	anim_step_V %= 210;

	anim_step_E2++;
	anim_step_E2 %= 190;

	anim_step_L2++;
	anim_step_L2 %= 380;

	if (anim_step < 57) {
		pic = pbk_level.anim("sign_2")[anim_blinking[anim_step]];
	}
}

void BackgroundPokeLevel2::render() {
	Sprite::render();

	if (anim_step_L1 < 57) {
		draw(x, y - 19, pbk_level.anim("sign_l")[anim_blinking[anim_step_L1]]);
	}
	if (anim_step_E1 < 57) {
		draw(x + 12, y - 19, pbk_level.anim("sign_e")[anim_blinking[anim_step_E1]]);
	}
	if (anim_step_V < 57) {
		draw(x + 23, y - 19, pbk_level.anim("sign_v")[anim_blinking[anim_step_V]]);
	}
	if (anim_step_E2 < 57) {
		draw(x + 36, y - 19, pbk_level.anim("sign_e")[anim_blinking[anim_step_E2]]);
	}
	if (anim_step_L2 < 57) {
		draw(x + 47, y - 19, pbk_level.anim("sign_l")[anim_blinking[anim_step_L2]]);
	}
}
