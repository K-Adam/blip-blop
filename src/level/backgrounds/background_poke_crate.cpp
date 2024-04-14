#include "entities/sprite.h"
#include "level/backgrounds/background_poke_crate.h"

BackgroundPokeCrate::BackgroundPokeCrate() { pic = pbk_niveau[117]; }

void BackgroundPokeCrate::update() {
	int yp;

	tombe();

	// Et si la caisse touche le sol ?
	if (dy > 0 && (yp = plat(x, y + dy)) != 0) {
		y = yp;
		grave(x, y - 10, pic);

		int i;

		for (i = x - 90; i < x + 140; i++) {
			set_plat(4, i, y - 125);
		}

		colFromPic();

		a_detruire = true;
	}
};
