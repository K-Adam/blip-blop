#include "entities/enemies/enemy_yoshi_antiair.h"
#include "entities/shots/shot_shell.h"

EnemyYoshiAntiair::EnemyYoshiAntiair() : attack_delay(50 + rand() % 150), wait_for_attack(0) {
	tresor = 12;
	hp = 400;
}

void EnemyYoshiAntiair::update() {
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

		case ETAT_TIRE:	 // ETAT_TIRE is throwing shells
			onTire();
			break;
	}

	updateDestroyed();
}

void EnemyYoshiAntiair::onAvance() {
	fall();
	wait_for_attack++;
	if (wait_for_attack >= attack_delay && x <= offset + 640) {
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 150;
		if (targeted_player->x > x) {
			dir = FACING_RIGHT;
		} else {
			dir = FACING_LEFT;
		}
		anim_step = 0;
		status = ETAT_TIRE;
		sbk_level.play(35);
		onTire();
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[28];
	} else {
		pic = pbk_enemies[31];
	}

	colFromPic();
}

void EnemyYoshiAntiair::onMeure() {
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

void EnemyYoshiAntiair::onTire() {
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

		anim_step = 0;
		status = ETAT_AVANCE;
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[54 + anim_step];
	} else {
		pic = pbk_enemies[56 + anim_step];
	}

	colFromPic();
}

void EnemyYoshiAntiair::onCarbonise() {
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

void EnemyYoshiAntiair::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) {
		sbk_misc.play(6 + rand() % 3);
	}
}
