#include "entities/enemies/enemy.h"
#include "entities/shots/shot_cloud.h"

ShotCloud::ShotCloud() : status(0), shoot_step(0), lightning_timer(0), lightning(0) { lifespan = 600 + rnd() % 400; }

void ShotCloud::update() {
	switch (status) {
		case 0:
			onCreation();
			break;

		case 1:
			onNormal();
			break;

		case 2:
			onDestruction();
			break;

		case 3:
			onShoot();
			break;
	}

	updateDestroyed();
}

void ShotCloud::onCreation() {
	static const int SPEED = 1;

	anim_tick += 1;
	anim_tick %= 10;

	if (dir == FACING_RIGHT) {
		x += SPEED;

		if (x > level().camera.offset + 600) dir = FACING_LEFT;
	} else {
		x -= SPEED;

		if (x < level().camera.offset + 30) dir = FACING_RIGHT;
	}

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step >= 3) {
			anim_tick = anim_step = 0;
			status = 1;
		} else {
			pic = assets().pics.enemies.anim("cloud_shot_grow")[anim_step];
		}
	}

	noCol();
}

void ShotCloud::onDestruction() {
	static const int SPEED = 1;

	anim_tick += 1;
	anim_tick %= 6;

	if (dir == FACING_RIGHT)
		x += SPEED;
	else
		x -= SPEED;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step >= 3)
			destroyed = true;
		else
			pic = assets().pics.enemies.anim("cloud_shot_grow")[2 - anim_step];
	}

	noCol();
}

void ShotCloud::onNormal() {
	static const int anim[] = {0, 1, 2, 1};
	static const int SPEED = 1;

	lifespan -= 1;

	if (lifespan <= 0) {
		anim_step = anim_tick = 0;
		status = 2;
	}

	pic = assets().pics.enemies.anim("cloud_shot_float")[animate(anim, 4, 9)];

	if (dir == FACING_RIGHT) {
		x += SPEED;

		if (x > level().camera.offset + 600) dir = FACING_LEFT;
	} else {
		x -= SPEED;

		if (x < level().camera.offset + 30) dir = FACING_RIGHT;
	}

	if (shoot_step == 0) {
		if (session().targeted_player != NULL && session().targeted_player->y > y) {
			int ddx = x - session().targeted_player->x;

			if (ddx > -10 && ddx < 10) {
				shoot_step = 0;
				status = 3;
			}
		}
	} else {
		shoot_step -= 1;
	}

	noCol();
}

void ShotCloud::onShoot() {
	static const int anim[] = {0, 1, 2, 1};

	lightning_timer += 1;
	lightning_timer %= 2;

	if (lightning_timer == 0) {
		lightning += 1;
		lightning %= 4;
	}

	lifespan -= 1;

	if (lifespan <= 0) {
		anim_step = anim_tick = 0;
		status = 2;
	}

	pic = assets().pics.enemies.anim("cloud_shot_float")[animate(anim, 4, 9)];

	shoot_step += 1;

	if (shoot_step == 50) {
		assets().sounds.level.play(24);
	}

	if (shoot_step >= 100) {
		status = 1;
		shoot_step = 100;
		colFromPic();
	} else if (shoot_step > 50) {
		x1 = x - 20;
		x2 = x + 20;
		y1 = y - 10;
		y2 = 480;
		col_on = true;
	}

	dy = rnd() % 20;
}

void ShotCloud::render() {
	int bolt_frame = lightning;

	if (status == 3 && shoot_step > 50) {
		for (int yy = y - 10 + dy; yy < 480; yy += 100) {
			draw(x, yy, assets().pics.enemies.anim("cloud_shot_lightning")[bolt_frame]);

			bolt_frame = (bolt_frame + 1) % 4;
		}
	}

	Sprite::render();
}
