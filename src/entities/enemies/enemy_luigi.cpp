#include "entities/enemies/enemy_luigi.h"
#include "entities/shots/shot_plunger.h"
#include "entities/gore_splatter.h"
#include "entities/enemies/enemy_sonic.h"
#include "entities/enemies/enemy_pacman_car.h"
#include "level/events/event_hold_fire.h"

const int anim_luigi_marche_droite[] = {0, 1, 2, 1};
const int anim_luigi_marche_gauche[] = {3, 4, 5, 4};
const int anim_luigi_attaque_droite[] = {10, 11, 12, 11};
const int anim_luigi_attaque_gauche[] = {13, 14, 15, 14};
const int anim_luigi_marche_mort_droite[] = {205, 206, 207, 206};
const int anim_luigi_marche_mort_gauche[] = {213, 214, 215, 214};
const int anim_luigi_mort[] = {208, 209, 210, 211, 212, 211, 210, 209};

EnemyLuigi::EnemyLuigi() : speed(1), etape_speed(0), attack_delay(50 + rand() % 250), wait_for_attack(0), encaissement(0) {
	hp = 10000;
	xmin = offset + 20;
}

void EnemyLuigi::update() {
	if (game_flag[0] == 7) destroyed = true;

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
					break;
				case 1:
					onAttack();
					break;
			}
			break;
	}
}

