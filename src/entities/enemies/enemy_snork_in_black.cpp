#include "entities/enemies/enemy_snork_in_black.h"
#include "entities/shots/shot_thorn.h"

const int anim_snork_in_black_marche_droite[] = {344, 345, 346, 345};
const int anim_snork_in_black_marche_gauche[] = {347, 348, 349, 348};

EnemySnorkInBlack::EnemySnorkInBlack() : wait_for_shoot(0), shoot_delay(50 + rand() % 100), nb_tir(0) {
	hp = 250;
	dy = 0;
}

void EnemySnorkInBlack::update() {
	switch (status) {
		case ETAT_AVANCE:
		case STATE_NORMAL:
			onAvance();
			break;

		case ETAT_TOMBE:
		case ETAT_SAUTE:
			onTombe();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;

		case ETAT_TIRE:
			onTire();
			break;
	}

	updateDestroyed();
}

void EnemySnorkInBlack::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onTombe();
		return;
	}

	wait_for_shoot++;
	if ((targeted_player != NULL) && (wait_for_shoot > shoot_delay) && ((dir == FACING_RIGHT && x > offset + 80) || (dir == FACING_LEFT && x < offset + 560)))

	{
		x_cible = targeted_player->x;
		y_target = targeted_player->y;

		if (x_cible != x) {
			int vy = ((y_target - y + 14) * 6) / (x_cible - x);

			if ((dir == FACING_RIGHT) && (x < x_cible) && (vy < 2) && (vy > -2)) {
				wait_for_shoot = 0;
				shoot_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				nb_tir = rand() % 3;
				status = ETAT_TIRE;
				onTire();
				return;
			} else if ((dir == FACING_LEFT) && (x > x_cible) && (vy < 2) && (vy > -2)) {
				wait_for_shoot = 0;
				shoot_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				nb_tir = rand() % 3;
				status = ETAT_TIRE;
				onTire();
				return;
			}
		}
	}

	if (dir == FACING_LEFT) {
		marche(-SNORK_IN_BLACK_SPEED);

		pic = pbk_enemies[animate(anim_snork_in_black_marche_gauche, 4, 4)];

		if (solid_wall(x - SNORK_IN_BLACK_SPEED, y) || (x - SNORK_IN_BLACK_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		marche(SNORK_IN_BLACK_SPEED);

		pic = pbk_enemies[animate(anim_snork_in_black_marche_droite, 4, 4)];

		if (solid_wall(x + SNORK_IN_BLACK_SPEED, y) || (x + SNORK_IN_BLACK_SPEED > offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkInBlack::onTombe() {
	fall();

	if (platform_y(x, y) != 0) status = ETAT_AVANCE;

	colFromPic();
}

void EnemySnorkInBlack::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 5) {
		if (platform_y(x, y) != 0) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[354 + anim_step];
		} else {
			pic = pbk_enemies[359 + anim_step];
		}
	}
}

void EnemySnorkInBlack::onTire() {
	anim_tick++;
	if (anim_step == 0) {
		anim_tick %= 20;
	} else {
		anim_tick %= 4;
	}

	if (anim_tick == 0) {
		anim_step++;
	}

	if ((anim_step == 1) && (anim_tick == 0)) {
		if (dir == FACING_RIGHT) {
			int vx = 5 + rand() % 2;
			int vy = rand() % 5 - 2;

			ShotThorn* shot = new ShotThorn(364, vx, vy);

			shot->setDir(dir);
			shot->x = x + 49;
			shot->y = y - 15;

			list_enemy_shots.emplace_back(shot);
		} else {
			int vx = -5 - rand() % 2;
			int vy = rand() % 5 - 2;

			ShotThorn* shot = new ShotThorn(365, vx, vy);

			shot->setDir(dir);
			shot->x = x - 49;
			shot->y = y - 15;

			list_enemy_shots.emplace_back(shot);
		}

		sbk_level.play(24);
	}

	if (anim_step == 3) {
		nb_tir++;
		anim_step = 0;
		if (nb_tir >= SNORK_IN_BLACK_MAX_TIR) {
			anim_tick = 0;
			anim_step = 0;
			status = ETAT_AVANCE;
			onAvance();
			return;
		}
	}

	if (dir == FACING_RIGHT) {
		if (anim_step == 0) {
			pic = pbk_enemies[344];
		} else {
			pic = pbk_enemies[350 + anim_step - 1];
		}
	} else {
		if (anim_step == 0) {
			pic = pbk_enemies[347];
		} else {
			pic = pbk_enemies[352 + anim_step - 1];
		}
	}
	colFromPic();
}

void EnemySnorkInBlack::onCarbonise() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 7) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[373 + anim_step];
		else
			pic = pbk_enemies[366 + anim_step];
	}
}

void EnemySnorkInBlack::takeHit(Shot* shot) {
	static const int dx_giclure_snorkbase1[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_snorkbase1[] = {-15, -15, -15, -15, -25, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_snorkbase1, dy_giclure_snorkbase1);

	if (status == ETAT_MEURE) {
		sbk_level.play(21 + rand() % 3);
	}
}
