#include "level/backgrounds/background_poke_crate_platform_2.h"
#include <cmath>
#include <numbers>

BackgroundPokeCratePlatform2::BackgroundPokeCratePlatform2() { pic = pbk_niveau[87]; }

void BackgroundPokeCratePlatform2::update() {
	if (y < 20) {
		y += 3;
	} else {
		etape += 2;
		etape %= 360;

		y = 80 - int(60 * std::cos(etape * std::numbers::pi / 180));

		if (y + 28 != 0) {
			for (int i = x - 90; i < x + 140; i++) {
				set_plat(3, i, y + 28);
			}
		} else {
			for (int i = x - 90; i < x + 140; i++) {
				set_plat(3, i, y + 29);
			}
		}
	}
};
