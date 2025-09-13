#include "entities/enemies/enemy_bulbizarre.h"

EnemyBulbizarre::EnemyBulbizarre() { hp = 100; }

void EnemyBulbizarre::update() {
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

void EnemyBulbizarre::onAvance() {
	fall();

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (x - 1 < xmin || solid_wall(x - 1, y))
		dir = FACING_RIGHT;
	else if (x + 1 > offset + 640 || solid_wall(x + 1, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(1);
		pic = pbk_enemies[24 + anim_step];
	} else {
		marche(-1);
		pic = pbk_enemies[26 + anim_step];
	}

	colFromPic();
}

void EnemyBulbizarre::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0 && anim_step < 8) anim_step += 1;

	if (anim_step >= 8) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[36 + anim_step];
		else
			pic = pbk_enemies[28 + anim_step];
	}
}

void EnemyBulbizarre::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 14)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[128 + anim_step];
		else
			pic = pbk_enemies[143 + anim_step];
	}
}

void EnemyBulbizarre::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) {
		sbk_misc.play(8 + rand() % 2);
	}
}
