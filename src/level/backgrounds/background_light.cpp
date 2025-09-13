#include <cstdlib>
#include "level/backgrounds/background_light.h"

#define ETAT_CLIGNOTE 100

BackgroundLight::BackgroundLight() : allume(false), wait_for_blink(0) {
	delai_cligno = 50 + rand() % 200;
	pic = pbk_level[0];
}

void BackgroundLight::update() {
	static const int anim_cligno[] = {2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2,
									  1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0};

	wait_for_blink += 1;

	if (allume) {
		if (wait_for_blink >= delai_cligno) {
			allume = false;
			anim_tick = 0;
			anim_step = 0;
		}
	} else {
		pic = pbk_level[animate(anim_cligno, 58, 3)];

		if (anim_step == 57) {
			allume = true;
			wait_for_blink = 0;
			delai_cligno = 50 + rand() % 200;
		}
	}

	if (x < offset - 300) destroyed = true;
}
