#include "entities/enemies/enemy_bisou_sib.h"

#define STATE_WAITING_HURT 100
#define STATE_WAITING 101
#define STATE_RUN_FOREST 102
#define STATE_HURT_RECOVER 103

static const int anim_run[] = {0, 1, 2, 1};

EnemyBisouSIB::EnemyBisouSIB() {
	if (level().rules.flags[FLAG_USER1] != 0) {
		status = STATE_WAITING_HURT;
		level().rules.flags[FLAG_USER1] = 0;
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
			pic = assets().pics.enemies.anim("sib_get_up")[0];
			colFromPic();

			if (x < level().camera.offset + 440 || level().rules.flags[1] == 0) status = STATE_HURT_RECOVER;

			break;

		case STATE_WAITING:
			pic = assets().pics.enemies.anim("sib_get_up")[4];
			colFromPic();

			if (x - session().targeted_player->x < 200) status = STATE_RUN_FOREST;

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
					pic = assets().pics.enemies.anim("sib_get_up")[anim_step];
				}
			}

			noCol();
			break;

		case STATE_RUN_FOREST:
			pic = assets().pics.enemies.anim("sib_run_right")[animate(anim_run, 4, 5)];
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
