#include "entities/sprite.h"
#include "level/backgrounds/background_poke_crate.h"

BackgroundPokeCrate::BackgroundPokeCrate() { pic = pbk_level[117]; }

void BackgroundPokeCrate::update() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		y = yp;
		engrave(x, y - 10, pic);

		int i;

		for (i = x - 90; i < x + 140; i++) {
			set_plat(4, i, y - 125);
		}

		colFromPic();

		destroyed = true;
	}
};
