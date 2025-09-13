#include "entities/enemies/enemy_mario.h"
#include "entities/shots/shot_mario_fireball.h"
#include "entities/shots/shot_mario_fireball_vertical.h"
#include "entities/enemies/enemy_mario_hologram.h"

const int anim_mario_marche_droite[] = {0, 1, 2, 1};
const int anim_mario_marche_gauche[] = {3, 4, 5, 4};
const int recul_mario[] = {4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

EnemyMario::EnemyMario() : speed(1), etape_speed(0), attack_delay(50 + rand() % 50), wait_for_attack(0), fireball(0), hologramme(1), nb_pluie(0) {
	hp = 50000;
	pic = pbk_enemies[3];
}

void EnemyMario::update() {
	xmin = offset + 40;

	if (game_flag[0] == 0) {
		fall();
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
			switch (attack_type) {
				case 0:
					onTirehorizontale();
					break;

				case 1:
					onTireverticale();
					break;

				case 2:
					onRafalehorizontale();
					break;

				case 3:
					onRafaleverticale();
					break;

				case 4:
					onRafaleverticaleinverser();
					break;

				case 5:
					onRafalehorizontaledouble();
					break;

				case 6:
					onRafaleverticaledouble();
					break;

				case 7:
					onTirecross();
					break;

				case 8:
					onRafalehorizontaleconstante();
					break;

				case 9:
					onRafaleverticaleconstante();
					break;

				case 10:
					onRafalecross();
					break;

				case 11:
					onPluiedefeu();
					break;

				case 12:
					onRafaleverticalechercheuse();
					break;

				case 13:
					onRafalehorizontalechercheuse();
					break;

				case 14:
					onRafalecrosschercheuse();
					break;

				case 15:
					onRafaleberserker();
					break;
			}
			break;
	}
}

void EnemyMario::onAvance() {
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
			status = ETAT_TIRE;
			y_target = targeted_player->y;
			x_cible = targeted_player->x;
			attack_step = 0;
			attack_ss_etape = 0;
			attack_phase = 0;

			if (hp < MARIO_PV_SUR_CRITIQUE) {
				attack_type = rand() % 5 + 11;
				attack_delay = 30 + rand() % 30;
			} else if (hp < MARIO_PV_CRITIQUE) {
				attack_type = rand() % 6 + 5;
				attack_delay = 50 + rand() % 50;
			} else if (hp < MARIO_PV_SOUS_CRITIQUE) {
				attack_type = rand() % 5 + 2;
				attack_delay = 100 + rand() % 50;
			} else {
				attack_type = rand() % 5;
				attack_delay = 150 + rand() % 50;
			}
			switch (attack_type) {
				case 0:
					fireball = true;
					onTirehorizontale();
					break;

				case 1:
					fireball = true;
					onTireverticale();
					break;

				case 2:
					fireball = true;
					onRafalehorizontale();
					break;

				case 3:
					fireball = true;
					onRafaleverticale();
					break;

				case 4:
					if (dir == FACING_RIGHT) {
						attack_step = 1 + (offset + 640 - x) / 100;
					} else {
						attack_step = 1 + (x - offset) / 100;
					}
					fireball = true;
					onRafaleverticaleinverser();
					break;

				case 5:
					fireball = true;
					onRafalehorizontaledouble();
					break;

				case 6:
					fireball = true;
					onRafaleverticaledouble();
					break;

				case 7:
					fireball = true;
					onTirecross();
					break;

				case 8:
					nb_pluie = 4 + rand() % 7;
					fireball = true;
					onRafalehorizontaleconstante();
					break;

				case 9:
					fireball = true;
					nb_pluie = 3 + rand() % 5;
					onRafaleverticaleconstante();
					break;

				case 10:
					fireball = true;
					nb_pluie = 4 + rand() % 5;
					onRafalecross();
					break;

				case 11:
					speed = 5;
					nb_pluie = 3 + rand() % 4;
					onPluiedefeu();
					break;

				case 12:
					fireball = true;
					nb_pluie = 3 + rand() % 3;
					onRafaleverticalechercheuse();
					break;

				case 13:
					fireball = true;
					nb_pluie = 3 + rand() % 3;
					onRafalehorizontalechercheuse();
					break;

				case 14:
					fireball = true;
					nb_pluie = 3 + rand() % 4;
					onRafalecrosschercheuse();
					break;

				case 15:
					speed = 5;
					fireball = false;
					onRafaleberserker();
					break;
			}
			return;
		}
	}
	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[animate(anim_mario_marche_droite, 4, 16 - (3 * speed))];
	} else {
		marche(-speed);
		pic = pbk_enemies[animate(anim_mario_marche_gauche, 4, 16 - (3 * speed))];
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

void EnemyMario::onMeure() { game_flag[0] = 7; }

void EnemyMario::onCharge() {}

void EnemyMario::onSaute() {
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
			pic = pbk_enemies[6];
		} else {
			pic = pbk_enemies[7];
		}
	} else {
		x -= speed;
		if (dy < 0) {
			pic = pbk_enemies[8];
		} else {
			pic = pbk_enemies[9];
		}
	}

	colFromPic();
}

