#include "level/backgrounds/background_poke_crate_platform.h"

BackgroundPokeCratePlatform::BackgroundPokeCratePlatform(int target_y) : y_target(target_y) { pic = assets().pics.level.pic("crate_hoist"); }

void BackgroundPokeCratePlatform::update() {
	if (y < y_target - 50) {
		y += 5;
	} else if (y < y_target - 40) {
		y += 4;
	} else if (y < y_target - 30) {
		y += 3;
	} else if (y < y_target - 20) {
		y += 2;
	} else if (y < y_target - 10) {
		y += 1;
	}

	int i;

	if (y + 28 != 0) {
		for (i = x - 90; i < x + 140; i++) {
			level().terrain.set_platform(4, i, y + 28);
		}
	} else {
		for (i = x - 90; i < x + 140; i++) {
			level().terrain.set_platform(4, i, y + 29);
		}
	}
};
