#include "level/backgrounds/background_rock_1.h"

#include "entities/ball.h"

const int monolite_levitation[] = {0, 0, 1, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0};
const int monolite_animation[] = {50, 51, 51, 52, 52, 52, 53, 53, 53, 53, 53, 52, 52, 52, 51, 51,
								  50, 54, 54, 55, 55, 55, 56, 56, 56, 56, 56, 55, 55, 55, 54, 54};

BackgroundRock1::BackgroundRock1() : etape_levitation(0), ss_etape_levitation(0) {
	pic = pbk_level[31];
	dy = 0;
}

void BackgroundRock1::update() {
	pic = pbk_level[animate(monolite_animation, 32, 3)];

	ss_etape_levitation++;
	ss_etape_levitation %= 4;
	if (ss_etape_levitation == 0) {
		etape_levitation++;
		if (etape_levitation == 26) {
			etape_levitation = 0;
		}
	}

	int xtmp;

	for (Ball* hero : list_players) {
		xtmp = hero->x;

		if (xtmp >= x - 45 && xtmp < x + pic->xSize() - 45 && platform_y(xtmp, hero->y) == y - 30) {
			if (etape_levitation <= 13) {
				hero->y += monolite_levitation[etape_levitation];
			} else {
				hero->y -= monolite_levitation[etape_levitation - 13];
			}
		}
	}

	if (etape_levitation <= 13) {
		y += monolite_levitation[etape_levitation];
	} else {
		y -= monolite_levitation[etape_levitation - 13];
	}

	for (int i = x - 45; i < x + pic->xSize() - 45; i++) set_plat(4, i, y - 30);

	colFromPic();
}
