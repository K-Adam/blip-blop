#include "entities/enemies/enemy_papa_smurf.h"

#define ATT_ATATA 0
#define ATT_BOOM 1

EnemyPapaSmurf::EnemyPapaSmurf() : wait_change_target(0), dx(0), target(NULL), attack_wanted(ATT_BOOM) { hp = 5000; }

void EnemyPapaSmurf::updateTarget() {
	if (wait_change_target > 0 && target != NULL && !target->destroyed) {
		wait_change_target -= 1;
	} else {
		if (targeted_player != NULL && !targeted_player->destroyed) {
			target = targeted_player;
			wait_change_target = 200 + rand() % 100;
		} else {
			target = NULL;
		}
	}
}

void EnemyPapaSmurf::updateChoice() {
	if (target == NULL) return;

	int ddx = target->x - x;

	if (ddx < 0)
		dir = FACING_LEFT;
	else
		dir = FACING_RIGHT;

	if (attack_wanted == ATT_ATATA) {
		if (ddx > -100 && ddx < 100) {
			anim_step = anim_tick = 0;
			status = STATE_SHOOT;
		} else if (ddx < 0)
			dx = -3;
		else
			dx = 3;
	} else {
		if (ddx < -400 || ddx > 400 || x <= offset + 60 || x >= offset + 580)
			attack_wanted = ATT_ATATA;
		else if (ddx < 0)
			dx = 3;
		else
			dx = -3;
	}
}

void EnemyPapaSmurf::update() {
	if (blood != 0) blood -= 1;

	updateTarget();

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			updateChoice();
			onWalk();
			break;

		case STATE_SHOOT:
			switch (attack_wanted) {
				case ATT_ATATA:
					onAtata();
					break;
			}
			break;

		case STATE_CHARRED:
		case STATE_DYING:
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
		attack_wanted = rand() % 2;
	} else {
		if (dir == FACING_LEFT) {
			pic = pbk_enemies.anim("smurf_walk_right")[3 + (anim_step % 3)];
			dx = -1;
		} else {
			pic = pbk_enemies.anim("smurf_walk_right")[(anim_step % 3)];
			dx = 1;
		}

		int x2 = x + dx;

		if (!solid_wall(x2, y) && x2 > offset + 50 && x2 < offset + 600) x = x2;
	}
}

void EnemyPapaSmurf::onWalk() {
	int x2 = x + dx;

	if (!solid_wall(x2, y) && x2 > offset + 50 && x2 < offset + 600) x = x2;

	if (dx <= 0) {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("smurf_walk_left")[4];
		else
			pic = pbk_enemies.anim("smurf_burn_left")[3];
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("smurf_burn_right")[0];
		else
			pic = pbk_enemies.anim("smurf_walk_left")[1];
	}
}
