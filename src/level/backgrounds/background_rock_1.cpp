#include "level/backgrounds/background_rock_1.h"

#include "entities/ball.h"

const int monolith_levitation[] = {0, 0, 1, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0};
const int monolith_animation[] = {50, 51, 51, 52, 52, 52, 53, 53, 53, 53, 53, 52, 52, 52, 51, 51,
								  50, 54, 54, 55, 55, 55, 56, 56, 56, 56, 56, 55, 55, 55, 54, 54};

BackgroundRock1::BackgroundRock1() : levitation_step(0), levitation_tick(0) {
	pic = pbk_level[31];
	dy = 0;
}

void BackgroundRock1::update() {
	pic = pbk_level[animate(monolith_animation, 32, 3)];

	levitation_tick++;
	levitation_tick %= 4;
	if (levitation_tick == 0) {
		levitation_step++;
		if (levitation_step == 26) {
			levitation_step = 0;
		}
	}

	int xtmp;

	for (Ball* hero : list_players) {
		xtmp = hero->x;

		if (xtmp >= x - 45 && xtmp < x + pic->xSize() - 45 && platform_y(xtmp, hero->y) == y - 30) {
			if (levitation_step <= 13) {
				hero->y += monolith_levitation[levitation_step];
			} else {
				hero->y -= monolith_levitation[levitation_step - 13];
			}
		}
	}

	if (levitation_step <= 13) {
		y += monolith_levitation[levitation_step];
	} else {
		y -= monolith_levitation[levitation_step - 13];
	}

	for (int i = x - 45; i < x + pic->xSize() - 45; i++) set_platform(4, i, y - 30);

	colFromPic();
}
