#include "level/backgrounds/background_poke_elevator.h"

#include "entities/ball.h"

BackgroundPokeElevator::BackgroundPokeElevator(int active_after, bool hidden) : active_after_(active_after), hidden_(hidden) {
	pic = assets().pics.level.pic("elevator");
	dy = 1;
}

void BackgroundPokeElevator::update() {
	if (level().rules.flags[2] > active_after_) {
		int xtmp;

		if (y <= 96)
			dy = 1;
		else if (y >= 387)
			dy = -1;

		for (Ball* hero : session().players) {
			xtmp = hero->x;

			if (xtmp > x && xtmp < x + pic->xSize() && level().terrain.platform_y(xtmp, hero->y) == y + 20) hero->y += dy;
		}

		y += dy;

		for (int i = x; i < x + pic->xSize(); i++) level().terrain.set_platform(4, i, y + 20);

		colFromPic();

		if (x < level().camera.offset - 400) destroyed = true;
	} else if (hidden_) {
		y = -60;
	}
}
