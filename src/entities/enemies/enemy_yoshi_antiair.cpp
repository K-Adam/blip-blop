#include "entities/enemies/enemy_yoshi_antiair.h"
#include "entities/shots/shot_shell.h"

EnemyYoshiAntiair::EnemyYoshiAntiair() : attack_delay(50 + rand() % 150), wait_for_attack(0) {
	treasure = 12;
	hp = 400;
}

void EnemyYoshiAntiair::update() {
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

		case STATE_SHOOT:  // ETAT_TIRE is throwing shells
			onShoot();
			break;
	}

	updateDestroyed();
}

void EnemyYoshiAntiair::onWalk() {
	fall();
	wait_for_attack++;
	if (wait_for_attack >= attack_delay && x <= offset + 640) {
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 150;
		if (targeted_player->x > x) {
			dir = FACING_RIGHT;
		} else {
			dir = FACING_LEFT;
		}
		anim_step = 0;
		status = STATE_SHOOT;
		sbk_level.play(35);
		onShoot();
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.anim("yoshi_walk_right")[0];
	} else {
		pic = pbk_enemies.anim("yoshi_walk_left")[0];
	}

	colFromPic();
}

void EnemyYoshiAntiair::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 9) anim_step += 1;

	if (anim_step >= 9) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("yoshi_die_left")[anim_step];
		else
			pic = pbk_enemies.anim("yoshi_die_right")[anim_step];
	}
}

void EnemyYoshiAntiair::onShoot() {
	anim_tick += 1;
	anim_tick %= 15;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 2) {
		ShotShell* shot = new ShotShell();

		shot->setDir(dir);

		if (dir == FACING_LEFT)
			shot->x = x - 23;
		else
			shot->x = x + 23;

		shot->y = y - 23;

		list_enemy_shots.emplace_back(shot);

		anim_step = 0;
		status = STATE_WALK;
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.anim("yoshi_throw_right")[anim_step];
	} else {
		pic = pbk_enemies.anim("yoshi_throw_left")[anim_step];
	}

	colFromPic();
}

void EnemyYoshiAntiair::onCharred() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step++;

	if (anim_step >= 14)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("yoshi_burn_right")[anim_step];
		else
			pic = pbk_enemies.anim("yoshi_burn_left")[anim_step];
	}
}

void EnemyYoshiAntiair::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		sbk_misc.play(6 + rand() % 3);
	}
}
