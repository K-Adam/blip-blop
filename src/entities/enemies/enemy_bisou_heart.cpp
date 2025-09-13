#include "entities/enemies/enemy_bisou_heart.h"
#include "entities/gore_head_bisou_heart.h"
#include "entities/shots/shot_bisou_heart.h"

EnemyBisouHeart::EnemyBisouHeart() { hp = 300; }

void EnemyBisouHeart::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_MEURE_2:
			onPart();
			break;

		case ETAT_CARBONISE:
			break;
	}

	updateDestroyed();
}

void EnemyBisouHeart::onAvance() {
	static const int SPEED = 2;

	static const int anim_gauche[] = {431, 432, 433, 434, 433, 432};
	static const int anim_droite[] = {427, 428, 429, 430, 429, 428};

	if (dir == FACING_LEFT) {
		x -= SPEED;

		if (solid_wall(x, y) || x < xmin) dir = FACING_RIGHT;

		pic = pbk_enemies[animate(anim_gauche, 6, 7)];
	} else {
		x += SPEED;

		if (solid_wall(x, y) || x > offset + 600) dir = FACING_LEFT;

		pic = pbk_enemies[animate(anim_droite, 6, 7)];
	}

	if (anim_step == 3 && anim_tick == 0) {
		ShotBisouHeart* shot = new ShotBisouHeart();

		if (dir == FACING_RIGHT)
			shot->xbase = x + 10;
		else
			shot->xbase = x - 10;

		shot->y = y;

		list_enemy_shots.emplace_back(shot);
	}

	colFromPic();
}

void EnemyBisouHeart::onMeure() {
	static const int SPEED = 2;

	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 5) {
			status = ETAT_MEURE_2;
			return;
		}

		if (anim_step == 4) {
			GoreHeadBisouHeart* tete = new GoreHeadBisouHeart();

			tete->y = y - 20;
			tete->dir = dir;

			if (dir == FACING_RIGHT)
				tete->x = x + 11;
			else
				tete->x = x - 11;

			list_gore.emplace_back(tete);
		}
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[435 + anim_step];
		x += SPEED;
	} else {
		pic = pbk_enemies[441 + anim_step];
		x -= SPEED;
	}
}

void EnemyBisouHeart::onPart() {
	static const int SPEED = 10;

	if (dir == FACING_RIGHT)
		x += SPEED;
	else
		x -= SPEED;
}

void EnemyBisouHeart::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) sbk_level.play((rand() % 9));
}
