#include "entities/enemies/enemy_knuckles.h"

EnemyKnuckles::EnemyKnuckles() : speed(1), speed_tick(0), charge_delay(100 + rnd() % 150), wait_for_charge(0) {
	treasure = 7;
	hp = 150;
}

void EnemyKnuckles::update() {
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
			onJump();
			break;

		case STATE_FALL:
			onFall();
			break;

		case STATE_SHOOT:  // ETAT_TIRE is Knuckles charging
			onCharge();
			break;
	}
	updateDestroyed();
}

void EnemyKnuckles::onWalk() {
	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onFall();
		return;
	}

	if (x - speed < xmin || level().terrain.solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}
	wait_for_charge++;
	if (wait_for_charge >= charge_delay && session().targeted_player != NULL) {
		if ((level().terrain.platform_y2(session().targeted_player->x, session().targeted_player->y) == level().terrain.platform_y2(x, y)) &&
			(((dir == FACING_RIGHT) && (x > (session().targeted_player->x - 200)) && (session().targeted_player->x > x)) ||
			 ((dir == FACING_LEFT) && (x < (session().targeted_player->x + 200)) && (session().targeted_player->x < x)))) {
			status = STATE_SHOOT;
			assets().sounds.level.play(6);
			speed = KNUCKLES_CHARGE_SPEED;
			anim_step = 0;
			anim_tick = 0;
			onCharge();
			return;
		} else if ((session().targeted_player->y < y) && (session().targeted_player->x > x - 30) && (session().targeted_player->x < x + 30)) {
			status = STATE_JUMP;
			speed = 0;
			anim_step = 0;
			anim_tick = 0;
			grav_tick = 0;	// Otherwise jump heights vary
			dy = 0;
			assets().sounds.level.play(7);
			onJump();
			return;
		} else {
			wait_for_charge = 0;
			charge_delay = 10 + rnd() % 10;
		}
	}

	anim_tick += 1;
	anim_tick %= 10 - 2 * speed;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = assets().pics.enemies.anim("knuckles_walk_right")[anim_step];
	} else {
		walk(-speed);
		pic = assets().pics.enemies.anim("knuckles_walk_left")[anim_step];
	}
	if (speed < KNUCKLES_SPEED) {
		speed_tick++;
		if (speed_tick >= 20) {
			speed_tick = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyKnuckles::onDie() {
	fall();
	{
		anim_tick += 1;
		anim_tick %= 6;

		if (anim_tick == 0 && anim_step < 10) anim_step += 1;

		if (anim_step >= 10) {
			int yy = level().terrain.platform_y(x, y);

			if (yy != 0) {
				engrave(x, y, pic);
				destroyed = true;
			} else {
				if (dir == FACING_LEFT && !level().terrain.solid_wall(x - speed, y))
					x -= speed;
				else if (!level().terrain.solid_wall(x + speed, y))
					x += speed;
			}
		} else {
			if (dir == FACING_LEFT) {
				if (!level().terrain.solid_wall(x - speed, y)) x -= speed;
				pic = assets().pics.enemies.anim("knuckles_die_left")[anim_step];
			} else {
				if (!level().terrain.solid_wall(x + speed, y)) x += speed;
				pic = assets().pics.enemies.anim("knuckles_die_right")[anim_step];
			}
		}
	}
}

void EnemyKnuckles::onJump() {
	anim_tick++;
	anim_tick %= 60;
	if ((anim_tick == 0) && (anim_step == 0)) {
		anim_step++;
		dy = -KNUCKLES_JUMP_SPEED;
	}

	fall();

	int yp;
	if ((dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) && anim_step > 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		speed = 1;
		wait_for_charge = 0;
		anim_step = 0;
		anim_tick = 0;
		charge_delay = 50 + rnd() % 150;
		onWalk();
		return;
	}
	if (dir == FACING_LEFT) {
		pic = assets().pics.enemies.anim("knuckles_jump_left")[anim_step];
	} else {
		pic = assets().pics.enemies.anim("knuckles_jump_right")[anim_step];
	}
	colFromPic();
}

void EnemyKnuckles::onCharge() {
	fall();
	anim_tick++;
	anim_tick %= 40;
	if (anim_tick % 10 == 0) {
		speed--;
	}
	if (anim_step == 2) {
		status = STATE_WALK;
		speed = 1;
		wait_for_charge = 0;
		charge_delay = 50 + rnd() % 150;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}
	if (anim_tick == 0) {
		anim_step++;
		speed = KNUCKLES_CHARGE_SPEED - 1;
	}

	if (dir == FACING_LEFT) {
		if (!level().terrain.solid_wall(x - speed, y)) {
			walk(-speed);
		} else {
			status = STATE_WALK;
			wait_for_charge = 0;
			charge_delay = 50 + rnd() % 150;
			anim_step = 0;
			anim_tick = 0;
			onWalk();
			return;
		}
		if (anim_step < 2) pic = assets().pics.enemies.anim("knuckles_charge_left")[anim_step];
	} else {
		if (!level().terrain.solid_wall(x + speed, y)) {
			walk(speed);
		} else {
			status = STATE_WALK;
			wait_for_charge = 0;
			charge_delay = 50 + rnd() % 150;
			anim_step = 0;
			anim_tick = 0;
			onWalk();
			return;
		}
		if (anim_step < 2) pic = assets().pics.enemies.anim("knuckles_charge_right")[anim_step];
	}
	colFromPic();
}

void EnemyKnuckles::onFall() {
	int yp;

	fall();

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;

		onWalk();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		pic = assets().pics.enemies.anim("knuckles_walk_right")[0];
	} else {
		x -= speed;
		pic = assets().pics.enemies.anim("knuckles_walk_left")[0];
	}

	colFromPic();
}

void EnemyKnuckles::onCharred() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step++;

	if (anim_step >= 10)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = assets().pics.enemies.anim("knuckles_burn_right")[anim_step];
		else
			pic = assets().pics.enemies.anim("knuckles_burn_left")[anim_step];
	}
}

void EnemyKnuckles::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		assets().sounds.misc.play(10);
	}
}

void EnemyKnuckles::fall() {
	grav_tick += 1;
	grav_tick %= GRAVITY_DELAY;

	if (grav_tick == 0 && dy < KNUCKLES_GRAVITY_MAX) dy += 1;

	if (dy < 0 && level().terrain.solid_wall(x, y + dy)) dy = KNUCKLES_GRAVITY_MAX;

	int ny = level().terrain.platform_y(x, y + dy);

	if (ny != 0 && dy > 0)
		y = ny;
	else
		y += dy;
}
