#include "entities/enemies/enemy_papa_smurf.h"

#define ATT_ATATA 0
#define ATT_BOOM 1

EnemyPapaSmurf::EnemyPapaSmurf() : wait_change_cible(0), dx(0), cible(NULL), attak_wanted(ATT_BOOM) { hp = 5000; }

void EnemyPapaSmurf::updateCible() {
	if (wait_change_cible > 0 && cible != NULL && !cible->destroyed) {
		wait_change_cible -= 1;
	} else {
		if (targeted_player != NULL && !targeted_player->destroyed) {
			cible = targeted_player;
			wait_change_cible = 200 + rand() % 100;
		} else {
			cible = NULL;
		}
	}
}

void EnemyPapaSmurf::updateChoice() {
	if (cible == NULL) return;

	int ddx = cible->x - x;

	if (ddx < 0)
		dir = FACING_LEFT;
	else
		dir = FACING_RIGHT;

	if (attak_wanted == ATT_ATATA) {
		if (ddx > -100 && ddx < 100) {
			anim_step = anim_tick = 0;
			status = ETAT_TIRE;
		} else if (ddx < 0)
			dx = -3;
		else
			dx = 3;
	} else {
		if (ddx < -400 || ddx > 400 || x <= offset + 60 || x >= offset + 580)
			attak_wanted = ATT_ATATA;
		else if (ddx < 0)
			dx = 3;
		else
			dx = -3;
	}
}

void EnemyPapaSmurf::update() {
	if (blood != 0) blood -= 1;

	updateCible();

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			updateChoice();
			onAvance();
			break;

		case ETAT_TIRE:
			switch (attak_wanted) {
				case ATT_ATATA:
					onAtata();
					break;
			}
			break;

		case ETAT_CARBONISE:
		case ETAT_MEURE:
			break;
	}
}

void EnemyPapaSmurf::onAtata() {
	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step > 50) {
		anim_step = anim_tick = 0;
		status = STATE_NORMAL;
		attak_wanted = rand() % 2;
	} else {
		if (dir == FACING_LEFT) {
			pic = pbk_enemies[3 + (anim_step % 3)];
			dx = -1;
		} else {
			pic = pbk_enemies[(anim_step % 3)];
			dx = 1;
		}

		int x2 = x + dx;

		if (!solid_wall(x2, y) && x2 > offset + 50 && x2 < offset + 600) x = x2;
	}
}

void EnemyPapaSmurf::onAvance() {
	int x2 = x + dx;

	if (!solid_wall(x2, y) && x2 > offset + 50 && x2 < offset + 600) x = x2;

	if (dx <= 0) {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[9];
		else
			pic = pbk_enemies[37];
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[40];
		else
			pic = pbk_enemies[6];
	}
}
