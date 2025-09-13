#include "entities/enemies/enemy_smurf.h"

const int anim_smurf_walk_right[] = {0, 1, 2, 3, 4, 3, 2, 1};
const int anim_smurf_walk_left[] = {5, 6, 7, 8, 9, 8, 7, 6};

EnemySmurf::EnemySmurf() : wait_for_jump(0) {
	hp = 100;
	jump_delay = 50 + rand() % 250;
	dy_jump = -5;
	speed = 1;
}

void EnemySmurf::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case STATE_WALK:
		case STATE_NORMAL:
			onWalk();
			break;

		case STATE_FALL:
		case STATE_JUMP:
			onJump();
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

void EnemySmurf::onWalk() {
	if (platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onJump();
	}

	wait_for_jump += 1;

	if (wait_for_jump > jump_delay) {
		status = STATE_JUMP;
		dy = dy_jump;
		grav_tick = 0;	// Otherwise jump heights vary
		wait_for_jump = 0;
		jump_delay = 50 + rand() % 250;
		onJump();
	}

	if (x - speed < xmin || solid_wall(x - speed, y))
		dir = FACING_RIGHT;
	else if (x + speed > offset + 640 || solid_wall(x + speed, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = pbk_enemies[animate(anim_smurf_walk_right, 8, 5)];
	} else {
		walk(-speed);
		pic = pbk_enemies[animate(anim_smurf_walk_left, 8, 5)];
	}

	colFromPic();
}

void EnemySmurf::onJump() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		onWalk();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		pic = pbk_enemies[10];
	} else {
		x -= speed;
		pic = pbk_enemies[11];
	}

	colFromPic();
}

void EnemySmurf::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step > 10) {
		destroyed = true;
		return;
	}

	if (dir == FACING_LEFT) {
		if (!solid_wall(x - speed, y) && platform_y(x, y) == 0) x -= speed;

		pic = pbk_enemies[23 + anim_step];
	} else {
		if (!solid_wall(x + speed, y) && platform_y(x, y) == 0) x += speed;

		pic = pbk_enemies[12 + anim_step];
	}
}

void EnemySmurf::takeHit(Shot* shot) {
	static const int dx_splatter_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_smurf, dy_splatter_smurf);

	if (status == STATE_DYING) sbk_level.play(4 + rand() % 10);
}

void EnemySmurf::onCharred() {
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
