#include "entities/enemies/enemy_papa_smurf.h"

#define ATT_ATATA 0
#define ATT_BOOM 1

EnemyPapaSmurf::EnemyPapaSmurf() : target(NULL), wait_change_target(0), dx(0), attack_wanted(ATT_BOOM) { hp = 5000; }

void EnemyPapaSmurf::updateTarget() {
	if (wait_change_target > 0 && target != NULL && !target->destroyed) {
		wait_change_target -= 1;
	} else {
		if (session().targeted_player != NULL && !session().targeted_player->destroyed) {
			target = session().targeted_player;
			wait_change_target = 200 + rnd() % 100;
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
		if (ddx < -400 || ddx > 400 || x <= level().camera.offset + 60 || x >= level().camera.offset + 580)
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
		attack_wanted = rnd() % 2;
	} else {
		if (dir == FACING_LEFT) {
			pic = assets().pics.enemies.anim("smurf_walk_right")[3 + (anim_step % 3)];
			dx = -1;
		} else {
			pic = assets().pics.enemies.anim("smurf_walk_right")[(anim_step % 3)];
			dx = 1;
		}

		int x_next = x + dx;

		if (!level().terrain.solid_wall(x_next, y) && x_next > level().camera.offset + 50 && x_next < level().camera.offset + 600) x = x_next;
	}
}

void EnemyPapaSmurf::onWalk() {
	int x_next = x + dx;

	if (!level().terrain.solid_wall(x_next, y) && x_next > level().camera.offset + 50 && x_next < level().camera.offset + 600) x = x_next;

	if (dx <= 0) {
		if (dir == FACING_LEFT)
			pic = assets().pics.enemies.anim("smurf_walk_left")[4];
		else
			pic = assets().pics.enemies.anim("smurf_burn_left")[3];
	} else {
		if (dir == FACING_LEFT)
			pic = assets().pics.enemies.anim("smurf_burn_right")[0];
		else
			pic = assets().pics.enemies.anim("smurf_walk_left")[1];
	}
}