void EnemyMario::onCarbonise() { destroyed = true; }

void EnemyMario::render() {
	Sprite::render();
	if ((fireball) && (anim_step < 3)) {
		if (dir == FACING_RIGHT) {
			draw(x + 29, y - 33, pbk_enemies[48 + anim_step]);
		} else {
			draw(x - 29, y - 33, pbk_enemies[51 + anim_step]);
		}
	}
}

void EnemyMario::onTirehorizontale() {
	if (dir == FACING_RIGHT)
		pic = pbk_enemies[10];

	else
		pic = pbk_enemies[11];

	if ((y - 25 < y_target) && (y < 444)) {
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
		if (y + dy > 444) {
			y = 444;
			dy = 0;
		} else {
			y += dy;
		}
	} else if ((y - 30 > y_target) && (y <= 444)) {
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
		} else if (attack_step >= 40) {
			if (attack_step == 40) {
				fireball = 0;
				boule_de_feu(4);
			}
			if ((dir == FACING_RIGHT) && (x > offset + 10)) {
				x -= recul_mario[attack_step - 40];
			} else if ((dir == FACING_LEFT) && (x < offset + 630)) {
				x += recul_mario[attack_step - 40];
			}
		}
	}
	colFromPic();
}

void EnemyMario::onTireverticale() {
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
	} else if (attack_step == 60) {
		status = ETAT_AVANCE;
		anim_step = 0;
		anim_tick = 0;
		speed = 1;
		onAvance();
		return;
	}
	colFromPic();
}

void EnemyMario::onTirecross() {
	switch (attack_phase) {
		case 0:
			onTirehorizontale();
			if (attack_step > 0) {
				attack_phase = 1;
				attack_step = 20;
				attack_ss_etape = 0;
				fireball = true;
			}
			break;

		case 1:
			onTireverticale();
			if (attack_step > 40) {
				attack_phase = 2;
				attack_step = 0;
				attack_ss_etape = 0;
				fireball = true;
			}
			break;

		case 2:
			onTirehorizontale();
			break;
	}
}

void EnemyMario::onRafalehorizontale() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[10];
	} else {
		pic = pbk_enemies[11];
	}

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if ((y >= 140) && (y <= 145)) {
		boule_de_feu(1);
	} else if ((y >= 240) && (y <= 245)) {
		boule_de_feu(1);
	} else if ((y >= 340) && (y <= 345)) {
		boule_de_feu(1);
	} else if ((y >= 440) && (y <= 455)) {
		boule_de_feu(1);
	}

	if (((dy < 0) && (y < 100)) || ((dy > 0) && (y > 435))) {
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_TOMBE;
		fireball = false;
		onSaute();

		if (dy > 3) {
			dy = 3;
		}
		return;
	}
	y += dy;
	colFromPic();
}

