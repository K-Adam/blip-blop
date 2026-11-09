#include "entities/enemies/enemy_tails_flying.h"

EnemyTailsFlying::EnemyTailsFlying() : placed(0), speed(1), speed_tick(0), fly_delay(100 + rand() % 350), wait_for_fly(0), fly(false) {
	treasure = 8;
	hp = 150;
}

void EnemyTailsFlying::update() {
	if (!placed) {
		y = -20;
		status = STATE_SHOOT;
		attack = false;
		speed = 0;
		anim_step = 0;
		anim_tick = 0;
		placed = true;

		fly = true;
		if (targeted_player != NULL) {
			x_target = targeted_player->x;
			y_target = targeted_player->y;
		} else {
			x_target = x - 10;
			y_target = 240;
		}
		if (x_target > x) {
			dir = FACING_RIGHT;
			direction = 1;
		} else {
			dir = FACING_LEFT;
			direction = -1;
		}
	}
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

		case STATE_JUMP:
		case STATE_FALL:
			onFall();
			break;

		case STATE_SHOOT:  // ETAT_TIRE is Tails flying
			onFly();
			break;
	}
	updateDestroyed();
}

void EnemyTailsFlying::onWalk() {
	if (platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		fly = true;
		anim_step = 0;
		anim_tick = 0;
		onFall();
		return;
	}

	if (x - speed < xmin || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}
	wait_for_fly++;
	if (wait_for_fly >= fly_delay) {
		status = STATE_SHOOT;
		attack = false;
		speed = 0;
		anim_step = 0;
		anim_tick = 0;

		fly = true;
		x_target = targeted_player->x;
		y_target = targeted_player->y;
		if (x_target > x) {
			dir = FACING_RIGHT;
			direction = 1;
		} else {
			dir = FACING_LEFT;
			direction = -1;
		}
		onFly();
		return;
	}

	anim_tick += 1;
	anim_tick %= 10 - 2 * speed;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = pbk_enemies.anim("tails_walk_right")[anim_step];
	} else {
		walk(-speed);
		pic = pbk_enemies.anim("tails_walk_left")[anim_step];
	}
	if (speed < TAILS_SPEED) {
		speed_tick++;
		if (speed_tick >= 30) {
			speed_tick = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyTailsFlying::onDie() {
	fall();
	if (fly) {
		int yp;
		anim_tick += 1;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 2;
		}
		if (dir == FACING_RIGHT) {
			if (!solid_wall(x + speed, y)) walk(speed);
			pic = pbk_enemies.anim("tails_fly_die_right")[anim_step];
		} else {
			if (!solid_wall(x - speed, y)) walk(-speed);
			pic = pbk_enemies.anim("tails_fly_die_left")[anim_step];
		}
		if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
			dy = 0;
			y = yp;
			fly = false;
			anim_step = 7;
		}
	} else {
		anim_tick += 1;
		anim_tick %= 6;

		if (anim_tick == 0 && anim_step < 11) anim_step += 1;

		if (anim_step >= 11) {
			int yy = platform_y(x, y);

			if (yy != 0 && yy != y_platform[4][x]) {
				engrave(x, y, pic);
				destroyed = true;
			} else {
				if (dir == FACING_LEFT && !solid_wall(x - speed, y))
					walk(-speed);
				else if (!solid_wall(x + speed, y))
					walk(speed);
			}
		} else {
			if (dir == FACING_LEFT) {
				if (!solid_wall(x - speed, y)) walk(-speed);
				pic = pbk_enemies.anim("tails_die_left")[anim_step];
			} else {
				if (!solid_wall(x + speed, y)) walk(speed);
				pic = pbk_enemies.anim("tails_die_right")[anim_step];
			}
		}
	}
}

void EnemyTailsFlying::onFly() {
	if (dir == FACING_LEFT && (x - speed < xmin || solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
		status = STATE_WALK;
		wait_for_fly = 0;
		fly_delay = 100 + rand() % 350;
		fly = false;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	} else if (dir == FACING_RIGHT && (x + speed > offset + 640 || solid_wall(x + speed, y))) {
		dir = FACING_LEFT;
		speed = 1;
		status = STATE_WALK;
		wait_for_fly = 0;
		fly = false;
		fly_delay = 100 + rand() % 350;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}

	anim_step += 1;
	anim_step %= 2;

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.anim("tails_fly_right")[anim_step];
	} else {
		pic = pbk_enemies.anim("tails_fly_left")[anim_step];
	}

	{}

	if (attack) {
		if ((y_target <= y) || (y >= y_platform[0][x])) {
			speed = TAILS_FLY_SPEED;
		} else if ((y_target - 80 > y) && (y > y_target - 110)) {
			y += 4;
			speed = 1;
		} else if ((y_target - 60 > y) && (y >= y_target - 80)) {
			y += 3;
			speed = 2;
		} else if ((y_target - 40 > y) && (y >= y_target - 60)) {
			y += 2;
			speed = 3;
		} else if ((y_target - 20 > y) && (y >= y_target - 40)) {
			y += 1;
			speed = 4;
		} else {
			y += 1;
			speed = 5;
		}
		x += speed * direction;
	} else {
		if (y < (y_target - 110)) {
			y += 5;
		} else if (y > (y_target - 100)) {
			y -= 3;
		} else {
			attack = true;
		}
	}

	colFromPic();
}

void EnemyTailsFlying::onFall() {
	int yp;

	fall();
	anim_tick += 1;
	anim_tick %= 3;
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		fly = false;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		pic = pbk_enemies.anim("tails_fly_right")[anim_step];
	} else {
		x -= speed;
		pic = pbk_enemies.anim("tails_fly_left")[anim_step];
	}

	colFromPic();
}

void EnemyTailsFlying::onCharred() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step++;

	if (anim_step >= 11)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("tails_burn_right")[anim_step];
		else
			pic = pbk_enemies.anim("tails_burn_left")[anim_step];
	}
}

void EnemyTailsFlying::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		sbk_misc.play(10);
	}
}
