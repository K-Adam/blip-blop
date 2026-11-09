#include "entities/enemies/enemy_snork_in_black.h"
#include "entities/shots/shot_thorn.h"

const int anim_snork_in_black_walk_right[] = {0, 1, 2, 1};
const int anim_snork_in_black_walk_left[] = {0, 1, 2, 1};

EnemySnorkInBlack::EnemySnorkInBlack() : nb_shots(0), wait_for_shoot(0), shoot_delay(50 + rand() % 100) {
	hp = 250;
	dy = 0;
}

void EnemySnorkInBlack::update() {
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

void EnemySnorkInBlack::onWalk() {
	if (platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onFall();
		return;
	}

	wait_for_shoot++;
	if ((targeted_player != NULL) && (wait_for_shoot > shoot_delay) && ((dir == FACING_RIGHT && x > offset + 80) || (dir == FACING_LEFT && x < offset + 560)))

	{
		x_target = targeted_player->x;
		y_target = targeted_player->y;

		if (x_target != x) {
			int vy = ((y_target - y + 14) * 6) / (x_target - x);

			if ((dir == FACING_RIGHT) && (x < x_target) && (vy < 2) && (vy > -2)) {
				wait_for_shoot = 0;
				shoot_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				nb_shots = rand() % 3;
				status = STATE_SHOOT;
				onShoot();
				return;
			} else if ((dir == FACING_LEFT) && (x > x_target) && (vy < 2) && (vy > -2)) {
				wait_for_shoot = 0;
				shoot_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				nb_shots = rand() % 3;
				status = STATE_SHOOT;
				onShoot();
				return;
			}
		}
	}

	if (dir == FACING_LEFT) {
		walk(-SNORK_IN_BLACK_SPEED);

		pic = pbk_enemies.anim("snork_in_black_walk_left")[animate(anim_snork_in_black_walk_left, 4, 4)];

		if (solid_wall(x - SNORK_IN_BLACK_SPEED, y) || (x - SNORK_IN_BLACK_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		walk(SNORK_IN_BLACK_SPEED);

		pic = pbk_enemies.anim("snork_in_black_walk_right")[animate(anim_snork_in_black_walk_right, 4, 4)];

		if (solid_wall(x + SNORK_IN_BLACK_SPEED, y) || (x + SNORK_IN_BLACK_SPEED > offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkInBlack::onFall() {
	fall();

	if (platform_y(x, y) != 0) status = STATE_WALK;

	colFromPic();
}

void EnemySnorkInBlack::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 5) {
		if (platform_y(x, y) != 0) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies.anim("snork_in_black_die_right")[anim_step];
		} else {
			pic = pbk_enemies.anim("snork_in_black_die_left")[anim_step];
		}
	}
}

void EnemySnorkInBlack::onShoot() {
	anim_tick++;
	if (anim_step == 0) {
		anim_tick %= 20;
	} else {
		anim_tick %= 4;
	}

	if (anim_tick == 0) {
		anim_step++;
	}

	if ((anim_step == 1) && (anim_tick == 0)) {
		if (dir == FACING_RIGHT) {
			int vx = 5 + rand() % 2;
			int vy = rand() % 5 - 2;

			ShotThorn* shot = new ShotThorn(pbk_enemies.anim("snork_in_black_thorn")[0], vx, vy);

			shot->setDir(dir);
			shot->x = x + 49;
			shot->y = y - 15;

			list_enemy_shots.emplace_back(shot);
		} else {
			int vx = -5 - rand() % 2;
			int vy = rand() % 5 - 2;

			ShotThorn* shot = new ShotThorn(pbk_enemies.anim("snork_in_black_thorn")[1], vx, vy);

			shot->setDir(dir);
			shot->x = x - 49;
			shot->y = y - 15;

			list_enemy_shots.emplace_back(shot);
		}

		sbk_level.play(24);
	}

	if (anim_step == 3) {
		nb_shots++;
		anim_step = 0;
		if (nb_shots >= SNORK_IN_BLACK_MAX_SHOTS) {
			anim_tick = 0;
			anim_step = 0;
			status = STATE_WALK;
			onWalk();
			return;
		}
	}

	if (dir == FACING_RIGHT) {
		if (anim_step == 0) {
			pic = pbk_enemies.anim("snork_in_black_walk_right")[0];
		} else {
			pic = pbk_enemies.anim("snork_in_black_shoot_right")[anim_step - 1];
		}
	} else {
		if (anim_step == 0) {
			pic = pbk_enemies.anim("snork_in_black_walk_left")[0];
		} else {
			pic = pbk_enemies.anim("snork_in_black_shoot_left")[anim_step - 1];
		}
	}
	colFromPic();
}

void EnemySnorkInBlack::onCharred() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 7) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("snork_in_black_burn_left")[anim_step];
		else
			pic = pbk_enemies.anim("snork_in_black_burn_right")[anim_step];
	}
}

void EnemySnorkInBlack::takeHit(Shot* shot) {
	static const int dx_splatter_snorkbase1[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_snorkbase1[] = {-15, -15, -15, -15, -25, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_snorkbase1, dy_splatter_snorkbase1);

	if (status == STATE_DYING) {
		sbk_level.play(21 + rand() % 3);
	}
}
