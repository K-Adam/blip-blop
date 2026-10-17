#include "level/backgrounds/background_rock_2.h"

#include "entities/ball.h"

const int rock2_levitation[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
const int rock2_animation[] = {0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 0, 4, 4, 5, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 4, 4};

BackgroundRock2::BackgroundRock2() : levitation_step(0), levitation_tick(0) {
	pic = pbk_level.anim("star_twinkle")[2];
	dy = 0;
}

void BackgroundRock2::update() {
	pic = pbk_level.anim("rock2_float")[animate(rock2_animation, 32, 4)];

	levitation_tick++;
	levitation_tick %= 6;
	if (levitation_tick == 0) {
		levitation_step++;
		if (levitation_step == 18) {
			levitation_step = 0;
		}
	}

	int xtmp;

	for (Ball* hero : list_players) {
		xtmp = hero->x;

		if (xtmp >= x - 45 && xtmp < x + pic->xSize() - 45 && platform_y(xtmp, hero->y) == y - 25) {
			if (levitation_step <= 9) {
				hero->y += rock2_levitation[levitation_step];
			} else {
				hero->y -= rock2_levitation[levitation_step - 9];
			}
		}
	}

	if (levitation_step <= 9) {
		y += rock2_levitation[levitation_step];
	} else {
		y -= rock2_levitation[levitation_step - 9];
	}

	for (int i = x - 45; i < x + pic->xSize() - 45; i++) set_platform(4, i, y - 25);

	colFromPic();
}
