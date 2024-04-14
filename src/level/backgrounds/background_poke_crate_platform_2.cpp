#include "level/backgrounds/background_poke_crate_platform_2.h"
#include "core/math_utils.h"

BackgroundPokeCratePlatform2::BackgroundPokeCratePlatform2() { pic = pbk_niveau[87]; }

void BackgroundPokeCratePlatform2::update() {
	if (y < 20) {  //-200
		y += 3;
	} else {
		etape += 2;
		etape %= 360;

		y = 80 - cosi(60, etape);

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
