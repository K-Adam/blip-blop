#include <cstdlib>
#include "level/backgrounds/background_light.h"

#define STATE_BLINK 100

BackgroundLight::BackgroundLight() : lit(false), wait_for_blink(0) {
	blink_interval = 50 + rand() % 200;
	pic = pbk_level[0];
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
		pic = pbk_level[animate(anim_blink, 58, 3)];

		if (anim_step == 57) {
			lit = true;
			wait_for_blink = 0;
			blink_interval = 50 + rand() % 200;
		}
	}

	if (x < offset - 300) destroyed = true;
}
