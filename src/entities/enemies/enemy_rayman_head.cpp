#include "entities/enemies/enemy_rayman_head.h"

EnemyRaymanHead::EnemyRaymanHead() : dx(3) {
	hp = 1;
	pic = pbk_enemies[171];
}

void EnemyRaymanHead::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
		case ETAT_SAUTE:
		case ETAT_TOMBE:
			onAvance();
			break;

		case ETAT_CARBONISE:
		case ETAT_MEURE:
			onMeure();
			break;
	}
}

void EnemyRaymanHead::onAvance() {
	x += dx;
	fall();
	colFromPic();

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 8;
		pic = pbk_enemies[171 + anim_step];
	}

	if (dy > 0 && (platform_y(x, y + dy)) != 0) {
		engrave(x, y, pic);
		game_flag[0] = 2;
		destroyed = true;
	}
}

void EnemyRaymanHead::onMeure() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
		if (anim_step >= 6) {
			game_flag[0] = 2;
			destroyed = true;
		} else {
			pic = pbk_enemies[179 + anim_step];
		}
	}
}

int EnemyRaymanHead::damage() { return 0; };

void EnemyRaymanHead::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	shot->aTouche(9999);
	gicle(shot, dx_giclure, dy_giclure);
}
