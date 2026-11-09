#include <cstdlib>
#include "level/backgrounds/background_light_2.h"

#define STATE_BLINK 100

BackgroundLight2::BackgroundLight2() : lit(false), wait_for_blink(0) {
	blink_interval = 50 + rand() % 200;
	pic = pbk_level.anim("light2_blink")[0];
}

void BackgroundLight2::update() {
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
		pic = pbk_level.anim("light2_blink")[animate(anim_blink, 58, 3)];

		if (anim_step == 57) {
			lit = true;
			wait_for_blink = 0;
			blink_interval = 50 + rand() % 200;
		}
	}

	if (x < offset - 300) destroyed = true;
}
