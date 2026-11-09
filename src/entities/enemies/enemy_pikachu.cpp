#include "entities/enemies/enemy_pikachu.h"

EnemyPikachu::EnemyPikachu() { hp = 100; }

void EnemyPikachu::update() {
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

void EnemyPikachu::onWalk() {
	static const int SPEED = 3;

	fall();

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (x - SPEED < xmin || solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(SPEED);
		pic = pbk_enemies.anim("pikachu_walk_right")[anim_step];
	} else {
		walk(-SPEED);
		pic = pbk_enemies.anim("pikachu_walk_left")[anim_step];
	}

	colFromPic();
}

void EnemyPikachu::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0 && anim_step < 10) anim_step += 1;

	if (anim_step >= 10) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("pikachu_die_left")[anim_step];
		else
			pic = pbk_enemies.anim("pikachu_die_right")[anim_step];
	}
}

void EnemyPikachu::onCharred() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 14)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("pikachu_burn_right")[anim_step];
		else
			pic = pbk_enemies.anim("pikachu_burn_left")[anim_step];
	}
}

void EnemyPikachu::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		sbk_misc.play(8 + rand() % 2);
	}
}
