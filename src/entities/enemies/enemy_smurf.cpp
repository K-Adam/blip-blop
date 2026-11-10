#include "entities/enemies/enemy_smurf.h"

const int anim_smurf_walk_right[] = {0, 1, 2, 3, 4, 3, 2, 1};
const int anim_smurf_walk_left[] = {0, 1, 2, 3, 4, 3, 2, 1};

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
	if (level().terrain.platform_y(x, y) == 0) {
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

	if (x - speed < xmin || level().terrain.solid_wall(x - speed, y))
		dir = FACING_RIGHT;
	else if (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = assets().pics.enemies.anim("smurf_walk_right")[animate(anim_smurf_walk_right, 8, 5)];
	} else {
		walk(-speed);
		pic = assets().pics.enemies.anim("smurf_walk_left")[animate(anim_smurf_walk_left, 8, 5)];
	}

	colFromPic();
}

void EnemySmurf::onJump() {
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
		pic = assets().pics.enemies.pic("smurf_jump_right");
	} else {
		x -= speed;
		pic = assets().pics.enemies.pic("smurf_jump_left");
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
		if (!level().terrain.solid_wall(x - speed, y) && level().terrain.platform_y(x, y) == 0) x -= speed;

		pic = assets().pics.enemies.anim("smurf_die_left")[anim_step];
	} else {
		if (!level().terrain.solid_wall(x + speed, y) && level().terrain.platform_y(x, y) == 0) x += speed;

		pic = assets().pics.enemies.anim("smurf_die_right")[anim_step];
	}
}

void EnemySmurf::takeHit(Shot* shot) {
	static const int dx_splatter_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_smurf, dy_splatter_smurf);

	if (status == STATE_DYING) assets().sounds.level.play(4 + rand() % 10);
}

void EnemySmurf::onCharred() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 6) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = assets().pics.enemies.anim("smurf_burn_left")[anim_step];
		else
			pic = assets().pics.enemies.anim("smurf_burn_right")[anim_step];
	}
}
