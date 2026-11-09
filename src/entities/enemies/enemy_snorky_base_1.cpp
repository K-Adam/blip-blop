#include "entities/enemies/enemy_snorky_base_1.h"
#include "entities/piece_snorky_head.h"
#include "entities/shots/shot_snorky_base.h"

const int anim_snorky_base_walk_right[] = {0, 1, 2, 3, 4, 3, 2, 1};
const int anim_snorky_base_walk_left[] = {0, 1, 2, 3, 4, 3, 2, 1};

EnemySnorkyBase1::EnemySnorkyBase1() : wait_for_shoot(0), shoot_delay(50 + rand() % 200) {
	hp = 200;
	pic = pbk_enemies.anim("snorky_walk_right")[0];
	dy = 0;
}

void EnemySnorkyBase1::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_WALK:
		case STATE_NORMAL:
			onWalk();
			break;

		case STATE_FALL:
		case STATE_JUMP:
			onFall();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;

		case STATE_SHOOT:
			onShoot();
			break;
	}

	updateDestroyed();
}

void EnemySnorkyBase1::onWalk() {
	if (platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onFall();
		return;
	}

	wait_for_shoot++;
	if ((wait_for_shoot > shoot_delay) && ((dir == FACING_RIGHT && x > offset + 40) || (dir == FACING_LEFT && x < offset + 600))) {
		wait_for_shoot = 0;
		shoot_delay = 50 + rand() % 200;
		anim_step = 0;
		anim_tick = 0;
		status = STATE_SHOOT;
		onShoot();
		return;
	}

	if (dir == FACING_LEFT) {
		walk(-SNORKY_BASE_SPEED);

		pic = pbk_enemies.anim("snorky_walk_left")[animate(anim_snorky_base_walk_left, 8, 4)];

		if (solid_wall(x - SNORKY_BASE_SPEED, y) || (x - SNORKY_BASE_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		walk(SNORKY_BASE_SPEED);

		pic = pbk_enemies.anim("snorky_walk_right")[animate(anim_snorky_base_walk_right, 8, 4)];

		if (solid_wall(x + SNORKY_BASE_SPEED, y) || (x + SNORKY_BASE_SPEED > offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkyBase1::onFall() {
	fall();

	if (platform_y(x, y) != 0) status = STATE_WALK;

	colFromPic();
}

void EnemySnorkyBase1::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4 && anim_tick == 1) {
		Sprite* s = new PieceSnorkyBaseHeadWhole();

		s->dir = dir;
		s->y = y - 22;
		s->x = x;

		list_splatters.emplace_back(s);

		dy = 0;
	}
	if (anim_step == 14) {
		if (platform_y(x, y) != 0) {
			engrave(x, y, pic);
		}

		destroyed = true;

	} else {
		if (dir == FACING_LEFT) {
			walk(-SNORKY_BASE_SPEED);
			pic = pbk_enemies.anim("snorky_die_left")[anim_step];
		} else {
			walk(SNORKY_BASE_SPEED);
			pic = pbk_enemies.anim("snorky_die_right")[anim_step];
		}
	}
}

void EnemySnorkyBase1::onShoot() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step == 12) {
		anim_step = 0;
		anim_tick = 0;
		status = STATE_WALK;
		onWalk();
		return;
	}

	if (anim_step == 10 && anim_tick == 0) {
		if (dir == FACING_RIGHT) {
			ShotSnorkyBase* shot = new ShotSnorkyBase(1);

			shot->setDir(dir);
			shot->x = x + 35;
			shot->y = y - 42;

			list_enemy_shots.emplace_back(shot);

		} else {
			ShotSnorkyBase* shot = new ShotSnorkyBase(-1);

			shot->setDir(dir);
			shot->x = x - 35;
			shot->y = y - 42;

			list_enemy_shots.emplace_back(shot);
		}

		sbk_level.play(17);
	}

	if (anim_step < 11) {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies.anim("snorky_shoot_right")[anim_step];
		} else {
			pic = pbk_enemies.anim("snorky_shoot_left")[anim_step];
		}
	}
}

void EnemySnorkyBase1::onCharred() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 9) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("snorky_burn_left")[anim_step];
		else
			pic = pbk_enemies.anim("snorky_burn_right")[anim_step];
	}
}

void EnemySnorkyBase1::takeHit(Shot* shot) {
	static const int dx_splatter_snorkbase1[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_snorkbase1[] = {-15, -15, -15, -15, -25, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_snorkbase1, dy_splatter_snorkbase1);

	if (status == STATE_DYING) {
		sbk_level.play(14 + rand() % 3);
	}
}
