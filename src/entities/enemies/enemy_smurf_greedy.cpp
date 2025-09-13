#include "entities/enemies/enemy_smurf_greedy.h"
#include "entities/shots/shot_cake.h"

#define STATE_HOLD 15

EnemySmurfGreedy::EnemySmurfGreedy() : wait_for_shoot(0) {
	hp = 150;
	shoot_delay = 50 + rand() % 100;
}

void EnemySmurfGreedy::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
			onNormal();
			break;

		case STATE_SHOOT:
			onShoot();
			break;

		case STATE_HOLD:
			onHold();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;
	}

	if (x < offset - 20) destroyed = true;

	updateDestroyed();
}

void EnemySmurfGreedy::onNormal() {
	fall();

	wait_for_shoot += 1;

	if (wait_for_shoot > shoot_delay && x > offset && x < offset + 640 && targeted_player != NULL) {
		sbk_level.play(14 + rand() % 2);
		status = STATE_SHOOT;
		wait_for_shoot = 0;
		shoot_delay = 100 + rand() % 250;
		anim_step = 0;
		anim_tick = 0;
		xtarget = targeted_player->x;
		ytarget = targeted_player->y;

		if (xtarget > x)
			dir = FACING_RIGHT;
		else
			dir = FACING_LEFT;

		onShoot();
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[182];
	} else {
		pic = pbk_enemies[188];
	}

	colFromPic();
}

void EnemySmurfGreedy::onShoot() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 5) {
		ShotCake* shot = new ShotCake();

		shot->setDir(dir);

		if (dir == FACING_LEFT)
			shot->x = x - 13;
		else
			shot->x = x + 13;

		shot->y = y - 28;

		lobShot(shot->x, shot->y, xtarget, ytarget, shot->dx, shot->dy);

		list_enemy_shots.emplace_back(shot);

		anim_step = 0;
		status = STATE_HOLD;
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[182 + anim_step];
	} else {
		pic = pbk_enemies[188 + anim_step];
	}

	colFromPic();
}

void EnemySmurfGreedy::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 11) anim_step += 1;

	if (dir == FACING_RIGHT) {
		if (!solid_wall(x - 2, y)) x -= 2;

		pic = pbk_enemies[195 + anim_step];
	} else {
		if (!solid_wall(x + 2, y)) x += 2;

		pic = pbk_enemies[207 + anim_step];
	}

	if (anim_step >= 11 && platform_y(x, y) != 0) {
		destroyed = true;
		engrave(x, y, pic);
	}
}

void EnemySmurfGreedy::takeHit(Shot* shot) {
	static const int dx_splatter_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_smurf, dy_splatter_smurf);

	if (shot->getSplatterDir() < 4)
		dir = FACING_LEFT;
	else
		dir = FACING_RIGHT;

	if (status == STATE_DYING) sbk_level.play(rand() % 4);
}

void EnemySmurfGreedy::onHold() {
	anim_step += 1;

	if (anim_step >= 50) {
		anim_step = 0;
		status = STATE_NORMAL;
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[187];
	} else {
		pic = pbk_enemies[193];
	}

	colFromPic();
}

void EnemySmurfGreedy::onCharred() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 6) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[34 + anim_step];
		else
			pic = pbk_enemies[40 + anim_step];
	}
}
