#include "entities/enemies/enemy_pikachu.h"

EnemyPikachu::EnemyPikachu() { hp = 100; }

void EnemyPikachu::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	updateDestroyed();
}

void EnemyPikachu::onAvance() {
	static const int SPEED = 3;

	fall();

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (x - SPEED < xmin || solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(SPEED);
		pic = pbk_enemies[anim_step];
	} else {
		marche(-SPEED);
		pic = pbk_enemies[2 + anim_step];
	}

	colFromPic();
}

void EnemyPikachu::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0 && anim_step < 10) anim_step += 1;

	if (anim_step >= 10) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[14 + anim_step];
		else
			pic = pbk_enemies[4 + anim_step];
	}
}

void EnemyPikachu::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 14)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[100 + anim_step];
		else
			pic = pbk_enemies[114 + anim_step];
	}
}

void EnemyPikachu::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) {
		sbk_misc.play(8 + rand() % 2);
	}
}
