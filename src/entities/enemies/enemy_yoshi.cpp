#include "entities/enemies/enemy_yoshi.h"
#include "entities/shots/shot_shell.h"

const int anim_yoshi_marche_droite[] = {28, 29, 30, 29};
const int anim_yoshi_marche_gauche[] = {31, 32, 33, 32};
const int anim_yoshi_attaque_droite[] = {34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34};
const int anim_yoshi_attaque_gauche[] = {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44};

EnemyYoshi::EnemyYoshi() : attack_delay(50 + rand() % 250), wait_for_attack(0) {
	tresor = 10;
	hp = 500;
}

void EnemyYoshi::update() {
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

		case ETAT_TOMBE:
			onTombe();
			break;

		case ETAT_TIRE:	 // ETAT_TIRE is the tongue or shell attack
			if (attack) {
				onTire();
			} else {
				onAttaque();
			}
			break;
	}

	updateDestroyed();
}

void EnemyYoshi::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		anim_step = 0;
		anim_tick = 0;
		onTombe();
		return;
	}

	wait_for_attack++;
	if (wait_for_attack >= attack_delay && x <= offset + 640) {
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		anim_step = 0;
		status = ETAT_TIRE;
		attack = true;
		anim_step = 0;
		anim_tick = 0;
		sbk_level.play(35);
		onTire();
		return;
	}

	if ((platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y)) &&
		(((dir == FACING_RIGHT) && (x > (targeted_player->x - 150)) && (targeted_player->x > x)) ||
		 ((dir == FACING_LEFT) && (x < (targeted_player->x + 150)) && (targeted_player->x < x)))) {
		anim_step = 0;
		status = ETAT_TIRE;
		attack = false;
		anim_step = 0;
		anim_tick = 0;
		sbk_level.play(37);
		onAttaque();
		return;
	}

	if (x - YOSHI_SPEED < xmin || solid_wall(x - YOSHI_SPEED, y)) {
		dir = FACING_RIGHT;
	} else if (x + YOSHI_SPEED > offset + 640 || solid_wall(x + YOSHI_SPEED, y)) {
		dir = FACING_LEFT;
	}

	if (dir == FACING_RIGHT) {
		marche(YOSHI_SPEED);
		pic = pbk_enemies[animate(anim_yoshi_marche_droite, 3, 3)];
	} else {
		marche(-YOSHI_SPEED);
		pic = pbk_enemies[animate(anim_yoshi_marche_gauche, 3, 3)];
	}
	colFromPic();
}

void EnemyYoshi::onTombe() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	}

	if (dir == FACING_RIGHT) {
		x++;
		pic = pbk_enemies[28];
	} else {
		x--;
		pic = pbk_enemies[31];
	}

	colFromPic();
}

void EnemyYoshi::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 9) anim_step += 1;

	if (anim_step >= 9) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[71 + anim_step];
		else
			pic = pbk_enemies[62 + anim_step];
	}
}

void EnemyYoshi::onAttaque() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[animate(anim_yoshi_attaque_droite, 18, 2)];
	} else {
		pic = pbk_enemies[animate(anim_yoshi_attaque_gauche, 18, 1)];
	}
	if (anim_step >= 17) {
		status = ETAT_AVANCE;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	}
	colFromPic();
}

void EnemyYoshi::onTire() {
	anim_tick += 1;
	anim_tick %= 15;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 2) {
		ShotShell* shot = new ShotShell();

		shot->setDir(dir);

		if (dir == FACING_LEFT)
			shot->x = x - 23;
		else
			shot->x = x + 23;

		shot->y = y - 23;

		list_enemy_shots.emplace_back(shot);

		status = ETAT_AVANCE;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[54 + anim_step];
	} else {
		pic = pbk_enemies[56 + anim_step];
	}

	colFromPic();
}

void EnemyYoshi::onCarbonise() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step++;

	if (anim_step >= 14)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[460 + anim_step];
		else
			pic = pbk_enemies[474 + anim_step];
	}
}

void EnemyYoshi::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) {
		sbk_misc.play(6 + rand() % 3);
	}
}
