#include "entities/enemies/enemy_ember.h"

EnemyEmber::EnemyEmber() { hp = 100; }

void EnemyEmber::update() {
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

void EnemyEmber::onWalk() {
	fall();

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (x - 1 < xmin || solid_wall(x - 1, y))
		dir = FACING_RIGHT;
	else if (x + 1 > offset + 640 || solid_wall(x + 1, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(2);
		pic = pbk_enemies[44 + anim_step];
	} else {
		walk(-2);
		pic = pbk_enemies[46 + anim_step];
	}

	colFromPic();
}

void EnemyEmber::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0 && anim_step < 14) anim_step += 1;

	if (anim_step >= 14) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[62 + anim_step];
		else
			pic = pbk_enemies[48 + anim_step];
	}
}

void EnemyEmber::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 10, 10, 0, -10, -10, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);
}
