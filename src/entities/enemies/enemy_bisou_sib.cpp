#include "entities/enemies/enemy_bisou_sib.h"

#define STATE_WAITING_HURT 100
#define STATE_WAITING 101
#define STATE_RUN_FOREST 102
#define STATE_HURT_RECOVER 103

static const int anim_run[] = {640, 641, 642, 641};

EnemyBisouSIB::EnemyBisouSIB() {
	if (game_flag[FLAG_USER1] != 0) {
		status = STATE_WAITING_HURT;
		game_flag[FLAG_USER1] = 0;
	} else {
		status = STATE_WAITING;
	}

	anim_tick = anim_step = 0;
	hits_taken = 0;
}

void EnemyBisouSIB::update() {
	fall();

	switch (status) {
		case STATE_WAITING_HURT:
			pic = pbk_enemies[635];
			colFromPic();

			if (x < offset + 440 || game_flag[1] == 0) status = STATE_HURT_RECOVER;

			break;

		case STATE_WAITING:
			pic = pbk_enemies[639];
			colFromPic();

			if (x - targeted_player->x < 200) status = STATE_RUN_FOREST;

			break;

		case STATE_HURT_RECOVER:
			anim_tick += 1;
			anim_tick %= 9;

			if (anim_tick == 0) {
				anim_step += 1;

				if (anim_step == 5) {
					anim_step = anim_tick = 0;
					status = STATE_RUN_FOREST;
				} else {
					pic = pbk_enemies[635 + anim_step];
				}
			}

			noCol();
			break;

		case STATE_RUN_FOREST:
			pic = pbk_enemies[animate(anim_run, 4, 5)];
			walk(3);
			noCol();
			break;
	}

	updateDestroyed();
}

void EnemyBisouSIB::takeHit(Shot* shot) {
	if (++hits_taken > 300) {
		if (status == STATE_WAITING_HURT)
			status = STATE_HURT_RECOVER;
		else if (status == STATE_WAITING)
			status = STATE_RUN_FOREST;

		noCol();
	}
}