void EnemyMario::onRafalehorizontaledouble() {
	switch (attack_phase) {
		case 0:
			if (y + dy < 100) {
				attack_phase = 1;
				fireball = false;
			} else {
				onRafalehorizontale();
			}
			break;

		case 1:
			fall();
			if ((dy > 0) && (y < 0)) {
				attack_phase = 2;
				fireball = true;
				dy = 6;
			}
			colFromPic();
			break;

		case 2:
			onRafalehorizontale();
			break;
	}
}

void EnemyMario::onRafalehorizontaleconstante() {
	onTirehorizontale();
	if ((nb_pluie > attack_phase) && (attack_step > 50)) {
		attack_phase++;
		attack_step = 30;
		attack_ss_etape = 0;
	} else if ((nb_pluie > attack_phase) && (attack_step == 40)) {
		fireball = true;
	}
}

void EnemyMario::onRafalehorizontalechercheuse() {
	onTirehorizontale();
	if ((nb_pluie > attack_phase) && (attack_step > 50)) {
		if ((targeted_player != NULL) && ((targeted_player->x > x && dir == FACING_RIGHT) || (targeted_player->x < x && dir == FACING_LEFT))) {
			attack_phase++;
			attack_step = 30;
			attack_ss_etape = 0;
			y_target = targeted_player->y;
		} else {
			fireball = false;
		}
	} else if ((nb_pluie > attack_phase) && (attack_step == 40)) {
		fireball = true;
	}
}

void EnemyMario::onRafaleverticale() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[10];
	} else {
		pic = pbk_enemies[11];
	}

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	attack_ss_etape++;
	attack_ss_etape %= 6;
	if (attack_ss_etape == 0) {
		attack_step++;
		if (dir == FACING_RIGHT) {
			x_cible = x + attack_step * 100 + rand() % 20;
			if (x_cible > offset + 720) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = ETAT_AVANCE;
				fireball = false;
				onAvance();
				return;
			}
			ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(attack_step, x_cible);

			shot->setDir(dir);
			shot->x = x + 38;
			shot->y = y - 38;

			list_enemy_shots.emplace_back(shot);
		} else {
			int x_cible = x - attack_step * 100 - rand() % 20;
			if (x_cible < offset - 100) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = ETAT_AVANCE;
				fireball = false;
				onAvance();
				return;
			}
			ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(-attack_step, x_cible);

			shot->setDir(dir);
			shot->x = x - 38;
			shot->y = y - 38;

			list_enemy_shots.emplace_back(shot);
		}
	}
	colFromPic();
}

void EnemyMario::onRafaleverticalechercheuse() {
	onTireverticale();
	if ((nb_pluie > attack_phase) && (attack_step == 40) && (targeted_player != NULL)) {
		attack_step = 10;
		attack_ss_etape = 0;
		fireball = true;
		attack_phase++;
		x_cible = targeted_player->x;
	}
}

void EnemyMario::onRafaleverticaleconstante() {
	onTireverticale();
	if ((nb_pluie > attack_phase) && (attack_step == 40)) {
		attack_step = 25;
		attack_ss_etape = 0;
		fireball = true;
		attack_phase++;
	}
}

void EnemyMario::onRafaleverticaledouble() {
	switch (attack_phase) {
		case 0:
			if ((dir == FACING_RIGHT) && (x + (attack_step + 1) * 100 + 20 > offset + 620)) {
				attack_phase = 1;
				attack_ss_etape = 0;
				attack_step = 1 + (offset + 640 - x) / 100;
			} else if ((dir == FACING_LEFT) && (x - (attack_step + 1) * 100 - 20 < offset + 20)) {
				attack_phase = 1;
				attack_ss_etape = 0;
				attack_step = 1 + (x - offset) / 100;
			} else {
				onRafaleverticale();
			}
			break;

		case 1:
			onRafaleverticaleinverser();
			break;
	}
}

