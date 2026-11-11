#include "level/backgrounds/background_light.h"

BackgroundLight::BackgroundLight(const char* sheet) : lit(false), wait_for_blink(0), sheet_(sheet) {
	blink_interval = 50 + rnd() % 200;
	pic = assets().pics.level.anim(sheet_)[0];
}

void BackgroundLight::update() {
	static const int anim_blink[] = {2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2,
									 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0};

	wait_for_blink += 1;

	if (lit) {
		if (wait_for_blink >= blink_interval) {
			lit = false;
			anim_tick = 0;
			anim_step = 0;
		}
	} else {
		pic = assets().pics.level.anim(sheet_)[animate(anim_blink, 58, 3)];

		if (anim_step == 57) {
			lit = true;
			wait_for_blink = 0;
			blink_interval = 50 + rnd() % 200;
		}
	}

	if (x < level().camera.offset - 300) destroyed = true;
}
