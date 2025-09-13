#include "entities/enemies/enemy_smurfette.h"
#include "entities/shots/shot_bisou_deadly.h"

EnemySmurfette::EnemySmurfette() : shoot_step(0) {
	wait_shoot = 100 + rand() % 100;
	hp = 200;
}

void EnemySmurfette::update() {
	if (blood > 0) blood -= 1;

	shoot_step += 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_SHOOT:
			onShoot();
			break;

		case STATE_CHARRED:
			onCharred();
			break;
	}

	if (x < offset - 20) destroyed = true;

	updateDestroyed();
}

void EnemySmurfette::onWalk() {
	static const int walk_right[] = {315, 316, 317, 318, 319, 318, 317, 316};
	static const int walk_left[] = {320, 321, 322, 323, 324, 323, 322, 321};
	static const int SPEED = 1;

	fall();

	if (x - SPEED < xmin || solid_wall(x - SPEED, y) || (platform_y(x - SPEED, y) == 0 && platform_y(x - SPEED, y + 5) == 0))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y) || (platform_y(x + SPEED, y) == 0 && platform_y(x + SPEED, y + 5) == 0))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(SPEED);
		pic = pbk_enemies[animate(walk_right, 8, 6)];
	} else {
		walk(-SPEED);
		pic = pbk_enemies[animate(walk_left, 8, 6)];
	}

	if (shoot_step >= wait_shoot && x > offset + 10 && x < offset + 630) {
		anim_step = anim_tick = 0;
		status = STATE_SHOOT;
		onShoot();
		return;
	}

	colFromPic();
}

void EnemySmurfette::onDie() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step >= 11) {
			destroyed = true;
			engrave(x, y, pic);
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[325 + anim_step];
	else
		pic = pbk_enemies[337 + anim_step];
}

void EnemySmurfette::onShoot() {
	static const int shoot_right[] = {301, 302, 303, 304, 305, 304, 303, 302, 301};
	static const int shoot_left[] = {306, 307, 308, 309, 310, 309, 308, 307, 306};

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step == 4 && anim_tick == 0 && targeted_player != NULL) {
			ShotBisouDeadly* t = new ShotBisouDeadly();

			t->x = x;
			t->y = y - 25;
			t->setDir(dir);
			t->target = targeted_player;

			list_enemy_shots.emplace_back(t);
		} else if (anim_step >= 9) {
			status = STATE_WALK;
			anim_step = anim_tick = shoot_step = 0;
			wait_shoot = 200 + rand() % 100;
			onWalk();
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[shoot_right[anim_step]];
	else
		pic = pbk_enemies[shoot_left[anim_step]];

	colFromPic();
}

void EnemySmurfette::takeHit(Shot* shot) {
	static const int dx_splatter_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_smurf, dy_splatter_smurf);

	if (status == STATE_DYING) {
		sbk_level.play(21);
	}
}

void EnemySmurfette::onCharred() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 6) {
		destroyed = true;
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[34 + anim_step];
		else
			pic = pbk_enemies[40 + anim_step];
	}
}
