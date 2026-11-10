#include "level/backgrounds/background_monolith.h"

#include "entities/ball.h"
#include "entities/enemies/enemy.h"

const int monolith_levitation[] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0};
const int monolith_animation[] = {0, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 3, 3, 3, 2, 2, 1, 0, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5};

BackgroundMonolith::BackgroundMonolith() : levitation_tick(0), levitation_step(0) {
	pic = assets().pics.level.anim("cruiser_hit_bottom")[1];
	dy = 0;
}

void BackgroundMonolith::update() {
	pic = assets().pics.level.anim("monolith_float")[animate(monolith_animation, 36, 3)];
	levitation_tick++;
	levitation_tick %= 4;
	if (levitation_tick == 0) {
		levitation_step++;
		if (levitation_step == 44) {
			levitation_step = 0;
		}
	}

	int xtmp;

	for (Ball* hero : session().players) {
		xtmp = hero->x;

		if (xtmp >= x - 138 && xtmp < x + pic->xSize() - 118 && level().terrain.platform_y(xtmp, hero->y) == y - 20) {
			if (levitation_step <= 22) {
				hero->y += monolith_levitation[levitation_step];
			} else {
				hero->y -= monolith_levitation[levitation_step - 22];
			}
		}
	}

	for (auto& enemies : level().entities.enemies) {
		xtmp = enemies->x;

		if (xtmp >= x - 138 && xtmp < x + pic->xSize() - 118 && level().terrain.platform_y(xtmp, enemies->y) == y - 20) {
			if (levitation_step <= 22) {
				enemies->y += monolith_levitation[levitation_step];
			} else {
				enemies->y -= monolith_levitation[levitation_step - 22];
			}
		}
	}

	if (levitation_step <= 22) {
		y += monolith_levitation[levitation_step];
	} else {
		y -= monolith_levitation[levitation_step - 22];
	}

	for (int i = x - 118; i < x + pic->xSize() - 138; i++) level().terrain.set_platform(4, i, y - 20);

	colFromPic();
}
