#include "entities/enemies/enemy_smurf_jokey.h"
#include "entities/shots/shot_gift.h"

const int anim_smurf_farceur_marche_droite[] = {46, 47, 48, 49, 50, 49, 48, 47};
const int anim_smurf_farceur_marche_gauche[] = {51, 52, 53, 54, 55, 54, 53, 52};

EnemySmurfJokey::EnemySmurfJokey() {
	hp = 150;
	bomb_delay = 100 + rand() % 400;
	wait_for_bomb = 0;
	porte_bomb = true;
	wait_for_new_bomb = 0;
	new_bomb_delay = 50 + rand() % 200;
	decalage_y_cadeau = 5;
}

void EnemySmurfJokey::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;
		case ETAT_MEURE:
			onMeure();
			break;
		case ETAT_ATTEND:
			onPoseCadeau();
			break;
		case ETAT_DEGAINE:
			onDegaine();
			break;
		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}
	updateDestroyed();
}

void EnemySmurfJokey::onDegaine() {
	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) anim_step += 1;

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[57 + anim_step];
	else
		pic = pbk_enemies[62 + anim_step];

	if (anim_step > 4) {
		status = STATE_NORMAL;
		porte_bomb = true;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
	}
}

void EnemySmurfJokey::onPoseCadeau() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) decalage_y_cadeau -= 1;

	if (decalage_y_cadeau < 0) {
		status = STATE_NORMAL;
		porte_bomb = false;
		decalage_y_cadeau = 5;
		wait_for_new_bomb = 0;
		new_bomb_delay = 50 + rand() % 200;

		ShotGift* shot = new ShotGift();
		if (dir == FACING_RIGHT)
			shot->x = x + 26;
		else
			shot->x = x - 26;
		shot->y = y;
		list_enemy_shots.emplace_back(shot);

		anim_step = 0;
		anim_tick = 0;
	}
}

void EnemySmurfJokey::onAvance() {
	fall();

	if (porte_bomb) {
		wait_for_bomb += 1;

		if (wait_for_bomb > bomb_delay && platform_y(x, y) != 0) {
			status = ETAT_ATTEND;
			wait_for_bomb = 0;
			bomb_delay = 50 + rand() % 250;
			anim_step = 0;
			anim_tick = 0;
		}
	} else if (platform_y(x, y) != 0) {
		wait_for_new_bomb += 1;
		if (wait_for_new_bomb > new_bomb_delay) {
			wait_for_new_bomb = 0;
			new_bomb_delay = 50 + rand() % 200;
			status = ETAT_DEGAINE;
			anim_step = 0;
			anim_tick = 0;
		}
	}

	if (x - SMURF_FARCEUR_SPEED < xmin || solid_wall(x - SMURF_FARCEUR_SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SMURF_FARCEUR_SPEED > offset + 640 || solid_wall(x + SMURF_FARCEUR_SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(SMURF_FARCEUR_SPEED);
		if (porte_bomb)
			pic = pbk_enemies[animate(anim_smurf_farceur_marche_droite, 8, 4)];
		else
			pic = pbk_enemies[animate(anim_smurf_marche_droite, 8, 4)];
	} else {
		marche(-SMURF_FARCEUR_SPEED);
		if (porte_bomb)
			pic = pbk_enemies[animate(anim_smurf_farceur_marche_gauche, 8, 4)];
		else
			pic = pbk_enemies[animate(anim_smurf_marche_gauche, 8, 4)];
	}
	colFromPic();
}

void EnemySmurfJokey::onMeure() {
	fall();

	if (porte_bomb) {
		status = ETAT_CARBONISE;
		porte_bomb = false;
		ShotGift* shot = new ShotGift();
		shot->anim_step = 20;
		shot->y = y - 5;
		if (dir == FACING_RIGHT) {
			shot->x = x + 26;
		} else {
			shot->x = x - 26;
		}
		list_enemy_shots.emplace_back(shot);
		onCarbonise();
	} else {
		anim_tick += 1;
		anim_tick %= 4;

		if (anim_tick == 0) anim_step += 1;

		if (anim_step > 10) {
			destroyed = true;
			return;
		}

		if (dir == FACING_LEFT) {
			if (!solid_wall(x - SMURF_FARCEUR_SPEED, y) && platform_y(x, y) == 0) x -= SMURF_FARCEUR_SPEED;

			pic = pbk_enemies[23 + anim_step];
		} else {
			if (!solid_wall(x + SMURF_FARCEUR_SPEED, y) && platform_y(x, y) == 0) x += SMURF_FARCEUR_SPEED;

			pic = pbk_enemies[12 + anim_step];
		}
	}
}

void EnemySmurfJokey::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 11) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[78 + anim_step];
		else
			pic = pbk_enemies[67 + anim_step];
	}
}

void EnemySmurfJokey::takeHit(Shot* shot) {
	static const int dx_giclure_smurf_farceur[] = {0, 0, 6, 0, 0, 0, -6, 0};
	static const int dy_giclure_smurf_farceur[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_smurf_farceur, dy_giclure_smurf_farceur);

	if (status == ETAT_MEURE) {
		if (porte_bomb)
			sbk_misc.play(11);
		else
			sbk_level.play(4 + rand() % 10);
	}
}
