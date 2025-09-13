#include "level/backgrounds/background_poke_level_2.h"
#include "core/debug.h"

const int anim_clignotement[] = {2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2,
								 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0};

BackgroundPokeLevel2::BackgroundPokeLevel2() : etape_L1(0), etape_E1(0), etape_V(0), etape_E2(0), etape_L2(0) { pic = pbk_level[130]; }

void BackgroundPokeLevel2::update() {
	anim_step++;
	anim_step %= 560;

	etape_L1++;
	etape_L1 %= 155;

	etape_E1++;
	etape_E1 %= 300;

	etape_V++;
	etape_V %= 210;

	etape_E2++;
	etape_E2 %= 190;

	etape_L2++;
	etape_L2 %= 380;

	if (anim_step < 57) {
		pic = pbk_level[PBK_2 + anim_clignotement[anim_step]];
	}
}

void BackgroundPokeLevel2::render() {
	Sprite::render();

	if (etape_L1 < 57) {
		draw(x, y - 19, pbk_level[PBK_L + anim_clignotement[etape_L1]]);
	}
	if (etape_E1 < 57) {
		draw(x + 12, y - 19, pbk_level[PBK_E + anim_clignotement[etape_E1]]);
	}
	if (etape_V < 57) {
		draw(x + 23, y - 19, pbk_level[PBK_V + anim_clignotement[etape_V]]);
	}
	if (etape_E2 < 57) {
		draw(x + 36, y - 19, pbk_level[PBK_E + anim_clignotement[etape_E2]]);
	}
	if (etape_L2 < 57) {
		draw(x + 47, y - 19, pbk_level[PBK_L + anim_clignotement[etape_L2]]);
	}
}