void EnemyMario::onRafaleverticaleinverser() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[10];
	} else {
		pic = pbk_enemies[11];
	}

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	attack_ss_etape++;
	attack_ss_etape %= 6;
	if (attack_ss_etape == 0) {
		attack_step--;
		if (dir == FACING_RIGHT) {
			x_cible = offset + attack_step * 100 + rand() % 20;
			if (x_cible < x + 20) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = ETAT_AVANCE;
				fireball = false;
				onAvance();
				return;
			}
			ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(attack_step, x_cible);

			shot->setDir(dir);
			shot->x = x + 38;
			shot->y = y - 38;

			list_enemy_shots.emplace_back(shot);
		} else {
			int x_cible = offset + 640 - attack_step * 100 - rand() % 20;
			if (x_cible > x - 20) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = ETAT_AVANCE;
				fireball = false;
				onAvance();
				return;
			}
			ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(-attack_step, x_cible);

			shot->setDir(dir);
			shot->x = x - 38;
			shot->y = y - 38;

			list_enemy_shots.emplace_back(shot);
		}
	}
	colFromPic();
}

void EnemyMario::onRafalecrosschercheuse() {
	if (attack_phase % 2 == 0) {
		onTireverticale();
		if ((nb_pluie > attack_phase) && (attack_step == 40) && (targeted_player != NULL)) {
			attack_step = 30;
			attack_ss_etape = 0;
			fireball = true;
			attack_phase++;
			y_target = targeted_player->y;
		}
	} else {
		onTirehorizontale();
		if ((nb_pluie > attack_phase) && (attack_step > 50)) {
			if ((targeted_player != NULL) && ((targeted_player->x > x && dir == FACING_RIGHT) || (targeted_player->x < x && dir == FACING_LEFT))) {
				attack_phase++;
				attack_step = 30;
				attack_ss_etape = 0;
				x_cible = targeted_player->x;
			} else {
				fireball = false;
			}
		} else if ((nb_pluie > attack_phase) && (attack_step == 40)) {
			fireball = true;
		}
	}
}

void EnemyMario::onRafalecross() {
	if (attack_phase % 2 == 0) {
		onTireverticale();
		if ((nb_pluie > attack_phase) && (attack_step == 40)) {
			attack_step = 35;
			attack_ss_etape = 0;
			fireball = true;
			attack_phase++;
		}
	} else {
		onTirehorizontale();
		if ((nb_pluie > attack_phase) && (attack_step > 50)) {
			attack_phase++;
			attack_step = 35;
			attack_ss_etape = 0;
		} else if ((nb_pluie > attack_phase) && (attack_step == 40)) {
			fireball = true;
		}
	}
}

void EnemyMario::onRafaleberserker() {
	switch (attack_phase) {
		case 0:
			if (dir == FACING_RIGHT) {
				marche(speed);
				pic = pbk_enemies[animate(anim_mario_marche_droite, 4, 16 - (3 * speed))];
				if (x > offset + 600) {
					fireball = true;
					attack_phase = 1;
					speed = 1;
					dir = FACING_LEFT;
				}
			} else {
				marche(-speed);
				pic = pbk_enemies[animate(anim_mario_marche_gauche, 4, 16 - (3 * speed))];
				if (x < offset + 40) {
					fireball = true;
					attack_phase = 1;
					speed = 1;
					dir = FACING_RIGHT;
				}
			}
			colFromPic();
			break;

		case 1:
			if ((dir == FACING_RIGHT) && (x + (attack_step + 1) * 100 + 20 > offset + 620)) {
				attack_phase = 2;
			} else if ((dir == FACING_LEFT) && (x - (attack_step + 1) * 100 - 20 < offset + 20)) {
				attack_phase = 2;
			} else {
				onRafaleverticale();
			}

			break;

		case 2:
			if (y + dy < 100) {
				attack_phase = 3;
				fireball = false;
			} else {
				onRafalehorizontale();
			}
			break;

		case 3:
			fall();
			if (y < 0) {
				if (dir == FACING_RIGHT) {
					dir = FACING_LEFT;
					x = offset + 600;
				} else {
					dir = FACING_RIGHT;
					x = offset + 40;
				}
				dy = 6;
				attack_phase = 4;
				fireball = true;
			}
			colFromPic();
			break;

		case 4:
			if ((y + dy) > 435) {
				attack_phase = 5;
				if (dir == FACING_RIGHT) {
					attack_step = 1 + (640 - x) / 100;
				} else {
					attack_step = 1 + x / 100;
				}
				attack_ss_etape = 0;
				anim_step = 0;
				anim_tick = 0;
				dy = 0;
			} else {
				onRafalehorizontale();
			}
			break;

		case 5:
			if ((dir == FACING_RIGHT) && (x + (attack_step - 1) * 100 - 20 < x)) {
				attack_phase = 6;
				attack_step = 0;
				fireball = false;
			} else if ((dir == FACING_LEFT) && (x - (attack_step - 1) * 100 + 20 > x)) {
				attack_phase = 6;
				attack_step = 0;
				fireball = false;
			} else {
				onRafaleverticaleinverser();
			}
			break;

		case 6:
			attack_step++;
			if (attack_step >= 160) {
				status = ETAT_AVANCE;
				anim_step = 0;
				anim_tick = 0;
				speed = 1;
				onAvance();
				return;
			}
			break;
	}
}

