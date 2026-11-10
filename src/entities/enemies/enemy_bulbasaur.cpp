#include "entities/enemies/enemy_bulbasaur.h"

EnemyBulbasaur::EnemyBulbasaur() { hp = 100; }

void EnemyBulbasaur::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;
	}

	updateDestroyed();
}

void EnemyBulbasaur::onWalk() {
	fall();

	step(8, 2);

	if (x - 1 < xmin || solid_wall(x - 1, y))
		dir = FACING_RIGHT;
	else if (x + 1 > offset + 640 || solid_wall(x + 1, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(1);
		pic = pbk_enemies.anim("bulbasaur_walk_right")[anim_step];
	} else {
		walk(-1);
		pic = pbk_enemies.anim("bulbasaur_walk_left")[anim_step];
	}

	colFromPic();
}

void EnemyBulbasaur::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0 && anim_step < 8) anim_step += 1;

	if (anim_step >= 8) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("bulbasaur_die_left")[anim_step];
		else
			pic = pbk_enemies.anim("bulbasaur_die_right")[anim_step];
	}
}

void EnemyBulbasaur::onCharred() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 14)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("bulbasaur_burn_right")[anim_step];
		else
			pic = pbk_enemies.anim("bulbasaur_burn_left")[anim_step];
	}
}

void EnemyBulbasaur::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		sbk_misc.play(8 + rand() % 2);
	}
}
