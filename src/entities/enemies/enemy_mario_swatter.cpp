#include "entities/enemies/enemy_mario_swatter.h"
#include "entities/enemies/enemy_luigi.h"
#include "entities/gore_splatter.h"
#include "level/events/event_hold_fire.h"

const int anim_mario_marche_droite[] = {75, 76, 77, 76};
const int anim_mario_marche_gauche[] = {78, 79, 80, 79};
const int anim_nabo_droite[] = {164, 165};
const int anim_nabo_gauche[] = {166, 167};
const int anim_transformation_droite[] = {75, 164};
const int anim_transformation_gauche[] = {78, 166};

EnemyMarioSwatter::EnemyMarioSwatter()
	: speed(1), etape_speed(0), attack_delay(50 + rand() % 250), wait_for_attack(0), aide_luigi(0), nabo(0), nabo_delay(0), encaissement(0) {
	hp = 10000;
	xmin = 1280;
	pic = pbk_enemies[78];
}

void EnemyMarioSwatter::update() {
	if (game_flag[0] < 1) {
		return;
	}

	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_CARBONISE:
		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_SAUTE:
		case ETAT_TOMBE:
			onSaute();
			break;

		case ETAT_TIRE:
			onCharge();
			break;
	}
}

void EnemyMarioSwatter::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onSaute();
	}

	if (x - speed < xmin || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}

	wait_for_attack++;

	if ((wait_for_attack >= attack_delay) && (targeted_player != NULL)) {
		if ((platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y)) &&
			(((dir == FACING_RIGHT) && (x < targeted_player->x)) || ((dir == FACING_LEFT) && (x > targeted_player->x)))) {
			anim_step = 0;
			anim_tick = 0;
			attack_step = 0;
			speed = 0;
			dy = 0;
			status = ETAT_TIRE;
			onCharge();
			return;
		} else {
			int dif_y = targeted_player->y - y;
			int dif_x = targeted_player->x - x;

			if ((dir == FACING_RIGHT) && (dif_y < 0) && (dif_x > 0) && (dif_y + dif_x < 0)) {
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				speed = ((dif_x * -9) / dif_y);
				dy = (int)(-9 + speed * 0.25);
				status = ETAT_SAUTE;
				lat_grav = 0;  // Otherwise jump heights vary
				onSaute();
				return;
			} else if ((dir == FACING_LEFT) && (dif_y < 0) && (dif_x < 0) && (dif_y - dif_x < 0)) {
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				speed = ((dif_x * 9) / dif_y);
				dy = (int)(-9 + speed * 0.25);
				status = ETAT_SAUTE;
				lat_grav = 0;  // Otherwise jump heights vary
				onSaute();
				return;
			}
		}
	}

	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[animate(anim_mario_marche_droite, 4, 16 - (3 * speed))];

		if (encaissement < 0) {
			if (speed > -encaissement / MULTIPLACATEUR_VITESSE_AU_SOL) {
				speed += encaissement / MULTIPLACATEUR_VITESSE_AU_SOL;
			} else {
				speed = 0;
			}
		} else {
			if (speed + encaissement / MULTIPLACATEUR_VITESSE_AU_SOL < MARIO_SPEED) {
				speed += encaissement / MULTIPLACATEUR_VITESSE_AU_SOL;
			} else {
				speed = MARIO_SPEED;
			}
		}
	} else {
		marche(-speed);
		pic = pbk_enemies[animate(anim_mario_marche_gauche, 4, 16 - (3 * speed))];

		if (encaissement > 0) {
			if (speed > encaissement / MULTIPLACATEUR_VITESSE_AU_SOL) {
				speed -= encaissement / MULTIPLACATEUR_VITESSE_AU_SOL;
			} else {
				speed = 0;
			}
		} else {
			if (speed - encaissement / MULTIPLACATEUR_VITESSE_AU_SOL < MARIO_SPEED) {
				speed -= encaissement / MULTIPLACATEUR_VITESSE_AU_SOL;
			} else {
				speed = MARIO_SPEED;
			}
		}
	}

	if (encaissement >= MULTIPLICATEUR_RECUL_SOL) {
		if (x + encaissement / MULTIPLICATEUR_RECUL_SOL < offset + 640) {
			x += encaissement / MULTIPLICATEUR_RECUL_SOL;
		}
	} else if (encaissement <= -MULTIPLICATEUR_RECUL_SOL) {
		if (x + encaissement / MULTIPLICATEUR_RECUL_SOL > xmin) {
			x += encaissement / MULTIPLICATEUR_RECUL_SOL;
		}
	}

	encaissement = 0;

	if (speed < MARIO_SPEED) {
		etape_speed++;
		if (etape_speed >= 20) {
			etape_speed = 0;
			speed++;
		}
	}

	colFromPic();
}

void EnemyMarioSwatter::onMeure() {
	if (!nabo) {
		fall();
		nabo_delay++;
		if (nabo_delay >= NABO_TRANSFORMATION) {
			speed = 1;
			nabo = true;
		} else {
			if (dir == FACING_RIGHT) {
				pic = pbk_enemies[animate(anim_transformation_droite, 2, 6)];
			} else {
				pic = pbk_enemies[animate(anim_transformation_gauche, 2, 6)];
			}
		}
	} else {
		fall();

		if (game_flag[0] >= 3) {
			if (!aide_luigi) {
				if (x - speed < xmin || solid_wall(x - speed, y)) {
					dir = FACING_RIGHT;
					speed = 2;
				} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
					dir = FACING_LEFT;
					speed = 2;
				}
			}

			if (dir == FACING_RIGHT) {
				marche(speed);
				pic = pbk_enemies[animate(anim_nabo_droite, 2, 16 - (3 * speed))];
			} else {
				marche(-speed);
				pic = pbk_enemies[animate(anim_nabo_gauche, 2, 16 - (3 * speed))];
			}
		}
	}

	if ((!aide_luigi) && (nabo)) {
		game_flag[3] = 1;
		aide_luigi = 1;
		dir = FACING_RIGHT;
		Enemy* luigi = new EnemyLuigi();

		nb_enemies_created++;

		luigi->x = 1960;
		luigi->y = y;
		luigi->dir = FACING_LEFT;
		list_enemies.emplace_back(luigi);

		EventHoldFire e;

		e.x_activation = 0;
		e.flag = 0;
		e.val = 3;
		e.doEvent();
	}
}

