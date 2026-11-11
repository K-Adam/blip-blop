#include "entities/enemies/enemy_bisou_cloud.h"
#include "entities/shots/shot_cloud.h"

EnemyBisouCloud::EnemyBisouCloud() : shoot_step(0) {
	hp = 250;
	wait_shoot = 150 + rnd() % 150;
}

void EnemyBisouCloud::update() {
	if (blood > 0) blood -= 1;

	shoot_step += 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onNormal();
			break;

		case STATE_DYING:
		case STATE_CHARRED:
			onDieWait();
			break;

		case STATE_DYING_2:
			onDieFall();
			break;

		case STATE_DYING_4:
			onDieEnd();
			break;
	}

	updateDestroyed();
}

void EnemyBisouCloud::onDieEnd() {
	if (anim_step < 6) {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) anim_step += 1;

		if (dir == FACING_RIGHT) {
			pic = assets().pics.enemies.anim("cloud_die_end_right")[anim_step];
		} else {
			pic = assets().pics.enemies.anim("cloud_die_end_left")[anim_step];
		}
	} else {
		engrave(x, y, pic);
		destroyed = true;
	}
}

void EnemyBisouCloud::onDieFall() {
	fall();

	step(8, 2);

	if (dir == FACING_LEFT)
		pic = assets().pics.enemies.anim("cloud_die_fall_left")[anim_step];
	else
		pic = assets().pics.enemies.anim("cloud_die_fall_right")[anim_step];

	if (level().terrain.platform_y(x, y) != 0) {
		anim_step = anim_tick = 0;
		status = STATE_DYING_4;
	}
}

void EnemyBisouCloud::onDieWait() {
	anim_step += 1;

	if (dir == FACING_LEFT)
		pic = assets().pics.enemies.pic("cloud_stunned_left");
	else
		pic = assets().pics.enemies.pic("cloud_stunned_right");

	if (anim_step >= 30) {
		anim_step = anim_tick = 0;
		status = STATE_DYING_2;
	}
}

void EnemyBisouCloud::onNormal() {
	static const int anim_left[] = {0, 1, 2, 1};
	static const int anim_right[] = {0, 1, 2, 1};

	if (x > (level().camera.offset + 320)) {
		dir = FACING_LEFT;
		pic = assets().pics.enemies.anim("cloud_fly_left")[animate(anim_left, 4, 10)];
	} else {
		dir = FACING_RIGHT;
		pic = assets().pics.enemies.anim("cloud_fly_right")[animate(anim_right, 4, 10)];
	}

	if (shoot_step >= wait_shoot && x <= level().camera.offset + 640) {
		ShotCloud* s = new ShotCloud();

		s->x = x;
		s->y = y - 17;
		s->dir = dir;

		level().entities.enemy_shots.emplace_back(s);

		shoot_step = 0;
		wait_shoot = 250 + rnd() % 250;
	}

	colFromPic();
}
