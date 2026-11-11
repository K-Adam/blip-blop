#include "entities/shots/shot_bb_cow.h"
#include "entities/gore_splatter.h"

void ShotBBCow::update() {
	y += dy;

	step(8, 8);

	if (level().terrain.solid_wall(x, y) && y > 50) {
		pic = NULL;
		if (anim_step < 4)
			engrave(x, y, assets().pics.bb.pic("cow_splat_left"));
		else
			engrave(x, y, assets().pics.bb.pic("cow_splat_right"));
		col_on = false;
		destroyed = true;

		GoreSplatter* s;

		for (int i = 0; i < 15; i++) {
			s = new GoreSplatter(rnd() % 11 - 5, -2 - rnd() % 6);
			s->x = x + rnd() % 11 - 5;
			s->y = y - rnd() % 30 - 10;
			level().entities.gore.emplace_back(s);
		}

		assets().sounds.misc.play(11);
	} else if (y > 550 || x < level().camera.offset - 50) {
		destroyed = true;
	} else {
		pic = assets().pics.bb.anim("cow_fall")[anim_step];
		colFromPic();
	}
}
