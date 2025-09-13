#include "entities/enemies/enemy_bisou_sib.h"

#define ETAT_ATTENDS_MAL 100
#define ETAT_ATTENDS 101
#define ETAT_COURS_FOREST 102
#define ETAT_RENTRE_MAL 103

static const int anim_cours[] = {640, 641, 642, 641};

EnemyBisouSIB::EnemyBisouSIB() {
	if (game_flag[FLAG_USER1] != 0) {
		status = ETAT_ATTENDS_MAL;
		game_flag[FLAG_USER1] = 0;
	} else {
		status = ETAT_ATTENDS;
	}

	anim_tick = anim_step = 0;
	encaisse = 0;
}

void EnemyBisouSIB::update() {
	fall();

	switch (status) {
		case ETAT_ATTENDS_MAL:
			pic = pbk_enemies[635];
			colFromPic();

			if (x < offset + 440 || game_flag[1] == 0) status = ETAT_RENTRE_MAL;

			break;

		case ETAT_ATTENDS:
			pic = pbk_enemies[639];
			colFromPic();

			if (x - targeted_player->x < 200) status = ETAT_COURS_FOREST;

			break;

		case ETAT_RENTRE_MAL:
			anim_tick += 1;
			anim_tick %= 9;

			if (anim_tick == 0) {
				anim_step += 1;

				if (anim_step == 5) {
					anim_step = anim_tick = 0;
					status = ETAT_COURS_FOREST;
				} else {
					pic = pbk_enemies[635 + anim_step];
				}
			}

			noCol();
			break;

		case ETAT_COURS_FOREST:
			pic = pbk_enemies[animate(anim_cours, 4, 5)];
			marche(3);
			noCol();
			break;
	}

	updateDestroyed();
}

void EnemyBisouSIB::takeHit(Shot* shot) {
	if (++encaisse > 300) {
		if (status == ETAT_ATTENDS_MAL)
			status = ETAT_RENTRE_MAL;
		else if (status == ETAT_ATTENDS)
			status = ETAT_COURS_FOREST;

		noCol();
	}
}
