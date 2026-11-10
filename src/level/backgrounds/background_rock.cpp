#include "level/backgrounds/background_rock.h"

#include "entities/ball.h"

int BackgroundRock::bob() const {
	int half = int(spec_.rise.size()) - 1;
	return levitation_step <= half ? spec_.rise[levitation_step] : -spec_.rise[levitation_step - half];
}

void BackgroundRock::update() {
	pic = pbk_level.anim(spec_.sheet)[animate(spec_.frames.data(), int(spec_.frames.size()), spec_.frame_delay)];

	levitation_tick = (levitation_tick + 1) % spec_.rise_delay;
	if (levitation_tick == 0 && ++levitation_step == 2 * (int(spec_.rise.size()) - 1)) levitation_step = 0;

	for (Ball* hero : list_players) {
		int xtmp = hero->x;
		if (xtmp >= x - 45 && xtmp < x + pic->xSize() - 45 && platform_y(xtmp, hero->y) == y - spec_.top) hero->y += bob();
	}

	y += bob();

	for (int i = x - 45; i < x + pic->xSize() - 45; i++) set_platform(4, i, y - spec_.top);

	colFromPic();
}
