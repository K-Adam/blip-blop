#include "entities/enemies/enemy.h"
#include "entities/shots/shot_cloud.h"

ShotCloud::ShotCloud() : etape_shoot(0), status(0), eclair(0), delta_eclair(0) { duree_vie = 600 + rand() % 400; }

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
			onTire();
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

		if (x > offset + 600) dir = FACING_LEFT;
	} else {
		x -= SPEED;

		if (x < offset + 30) dir = FACING_RIGHT;
	}

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step >= 3) {
			anim_tick = anim_step = 0;
			status = 1;
		} else {
			pic = pbk_enemies[250 + anim_step];
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
			pic = pbk_enemies[252 - anim_step];
	}

	noCol();
}

void ShotCloud::onNormal() {
	static const int anim[] = {253, 254, 255, 254};
	static const int SPEED = 1;

	duree_vie -= 1;

	if (duree_vie <= 0) {
		anim_step = anim_tick = 0;
		status = 2;
	}

	pic = pbk_enemies[animate(anim, 4, 9)];

	if (dir == FACING_RIGHT) {
		x += SPEED;

		if (x > offset + 600) dir = FACING_LEFT;
	} else {
		x -= SPEED;

		if (x < offset + 30) dir = FACING_RIGHT;
	}

	if (etape_shoot == 0) {
		if (targeted_player != NULL && targeted_player->y > y) {
			int ddx = x - targeted_player->x;

			if (ddx > -10 && ddx < 10) {
				etape_shoot = 0;
				status = 3;
			}
		}
	} else {
		etape_shoot -= 1;
	}

	noCol();
}

void ShotCloud::onTire() {
	static const int anim[] = {253, 254, 255, 254};
	static const int SPEED = 1;

	delta_eclair += 1;
	delta_eclair %= 2;

	if (delta_eclair == 0) {
		eclair += 1;
		eclair %= 4;
	}

	duree_vie -= 1;

	if (duree_vie <= 0) {
		anim_step = anim_tick = 0;
		status = 2;
	}

	pic = pbk_enemies[animate(anim, 4, 9)];

	etape_shoot += 1;

	if (etape_shoot == 50) {
		sbk_level.play(24);
	}

	if (etape_shoot >= 100) {
		status = 1;
		etape_shoot = 100;
		colFromPic();
	} else if (etape_shoot > 50) {
		x1 = x - 20;
		x2 = x + 20;
		y1 = y - 10;
		y2 = 480;
		col_on = true;
	}

	dy = rand() % 20;
}

void ShotCloud::render() {
	int de = eclair;

	if (status == 3 && etape_shoot > 50) {
		for (int yy = y - 10 + dy; yy < 480; yy += 100) {
			draw(x, yy, pbk_enemies[256 + de]);

			de = (de + 1) % 4;
		}
	}

	Sprite::render();
}
