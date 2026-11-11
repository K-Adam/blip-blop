#include "entities/sprite.h"
#include "level/backgrounds/background_poke_crate.h"

BackgroundPokeCrate::BackgroundPokeCrate() { pic = assets().pics.level.pic("crate"); }

void BackgroundPokeCrate::update() {
	int yp;

	fall();

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
		y = yp;
		engrave(x, y - 10, pic);

		int i;

		for (i = x - 90; i < x + 140; i++) {
			level().terrain.set_platform(4, i, y - 125);
		}

		colFromPic();

		destroyed = true;
	}
};
