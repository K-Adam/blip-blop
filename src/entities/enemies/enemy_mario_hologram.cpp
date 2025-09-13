#include "entities/enemies/enemy_mario_hologram.h"
#include "entities/shots/shot_mario_fireball.h"
#include "entities/shots/shot_mario_fireball_vertical.h"

const int anim_mario_marche_droite[] = {0, 1, 2, 1};
const int anim_mario_marche_gauche[] = {3, 4, 5, 4};
const int recul_mario[] = {4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

EnemyMarioHologram::EnemyMarioHologram()
	: speed(1), etape_speed(0), attack_delay(50 + rand() % 50), wait_for_attack(0), fireball(0), wait_for_blink(0), cligno_delay(10 + rand() % 10) {
	hp = 1;
	pic = pbk_enemies[0];
}

void EnemyMarioHologram::update() {
	wait_for_blink++;
	if (wait_for_blink > cligno_delay + NB_CLIGNO) {
		decalage_cligno = 54;
		wait_for_blink = 0;
		cligno_delay = 50 + rand() % 50;
	} else if (wait_for_blink > cligno_delay) {
		decalage_cligno = 54;
	} else {
		decalage_cligno = 0;
	}

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

		case ETAT_SAUTE:
		case ETAT_TOMBE:
			onSaute();
			break;

		case ETAT_TIRE:
			switch (attack_type) {
				case 0:
					onTire();
					break;

				case 1:
					onTireverticale();
					break;
			}
			break;
	}
}

void EnemyMarioHologram::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onSaute();
	}

	if (x - speed < xmin || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > offset + 600 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}
	wait_for_attack++;
	if ((wait_for_attack >= attack_delay) && (targeted_player != NULL)) {
		if (((dir == FACING_RIGHT) && (targeted_player->x > x)) || ((dir == FACING_LEFT) && (targeted_player->x < x))) {
			anim_step = 0;
			anim_tick = 0;
			speed = 0;
			dy = -6;
			wait_for_attack = 0;
			attack_delay = 50 + rand() % 50;
			status = ETAT_TIRE;
			y_target = targeted_player->y;
			x_cible = targeted_player->x;
			attack_step = 0;
			fireball = 1;
			attack_type = rand() % 2;
			switch (attack_type) {
				case 0:
					onTire();
					break;

				case 1:
					onTireverticale();
					break;
			}
			return;
		}
	}
	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[animate(anim_mario_marche_droite, 4, 16 - (3 * speed)) + decalage_cligno];
	} else {
		marche(-speed);
		pic = pbk_enemies[animate(anim_mario_marche_gauche, 4, 16 - (3 * speed)) + decalage_cligno];
	}
	if (speed < MARIO_SPEED) {
		etape_speed++;
		if (etape_speed >= 20) {
			etape_speed = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyMarioHologram::onMeure() { destroyed = true; }

void EnemyMarioHologram::onCharge() {}

void EnemyMarioHologram::onSaute() {
	int yp;

	if (dy < 0) {
		tombe_mario();
	} else {
		fall();
	}

	// A boss off screen could not be hit
	if ((dir == FACING_LEFT) && (x - speed < xmin || solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
	}

	else if ((dir == FACING_RIGHT) && (x + speed > offset + 640 || solid_wall(x + speed, y))) {
		dir = FACING_LEFT;
		speed = 1;
	}

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		onAvance();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		if (dy < 0) {
			pic = pbk_enemies[6 + decalage_cligno];
		} else {
			pic = pbk_enemies[7 + decalage_cligno];
		}
	} else {
		x -= speed;
		if (dy < 0) {
			pic = pbk_enemies[8 + decalage_cligno];
		} else {
			pic = pbk_enemies[9 + decalage_cligno];
		}
	}

	colFromPic();
}

void EnemyMarioHologram::onCarbonise() { destroyed = true; }

void EnemyMarioHologram::render() {
	Sprite::render();
	if (fireball) {
		if (dir == FACING_RIGHT) {
			draw(x + 29, y - 33, pbk_enemies[48 + anim_step]);
		} else {
			draw(x - 29, y - 33, pbk_enemies[51 + anim_step]);
		}
	}
}

void EnemyMarioHologram::onTire() {
	if (dir == FACING_RIGHT)
		pic = pbk_enemies[10 + decalage_cligno];

	else
		pic = pbk_enemies[11 + decalage_cligno];

	if ((y - 25 < y_target) && (y_target < 400)) {
		if (y > y_target - 60) {
			dy = 2;
		} else if (y > y_target - 120) {
			dy = 3;
		} else if (y > y_target - 180) {
			dy = 4;
		} else if (y > y_target - 240) {
			dy = 5;
		} else {
			dy = 6;
		}
		y += dy;
	} else if ((y - 30 > y_target) && (y_target < 400)) {
		if (y < y_target + 60) {
			dy = -2;
		} else if (y < y_target + 120) {
			dy = -3;
		} else if (y < y_target + 180) {
			dy = -4;
		} else if (y < y_target + 240) {
			dy = -5;
		} else {
			dy = -6;
		}
		y += dy;
	} else {
		anim_tick++;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step++;
			anim_step %= 3;
		}

		attack_step++;
		if (attack_step == 60) {
			status = ETAT_AVANCE;
			anim_step = 0;
			anim_tick = 0;
			speed = 1;
			onAvance();
			return;
			fireball = 0;
			boule_de_feu();
		} else if (attack_step >= 40) {
			if (attack_step == 40) {
				fireball = 0;
				boule_de_feu();
			}
			if ((dir == FACING_RIGHT) && (x > 10)) {
				x -= recul_mario[attack_step - 40];
			} else if ((dir == FACING_LEFT) && (x < 630)) {
				x += recul_mario[attack_step - 40];
			}
		}
	}
	colFromPic();
}

void EnemyMarioHologram::onTireverticale() {
	if (dir == FACING_RIGHT)
		pic = pbk_enemies[10];

	else
		pic = pbk_enemies[11];

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	attack_step++;
	if (attack_step == 40) {
		fireball = 0;
		ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(0, x_cible);

		shot->setDir(dir);
		if (dir == FACING_RIGHT) {
			shot->x = x + 38;
		} else {
			shot->x = x - 38;
		}
		shot->y = y - 38;

		list_enemy_shots.emplace_back(shot);
	} else if (attack_step == 80) {
		status = ETAT_AVANCE;
		anim_step = 0;
		anim_tick = 0;
		speed = 1;
		onAvance();
		return;
	}
	colFromPic();
}

inline void EnemyMarioHologram::boule_de_feu() {
	if (dir == FACING_RIGHT) {
		ShotMarioFireball* shot = new ShotMarioFireball(1);

		shot->setDir(dir);
		shot->x = x + 38;
		shot->y = y - 31;

		list_enemy_shots.emplace_back(shot);
	} else {
		ShotMarioFireball* shot = new ShotMarioFireball(-1);

		shot->setDir(dir);
		shot->x = x - 38;
		shot->y = y - 31;

		list_enemy_shots.emplace_back(shot);
	}
}

void EnemyMarioHologram::tombe_mario() {
	lat_grav += 1;
	lat_grav %= LATENCE_MARIO_GRAVITE;

	if (lat_grav == 0 && dy < GRAVITY_MAX) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = platform_y(x, y + dy);

	if (ny != 0 && dy > 0)
		y = ny;
	else
		y += dy;
}

void EnemyMarioHologram::takeHit(Shot* shot) {}