void EnemyMarioSwatter::onCharge() {
	speed = MARIO_CHARGE_SPEED;
	if (x - speed < xmin || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_AVANCE;
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		encaissement = 0;
		onAvance();
		return;
	} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_AVANCE;
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		encaissement = 0;
		onAvance();
		return;
	}
	attack_step += 1;
	if (attack_step < 40) {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[animate(anim_mario_marche_droite, 4, 3)];
		} else {
			pic = pbk_enemies[animate(anim_mario_marche_gauche, 4, 3)];
		}

		if (encaissement > 1000) {
			speed = 1;
			anim_step = 0;
			anim_tick = 0;
			status = ETAT_AVANCE;
			wait_for_attack = 0;
			attack_delay = 50 + rand() % 250;
			encaissement = 0;
			onAvance();
			return;
		}
	} else {
		if (dir == FACING_RIGHT) {
			marche(speed);
			pic = pbk_enemies[animate(anim_mario_marche_droite, 4, 2)];
		} else {
			marche(-speed);
			pic = pbk_enemies[animate(anim_mario_marche_gauche, 4, 2)];
		}
	}
	colFromPic();
}

void EnemyMarioSwatter::onSaute() {
	int yp;

	if (dy < 0) {
		tombe_mario();
	} else {
		fall();
	}

	// A boss off screen could not be hit
	if ((x - speed < xmin || solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
	}

	else if ((x + speed > offset + 640 || solid_wall(x + speed, y))) {
		dir = FACING_LEFT;
		speed = 1;
	}

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		if (speed > MARIO_SPEED) {
			speed = MARIO_SPEED;
		}
		onAvance();
		return;
	}

	if (!nabo) {
		if (dir == FACING_RIGHT) {
			x += speed;
			if (dy < 0) {
				pic = pbk_enemies[81];
			} else {
				pic = pbk_enemies[82];
			}

			if (encaissement != 0) {
				x += encaissement / MULTIPLICATEUR_RECUL_AERIEN;
			}

			if (encaissement < 0) {
				if (speed + encaissement / MULTIPLICATEUR_VITESSE_AERIEN < 0) {
					if (speed + encaissement / MULTIPLICATEUR_VITESSE_AERIEN < -2 * MARIO_SPEED) {
						speed = -2 * MARIO_SPEED;
					} else {
						speed += encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
					}
				} else {
					speed += encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
				}

			} else {
				if (speed + encaissement / MULTIPLICATEUR_VITESSE_AERIEN < 2 * MARIO_SPEED) {
					speed += encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
				} else {
					speed = 2 * MARIO_SPEED;
				}
			}
		} else {
			x -= speed;
			if (dy < 0) {
				pic = pbk_enemies[83];
			} else {
				pic = pbk_enemies[84];
			}

			if (encaissement != 0) {
				x += encaissement / MULTIPLICATEUR_RECUL_AERIEN;
			}

			if (encaissement > 0) {
				if (speed - encaissement / MULTIPLICATEUR_VITESSE_AERIEN < 0) {
					if (speed - encaissement / MULTIPLICATEUR_VITESSE_AERIEN < -2 * MARIO_SPEED) {
						speed = -2 * MARIO_SPEED;
					} else {
						speed -= encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
					}
				} else {
					speed -= encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
				}

			} else {
				if (speed - encaissement / MULTIPLICATEUR_VITESSE_AERIEN < 2 * MARIO_SPEED) {
					speed -= encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
				} else {
					speed = 2 * MARIO_SPEED;
				}
			}
		}
	} else {
		if (dir == FACING_RIGHT) {
			x += speed;
			pic = pbk_enemies[animate(anim_nabo_droite, 2, 16 - (3 * speed))];
		} else {
			x -= speed;
			pic = pbk_enemies[animate(anim_nabo_gauche, 2, 16 - (3 * speed))];
		}
	}

	encaissement = 0;
	colFromPic();
}

void EnemyMarioSwatter::onCarbonise() { destroyed = true; }

void EnemyMarioSwatter::tombe_mario() {
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

void EnemyMarioSwatter::takeHit(Shot* shot) {
	for (int i = 0; i < 4; i++) {
		GoreSplatter* s;
		if ((shot->dir >= 2) && (shot->dir <= 6)) {
			s = new GoreSplatter(rand() % 6, -2 - rand() % 6);
		} else if ((shot->dir >= 10) && (shot->dir <= 14)) {
			s = new GoreSplatter(-rand() % 5 - 1, -2 - rand() % 6);
		} else {
			s = new GoreSplatter(rand() % 11 - 5, -2 - rand() % 6);
		}
		s->x = x + rand() % 11 - 5;
		s->y = y - rand() % 30 - 10;

		list_splatters.emplace_back(s);
	}

	if ((shot->dir >= 2) && (shot->dir <= 6)) {
		encaissement += shot->damage();
	} else if ((shot->dir >= 10) && (shot->dir <= 14)) {
		encaissement -= shot->damage();
	}

	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);
}
