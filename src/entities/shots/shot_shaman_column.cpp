#include "entities/shots/shot_shaman_column.h"

ShotShamanColumn::ShotShamanColumn() : status(0) { lifespan = 250 + rand() % 100; }

void ShotShamanColumn::update() {
	travel();

	anim_tick += 1;
	anim_tick %= 6;

	switch (status) {
		case 0:

			if (anim_tick == 0) {
				anim_step += 1;

				if (anim_step > 4) {
					status = 1;
					return;
				}
			}

			pic = pbk_enemies.anim("shaman_column_grow")[4 - anim_step];

			break;

		case 1:

			if (anim_tick == 0) {
				anim_step += 1;
				anim_step %= 6;
			}

			lifespan -= 1;

			if (lifespan <= 0) {
				status = 2;
				anim_step = anim_tick = 0;
				return;
			}

			pic = pbk_enemies.anim("shaman_column_burn")[anim_step];

			break;

		case 2:

			if (anim_tick == 0) {
				anim_step += 1;

				if (anim_step > 4) {
					destroyed = true;
					return;
				}
			}

			pic = pbk_enemies.anim("shaman_column_grow")[anim_step];

			break;
	}

	colFromPic();
	updateDestroyed();
}

void ShotShamanColumn::travel() {
	static const int SPEED = 4;

	fall();

	if (solid_wall(x - SPEED, y) || x + SPEED < offset)
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640)
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(SPEED);
	} else {
		walk(-SPEED);
	}
}