void EnemyMario::onPluiedefeu() {
	if (attack_phase == 0) {
		if (dir == FACING_RIGHT) {
			marche(speed);
			pic = pbk_enemies[animate(anim_mario_marche_droite, 4, 16 - (3 * speed))];
			if (x > offset + 600) {
				fireball = true;
				attack_phase = 1;
				speed = 1;
				dir = FACING_LEFT;
			}
		} else {
			marche(-speed);
			pic = pbk_enemies[animate(anim_mario_marche_gauche, 4, 16 - (3 * speed))];
			if (x < offset + 40) {
				fireball = true;
				attack_phase = 1;
				speed = 1;
				dir = FACING_RIGHT;
			}
		}
		colFromPic();
	} else {
		if (attack_phase < nb_pluie) {
			if (attack_phase % 2 == 0) {
				if ((dir == FACING_RIGHT) && (x + (attack_step - 1) * 100 - 20 < x)) {
					attack_phase++;
					attack_ss_etape = 0;
					attack_step = 0;
				} else if ((dir == FACING_LEFT) && (x - (attack_step - 1) * 100 + 20 > x)) {
					attack_phase++;
					attack_ss_etape = 0;
					attack_step = 0;
				} else {
					onRafaleverticaleinverser();
				}
			} else {
				if ((dir == FACING_RIGHT) && (x + (attack_step + 1) * 100 + 20 > 620)) {
					attack_phase++;
					attack_ss_etape = 0;
					attack_step = 1 + (offset + 640 - x) / 100;
				} else if ((dir == FACING_LEFT) && (x - (attack_step + 1) * 100 - 20 < 20)) {
					attack_phase++;
					attack_ss_etape = 0;
					attack_step = 1 + (x - offset) / 100;
				} else {
					onRafaleverticale();
				}
			}
		} else {
			anim_step++;
			fireball = false;
			if (anim_step >= 60 + nb_pluie * 30) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = ETAT_AVANCE;
				onAvance();
				return;
			}
		}
	}
}

inline void EnemyMario::boule_de_feu(int vitesse) {
	if (dir == FACING_RIGHT) {
		ShotMarioFireball* shot = new ShotMarioFireball(vitesse);

		shot->setDir(dir);
		shot->x = x + 38;
		shot->y = y - 31;

		list_enemy_shots.emplace_back(shot);
	} else {
		ShotMarioFireball* shot = new ShotMarioFireball(-vitesse);

		shot->setDir(dir);
		shot->x = x - 38;
		shot->y = y - 31;

		list_enemy_shots.emplace_back(shot);
	}
}

void EnemyMario::tombe_mario() {
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

void EnemyMario::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);
}
