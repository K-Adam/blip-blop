#include "entities/shots/shot_bb_cow.h"
#include "entities/gore_splatter.h"

void ShotBBCow::update() {
	y += dy;

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (solid_wall(x, y) && y > 50) {
		pic = NULL;
		if (anim_step < 4)
			engrave(x, y, pbk_bb[211]);
		else
			engrave(x, y, pbk_bb[210]);
		col_on = false;
		destroyed = true;

		GoreSplatter* s;

		for (int i = 0; i < 15; i++) {
			s = new GoreSplatter(rand() % 11 - 5, -2 - rand() % 6);
			s->x = x + rand() % 11 - 5;
			s->y = y - rand() % 30 - 10;
			list_gore.emplace_back(s);
		}

		sbk_misc.play(11);
	} else if (y > 550 || x < offset - 50) {
		destroyed = true;
	} else {
		pic = pbk_bb[202 + anim_step];
		colFromPic();
	}
}
