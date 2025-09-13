#include "level/backgrounds/background_poke_level_3.h"
#include "core/debug.h"

const int anim_blinking[] = {2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2,
							 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0};

BackgroundPokeLevel3::BackgroundPokeLevel3() : anim_step_E1(0), anim_step_V(0), anim_step_E2(0), anim_step_L2(0) { pic = pbk_level[142]; }

void BackgroundPokeLevel3::update() {
	anim_step++;
	anim_step %= 460;

	anim_step_E1++;
	anim_step_E1 %= 120;

	anim_step_V++;
	anim_step_V %= 180;

	anim_step_E2++;
	anim_step_E2 %= 290;

	anim_step_L2++;
	anim_step_L2 %= 190;

	if (anim_step < 57) {
		pic = pbk_level[PBK_3 + anim_blinking[anim_step]];
	}
}

void BackgroundPokeLevel3::render() {
	Sprite::render();

	if (anim_step_E1 < 57) {
		draw(x + 12, y - 19, pbk_level[PBK_E + anim_blinking[anim_step_E1]]);
	}
	if (anim_step_V < 57) {
		draw(x + 23, y - 19, pbk_level[PBK_V + anim_blinking[anim_step_V]]);
	}
	if (anim_step_E2 < 57) {
		draw(x + 36, y - 19, pbk_level[PBK_E + anim_blinking[anim_step_E2]]);
	}
	if (anim_step_L2 < 57) {
		draw(x + 47, y - 19, pbk_level[PBK_L + anim_blinking[anim_step_L2]]);
	}
}
