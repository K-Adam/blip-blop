#include "entities/enemies/enemy_snorky_base_1.h"
#include "entities/piece_snorky_head.h"
#include "entities/shots/shot_snorky_base.h"

const int anim_snorky_base_marche_droite[] = {0, 1, 2, 3, 4, 3, 2, 1};
const int anim_snorky_base_marche_gauche[] = {5, 6, 7, 8, 9, 8, 7, 6};

EnemySnorkyBase1::EnemySnorkyBase1() : wait_for_shoot(0), shoot_delay(50 + rand() % 200) {
	hp = 200;
	pic = pbk_enemies[0];
	dy = 0;
}

void EnemySnorkyBase1::update() {
	if (blood > 0) blood -= 1;

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

void EnemySnorkyBase1::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onTombe();
		return;
	}

	wait_for_shoot++;
	if ((wait_for_shoot > shoot_delay) && ((dir == FACING_RIGHT && x > offset + 40) || (dir == FACING_LEFT && x < offset + 600))) {
		wait_for_shoot = 0;
		shoot_delay = 50 + rand() % 200;
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_TIRE;
		onTire();
		return;
	}

	if (dir == FACING_LEFT) {
		marche(-SNORKY_BASE_SPEED);

		pic = pbk_enemies[animate(anim_snorky_base_marche_gauche, 8, 4)];

		if (solid_wall(x - SNORKY_BASE_SPEED, y) || (x - SNORKY_BASE_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		marche(SNORKY_BASE_SPEED);

		pic = pbk_enemies[animate(anim_snorky_base_marche_droite, 8, 4)];

		if (solid_wall(x + SNORKY_BASE_SPEED, y) || (x + SNORKY_BASE_SPEED > offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkyBase1::onTombe() {
	fall();

	if (platform_y(x, y) != 0) status = ETAT_AVANCE;

	colFromPic();
}

void EnemySnorkyBase1::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4 && anim_tick == 1) {
		Sprite* s = new PieceSnorkyBaseHeadWhole();

		s->dir = dir;
		s->y = y - 22;
		s->x = x;

		list_splatters.emplace_back(s);

		dy = 0;
	}
	if (anim_step == 14) {
		if (platform_y(x, y) != 0) {
			engrave(x, y, pic);
		}

		destroyed = true;

	} else {
		if (dir == FACING_LEFT) {
			marche(-SNORKY_BASE_SPEED);
			pic = pbk_enemies[71 + anim_step];
		} else {
			marche(SNORKY_BASE_SPEED);
			pic = pbk_enemies[38 + anim_step];
		}
	}
}

void EnemySnorkyBase1::onTire() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step == 12) {
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_AVANCE;
		onAvance();
		return;
	}

	if (anim_step == 10 && anim_tick == 0) {
		if (dir == FACING_RIGHT) {
			ShotSnorkyBase* shot = new ShotSnorkyBase(1);

			shot->setDir(dir);
			shot->x = x + 35;
			shot->y = y - 42;

			list_enemy_shots.emplace_back(shot);

		} else {
			ShotSnorkyBase* shot = new ShotSnorkyBase(-1);

			shot->setDir(dir);
			shot->x = x - 35;
			shot->y = y - 42;

			list_enemy_shots.emplace_back(shot);
		}

		sbk_level.play(17);
	}

	if (anim_step < 11) {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[10 + anim_step];
		} else {
			pic = pbk_enemies[21 + anim_step];
		}
	}
}

void EnemySnorkyBase1::onCarbonise() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 9) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[105 + anim_step];
		else
			pic = pbk_enemies[96 + anim_step];
	}
}

void EnemySnorkyBase1::takeHit(Shot* shot) {
	static const int dx_giclure_snorkbase1[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_snorkbase1[] = {-15, -15, -15, -15, -25, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_snorkbase1, dy_giclure_snorkbase1);

	if (status == ETAT_MEURE) {
		sbk_level.play(14 + rand() % 3);
	}
}