void EnemyLuigi::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onSaute();
	}

	if (x - speed < (xmin + 20) || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > offset + 600 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}

	if (game_flag[0] >= 3) {
		wait_for_attack++;
	} else if (x < offset + 600) {
		game_flag[0] = 2;
	}

	if ((wait_for_attack >= attack_delay) && (targeted_player != NULL)) {
		int dif_y = targeted_player->y - y;
		int dif_x = targeted_player->x - x;

		if (dif_y == 0) {
			dif_y = 1;
		}
		int coef_direct = dif_x / dif_y;
		if ((dir == FACING_RIGHT) && (dif_y < 0) && (dif_x > 0) && (dif_y + dif_x < 0)) {
			wait_for_attack = 0;
			attack_delay = 10 + rand() % 50;
			speed = (int)((dif_x * -9) / dif_y);
			dy = (int)(-9 + speed * 0.25);
			anim_step = 0;
			anim_tick = 0;
			status = ETAT_SAUTE;
			lat_grav = 0;  // Otherwise jump heights vary
			onSaute();
			return;

		} else if ((dir == FACING_LEFT) && (dif_y < 0) && (dif_x < 0) && (dif_y - dif_x < 0)) {
			wait_for_attack = 0;
			attack_delay = 10 + rand() % 50;
			speed = (int)((dif_x * 9) / dif_y);
			dy = (int)(-9 + speed * 0.25);
			anim_step = 0;
			anim_tick = 0;
			status = ETAT_SAUTE;
			lat_grav = 0;  // Otherwise jump heights vary
			onSaute();
			return;
		} else if (hp >= 1000) {
			if ((dir == FACING_RIGHT) && (dif_x > 0) && ((coef_direct <= -2) || (coef_direct >= 2))) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				attack_type = 1;
				anim_tick = 0;
				anim_step = 0;
				speed = 0;
				ss_etape_attack = 0;
				etape_attack = 0;
				status = ETAT_TIRE;
				nb_tir = 0;
				sbk_level.play(9);
				onAttack();
				return;
			} else if ((dir == FACING_LEFT) && (dif_x < 0) && ((coef_direct <= -2) || (coef_direct >= 2))) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				attack_type = 1;
				anim_tick = 0;
				anim_step = 0;
				speed = 0;
				ss_etape_attack = 0;
				etape_attack = 0;
				status = ETAT_TIRE;
				nb_tir = 0;
				sbk_level.play(9);
				onAttack();
				return;
			}
		}
	}
	if (dir == FACING_RIGHT) {
		if (speed > 6)
			speed = 6;
		else if (speed < -6)
			speed = -6;

		marche(speed);
		pic = pbk_enemies[animate(anim_luigi_marche_droite, 4, 14 - (2 * speed))];

		if (encaissement < 0) {
			if (speed > -encaissement / MULTIPLACATEUR_VITESSE_AU_SOL) {
				speed += encaissement / MULTIPLACATEUR_VITESSE_AU_SOL;
			} else {
				speed = 0;
			}
		} else {
			if (speed + encaissement / MULTIPLACATEUR_VITESSE_AU_SOL < LUIGI_SPEED) {
				speed += encaissement / MULTIPLACATEUR_VITESSE_AU_SOL;
			} else {
				speed = LUIGI_SPEED;
			}
		}
	} else {
		if (speed > 6)
			speed = 6;
		else if (speed < -6)
			speed = -6;

		marche(-speed);
		pic = pbk_enemies[animate(anim_luigi_marche_gauche, 4, 14 - (2 * speed))];

		if (encaissement > 0) {
			if (speed > encaissement / MULTIPLACATEUR_VITESSE_AU_SOL) {
				speed -= encaissement / MULTIPLACATEUR_VITESSE_AU_SOL;
			} else {
				speed = 0;
			}
		} else {
			if (speed - encaissement / MULTIPLACATEUR_VITESSE_AU_SOL < LUIGI_SPEED) {
				speed -= encaissement / MULTIPLACATEUR_VITESSE_AU_SOL;
			} else {
				speed = LUIGI_SPEED;
			}
		}
	}

	if (encaissement >= MULTIPLICATEUR_RECUL_SOL) {
		if (x + encaissement / MULTIPLICATEUR_RECUL_SOL < offset + 620) {
			x += encaissement / MULTIPLICATEUR_RECUL_SOL;
		}
	} else if (encaissement <= -MULTIPLICATEUR_RECUL_SOL) {
		if (x + encaissement / MULTIPLICATEUR_RECUL_SOL > xmin) {
			x += encaissement / MULTIPLICATEUR_RECUL_SOL;
		}
	}

	encaissement = 0;

	if (speed < LUIGI_SPEED) {
		etape_speed++;
		if (etape_speed >= 20) {
			etape_speed = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyLuigi::onMeure() {
	if (game_flag[1] == 0) {
		fall();
		if (x - 3 < xmin || solid_wall(x - 3, y)) {
			dir = FACING_RIGHT;
		} else if (x + 3 > offset + 640 || solid_wall(x + 3, y)) {
			dir = FACING_LEFT;
		}
		if (dir == FACING_RIGHT) {
			marche(3);
			pic = pbk_enemies[animate(anim_luigi_marche_mort_droite, 4, 8)];
		} else {
			marche(-3);
			pic = pbk_enemies[animate(anim_luigi_marche_mort_gauche, 4, 8)];
		}
		if ((platform_y2(x, y) == 0) && (x > 1500) && (x < 1550)) {
			dir = FACING_LEFT;
			game_flag[1] = 1;
			attack_delay = 0;
		}
	} else if (game_flag[1] == 1) {
		attack_delay++;
		if (attack_delay >= EXPLOSE_DELAY) {
			game_flag[1] = 2;
			dir = FACING_RIGHT;
		} else {
			pic = pbk_enemies[animate(anim_luigi_mort, 8, 6)];
		}
	} else if (game_flag[1] == 2) {
		marche(3);
		pic = pbk_enemies[animate(anim_luigi_marche_mort_droite, 4, 8)];
		if (x > offset + 680) {
			game_flag[1] = 3;
			attack_delay = 0;
		}
	} else if (game_flag[1] == 3) {
		attack_delay++;
		if (attack_delay >= NOMBRE_GICLURES) {
			Enemy* sonic = new EnemyPacmanCar();

			sonic->x = 1960;
			sonic->y = y;
			sonic->dir = FACING_LEFT;
			list_enemies.emplace_back(sonic);

			nb_enemies_created++;

			sonic = new EnemySonic();

			sonic->x = 1960;
			sonic->y = y;
			sonic->dir = FACING_LEFT;
			list_enemies.emplace_back(sonic);
			destroyed = true;

		} else {
			int i;
			for (i = 0; i < 16; i++) {
				Sprite* s = new GoreSplatter(-rand() % 10 - 2, -1 - rand() % 10);
				s->x = x + rand() % 20 - 10;
				s->y = y - rand() % 60 - 10;
				list_splatters.emplace_back(s);
			}
		}
	}
}

void EnemyLuigi::onAttack() {
	nb_tir++;
	if (nb_tir > 275) {
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_AVANCE;
		onAvance();
		return;
	}

	if (nb_tir < 50) {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[10];
		else
			pic = pbk_enemies[13];

		colFromPic();
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[animate(anim_luigi_attaque_droite, 4, 2)];
		ss_etape_attack += 1;
		ss_etape_attack %= 4;
		if (ss_etape_attack == 0) {
			etape_attack += 1;
			etape_attack %= 6;
			ShotPlunger* tir1;
			ShotPlunger* tir2;

			switch (etape_attack) {
				case 0:
					tir1 = new ShotPlunger(19, 12, 0);
					tir2 = new ShotPlunger(19, 12, 0);

					tir1->setDir(dir);
					tir1->x = x + 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x + 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);

					break;
				case 1:
					tir1 = new ShotPlunger(18, 11, 1);
					tir2 = new ShotPlunger(20, 11, -1);

					tir1->setDir(dir);
					tir1->x = x + 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x + 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
				case 2:
					tir1 = new ShotPlunger(17, 10, 2);
					tir2 = new ShotPlunger(21, 10, -2);

					tir1->setDir(dir);
					tir1->x = x + 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x + 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
				case 3:
					tir1 = new ShotPlunger(16, 9, 3);
					tir2 = new ShotPlunger(22, 9, -3);

					tir1->setDir(dir);
					tir1->x = x + 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x + 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
				case 4:
					tir1 = new ShotPlunger(17, 10, 2);
					tir2 = new ShotPlunger(21, 10, -2);

					tir1->setDir(dir);
					tir1->x = x + 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x + 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
				case 5:
					tir1 = new ShotPlunger(18, 11, 1);
					tir2 = new ShotPlunger(20, 11, -1);

					tir1->setDir(dir);
					tir1->x = x + 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x + 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
			}

			if (x > offset + 30) {
				x -= 1;
			}
		}
	} else {
		pic = pbk_enemies[animate(anim_luigi_attaque_gauche, 4, 2)];
		ss_etape_attack += 1;
		ss_etape_attack %= 4;
		if (ss_etape_attack == 0) {
			etape_attack += 1;
			etape_attack %= 6;
			ShotPlunger* tir1;
			ShotPlunger* tir2;

			switch (etape_attack) {
				case 0:
					tir1 = new ShotPlunger(26, -12, 0);
					tir2 = new ShotPlunger(26, -12, 0);

					tir1->setDir(dir);
					tir1->x = x - 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x - 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);

					break;
				case 1:
					tir1 = new ShotPlunger(25, -11, 1);
					tir2 = new ShotPlunger(27, -11, -1);

					tir1->setDir(dir);
					tir1->x = x - 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x - 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
				case 2:
					tir1 = new ShotPlunger(24, -10, 2);
					tir2 = new ShotPlunger(28, -10, -2);

					tir1->setDir(dir);
					tir1->x = x - 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x - 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
				case 3:
					tir1 = new ShotPlunger(23, -9, 3);
					tir2 = new ShotPlunger(29, -9, -3);

					tir1->setDir(dir);
					tir1->x = x - 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x - 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
				case 4:
					tir1 = new ShotPlunger(24, -10, 2);
					tir2 = new ShotPlunger(28, -10, -2);

					tir1->setDir(dir);
					tir1->x = x - 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x - 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
				case 5:
					tir1 = new ShotPlunger(25, -11, 1);
					tir2 = new ShotPlunger(27, -11, -1);

					tir1->setDir(dir);
					tir1->x = x - 23;
					tir1->y = y - 13;

					tir2->setDir(dir);
					tir2->x = x - 23;
					tir2->y = y - 33;

					list_enemy_shots.emplace_back(tir1);
					list_enemy_shots.emplace_back(tir2);
					break;
			}

			if (x < offset + 610) {
				x += 1;
			}
		}
	}
	colFromPic();
}

