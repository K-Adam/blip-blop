#include "entities/enemies/enemy_bisou_sky.h"
#include "entities/gore_feet_bisou_sky.h"
#include "entities/shots/shot_rainbow.h"

EnemyBisouSky::EnemyBisouSky() : wait_for_pisser(0) { hp = 175; }

void EnemyBisouSky::update() {
	if (blood > 0) blood -= 1;

	if (wait_for_pisser > 0) wait_for_pisser -= 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeureEntier();
			break;

		case ETAT_MEURE_2:
			onMeureHaut();
			break;

		case ETAT_MEURE_3:
			onMeureHautFin();
			break;

		case ETAT_TIRE:
			onTire();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	updateDestroyed();
}

void EnemyBisouSky::onMeureEntier() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4) {
		GoreFeetBisouSky* pieds = new GoreFeetBisouSky();

		pieds->x = x;
		pieds->dir = dir;
		pieds->y = y;

		list_gore.emplace_back(pieds);

		y -= 19;
		dy = 0;
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_MEURE_2;
		onMeureHaut();
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[32 + anim_step];
		else
			pic = pbk_enemies[36 + anim_step];
	}
}

void EnemyBisouSky::onTire() {
	anim_tick += 1;

	if (anim_tick >= 60) {
		anim_tick = 0;
		status = ETAT_AVANCE;
		wait_for_pisser = 150;
	}
}

void EnemyBisouSky::onMeureHaut() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[40 + anim_step];
		x += 1;
	} else {
		pic = pbk_enemies[44 + anim_step];
		x -= 1;
	}

	fall();

	if (platform_y(x, y) != 0 && anim_step >= 3) {
		anim_step = anim_tick = 0;
		status = ETAT_MEURE_3;
		onMeureHautFin();
	}
}

void EnemyBisouSky::onMeureHautFin() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[24 + anim_step];
	else
		pic = pbk_enemies[28 + anim_step];

	if (anim_step >= 3) {
		engrave(x, y, pic);
		destroyed = true;
	}
}

void EnemyBisouSky::onAvance() {
	static const int SPEED = 1;

	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (x - SPEED < xmin || solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(SPEED);
		pic = pbk_enemies[8 + anim_step];
	} else {
		marche(-SPEED);
		pic = pbk_enemies[16 + anim_step];
	}

	if (wait_for_pisser == 0 && anim_step == 4) {				 // Frame 4 looks best
		if (targeted_player != NULL && platform_y(x, y) != 0) {	 // No shooting while falling
			int ddx = x - targeted_player->x;
			int ddy = y - targeted_player->y;

			if (ddy > -10 && ddy < 20) {
				if (dir == FACING_LEFT) {
					if (ddx > 0 && ddx < 90) {
						sbk_level.play(9);
						ShotRainbow* s = new ShotRainbow();
						s->x = x - 15;
						s->y = y - 25;
						s->dir = FACING_LEFT;

						list_enemy_shots.emplace_back(s);

						anim_tick = 0;
						status = ETAT_TIRE;
					}
				} else {
					if (ddx < 0 && ddx > -90) {
						sbk_level.play(9);
						ShotRainbow* s = new ShotRainbow();
						s->x = x + 15;
						s->y = y - 25;
						s->dir = FACING_RIGHT;

						list_enemy_shots.emplace_back(s);

						anim_tick = 0;
						status = ETAT_TIRE;
					}
				}
			}
		}
	}

	colFromPic();
}

void EnemyBisouSky::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 6) {
			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[559 + anim_step];
	else
		pic = pbk_enemies[566 + anim_step];
}

void EnemyBisouSky::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) sbk_level.play((rand() % 9));
}
