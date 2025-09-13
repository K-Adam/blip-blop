#include "entities/enemies/enemy_com_flying.h"

EnemyCOMFlying::EnemyCOMFlying() : speed(0), attack(true) {
	hp = 100;
	if (targeted_player != NULL) {
		y_target = targeted_player->y;
	} else {
		y_target = 100 + rand() % 200;
		;
	}
}

void EnemyCOMFlying::update() {
	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
		case ETAT_CARBONISE:
			onMeure();
			break;
	}

	updateDestroyed();
}

void EnemyCOMFlying::onAvance() {
	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if ((dir == FACING_LEFT) && (x - speed < offset || solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		attack = false;
		if (targeted_player != NULL) {
			y_target = targeted_player->y;
		} else {
			y_target = 100 + rand() % 200;
			;
		}
	} else if ((dir == FACING_RIGHT) && (x + speed > offset + 640 || solid_wall(x + speed, y))) {
		dir = FACING_LEFT;
		attack = false;
		if (targeted_player != NULL) {
			y_target = targeted_player->y;
		} else {
			y_target = 100 + rand() % 200;
			;
		}
	}

	if (attack) {
		if (y_target <= y) {
			speed = 6;
		} else if ((y_target - 80 > y) && (y > y_target - 110)) {
			y += 4;
			speed = 1;
		} else if ((y_target - 60 > y) && (y >= y_target - 80)) {
			y += 3;
			speed = 2;
		} else if ((y_target - 40 > y) && (y >= y_target - 60)) {
			y += 2;
			speed = 3;
		} else if ((y_target - 20 > y) && (y >= y_target - 40)) {
			y += 1;
			speed = 4;
		} else {
			y += 1;
			speed = 5;
		}
	} else {
		if (y < (y_target - 110)) {
			y += 5;
		} else if (y > (y_target - 100)) {
			y -= 3;
		} else {
			attack = true;
		}
	}

	if (dir == FACING_RIGHT)
		x += speed;
	else
		x -= speed;

	pic = pbk_enemies[98 + anim_step];

	colFromPic();
}

void EnemyCOMFlying::onMeure() {
	if (anim_tick == 0 && anim_step == 0) {
		sbk_level.play(6);
	}

	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0 && anim_step < 22) anim_step += 1;

	if (anim_step >= 22) {
		destroyed = true;
	} else {
		pic = pbk_enemies[76 + anim_step];
	}
}