void EnemyLuigi::onSaute() {
	int yp;

	if (dy < 0) {
		tombe_luigi();
	} else {
		fall();
	}

	// A boss off screen could not be hit
	if ((x - speed < xmin || solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
	}

	else if ((x + speed > offset + 600 || solid_wall(x + speed, y))) {
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
		if (x + speed < offset + 620) x += speed;

		if (dy < 0) {
			pic = pbk_enemies[6];
		} else {
			pic = pbk_enemies[7];
		}

		if (encaissement != 0) {
			x += encaissement / MULTIPLICATEUR_RECUL_AERIEN;
		}

		if (encaissement < 0) {
			if (speed + encaissement / MULTIPLICATEUR_VITESSE_AERIEN < 0) {
				if (speed + encaissement / MULTIPLICATEUR_VITESSE_AERIEN < -2 * LUIGI_SPEED) {
					speed = -2 * LUIGI_SPEED;
				} else {
					speed += encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
				}
			} else {
				speed += encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
			}

		} else {
			if (speed + encaissement / MULTIPLICATEUR_VITESSE_AERIEN < 2 * LUIGI_SPEED) {
				speed += encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
			} else {
				speed = 2 * LUIGI_SPEED;
			}
		}
	} else {
		if (x - speed > xmin) x -= speed;

		if (dy < 0) {
			pic = pbk_enemies[8];
		} else {
			pic = pbk_enemies[9];
		}

		if (encaissement != 0) {
			x += encaissement / MULTIPLICATEUR_RECUL_AERIEN;
		}

		if (encaissement > 0) {
			if (speed - encaissement / MULTIPLICATEUR_VITESSE_AERIEN < 0) {
				if (speed - encaissement / MULTIPLICATEUR_VITESSE_AERIEN < -2 * LUIGI_SPEED) {
					speed = -2 * LUIGI_SPEED;
				} else {
					speed -= encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
				}
			} else {
				speed -= encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
			}

		} else {
			if (speed - encaissement / MULTIPLICATEUR_VITESSE_AERIEN < 2 * LUIGI_SPEED) {
				speed -= encaissement / MULTIPLICATEUR_VITESSE_AERIEN;
			} else {
				speed = 2 * LUIGI_SPEED;
			}
		}
	}

	encaissement = 0;

	colFromPic();
}

void EnemyLuigi::onCarbonise() { destroyed = true; }

void EnemyLuigi::tombe_luigi() {
	lat_grav += 1;
	lat_grav %= LATENCE_LUIGI_GRAVITE;

	if (lat_grav == 0 && dy < GRAVITY_MAX) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = platform_y(x, y + dy);

	if (ny != 0 && dy > 0)
		y = ny;
	else
		y += dy;
}

void EnemyLuigi::takeHit(Shot* shot) {
	Sprite* s;

	for (int i = 0; i < 4; i++) {
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

	if (hp <= 0) {
		EventHoldFire e;

		e.x_activation = 0;
		e.flag = 0;
		e.val = 5;
		e.doEvent();
	}
}
