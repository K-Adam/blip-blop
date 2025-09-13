#include "entities/enemies/enemy_snorky_governor.h"
#include "entities/shots/shot_snorky_governor.h"

const int anim_gouverneur_marche_droite[] = {183, 184, 185, 186, 185, 184};
const int anim_gouverneur_marche_gauche[] = {187, 188, 189, 190, 189, 188};

EnemySnorkyGovernor::EnemySnorkyGovernor() : wait_for_shoot(0), shoot_delay(50 + rand() % 200) {
	hp = 150;
	pic = pbk_enemies[0];
	dy = 0;
}

void EnemySnorkyGovernor::update() {
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

void EnemySnorkyGovernor::onAvance() {
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
		nb_tir = 0;
		status = ETAT_TIRE;
		onTire();
		return;
	}

	if (dir == FACING_LEFT) {
		marche(-GOUVERNEUR_SPEED);

		pic = pbk_enemies[animate(anim_gouverneur_marche_gauche, 6, 4)];

		if (solid_wall(x - GOUVERNEUR_SPEED, y) || (x - GOUVERNEUR_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		marche(GOUVERNEUR_SPEED);

		pic = pbk_enemies[animate(anim_gouverneur_marche_droite, 6, 4)];

		if (solid_wall(x + GOUVERNEUR_SPEED, y) || (x + GOUVERNEUR_SPEED > offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkyGovernor::onTombe() {
	fall();

	if (platform_y(x, y) != 0) status = ETAT_AVANCE;

	colFromPic();
}

void EnemySnorkyGovernor::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 5) {
		if (platform_y(x, y) != 0) engrave(x, y, pic);

		destroyed = true;
	}

	if (dir == FACING_LEFT) {
		pic = pbk_enemies[210 + anim_step];
	} else {
		pic = pbk_enemies[205 + anim_step];
	}
}

void EnemySnorkyGovernor::onTire() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step == 10) {
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_AVANCE;
		onAvance();
		return;
	}

	if (anim_step == 6 && anim_tick == 0) {
		if (targeted_player != NULL) {
			ShotSnorkyGovernor* shot = new ShotSnorkyGovernor(targeted_player, rand() % 6 - 2, -5 - rand() % 5);

			shot->setDir(dir);
			shot->x = x;
			shot->y = y - 71;

			list_enemy_shots.emplace_back(shot);
			nb_tir++;
		}

		if (nb_tir == 0) {
			sbk_level.play(6);
		}
	}
	if ((anim_step == 7) && (nb_tir < 5) && (anim_tick == 0)) {
		anim_step -= 4;
	}

	if (anim_step < 7) {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[191 + anim_step];
		} else {
			pic = pbk_enemies[198 + anim_step];
		}
	}
}

void EnemySnorkyGovernor::onCarbonise() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 8)
		destroyed = true;

	else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[224 + anim_step];

		else
			pic = pbk_enemies[215 + anim_step];
	}
}

void EnemySnorkyGovernor::takeHit(Shot* shot) {
	static const int dx_giclure_gouverneur[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_gouverneur[] = {-15, -15, -15, -15, -35, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_gouverneur, dy_giclure_gouverneur);

	if (status == ETAT_MEURE) {
		sbk_level.play(4 + rand() % 2);
	}
}
