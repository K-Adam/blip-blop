#include "entities/enemies/enemy_com.h"

EnemyCOM::EnemyCOM() { hp = 100; }

void EnemyCOM::update() {
	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
		case STATE_CHARRED:
			onDie();
			break;
	}

	updateDestroyed();
}

void EnemyCOM::onWalk() {
	step(8, 2);

	if (x - 1 < level().camera.offset || level().terrain.solid_wall(x - 1, y))
		dir = FACING_RIGHT;
	else if (x + 1 > level().camera.offset + 640 || level().terrain.solid_wall(x + 1, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT)
		x += 1;
	else
		x -= 1;

	pic = assets().pics.enemies.anim("com_fly")[anim_step];

	colFromPic();
}

void EnemyCOM::onDie() {
	if (anim_tick == 0 && anim_step == 0) {
		assets().sounds.level.play(6);
	}

	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0 && anim_step < 22) anim_step += 1;

	if (anim_step >= 22) {
		destroyed = true;
	} else {
		pic = assets().pics.enemies.anim("com_die")[anim_step];
	}
}
