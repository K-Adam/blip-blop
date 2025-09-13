#include "entities/enemies/enemy_rayman.h"
#include "entities/shots/shot_rayman_fist.h"
#include "entities/piece_rayman.h"
#include "entities/enemies/enemy_pikachu_hero.h"
#include "entities/enemies/enemy_bulbizarre_hero.h"
#include "entities/enemies/enemy_com_hero.h"
#include "entities/enemies/enemy_ember_hero.h"
#include "entities/piece_rayman_head.h"

EnemyRayman::EnemyRayman() : dorkemon(4), attack_delay(100 + rand() % 250), wait_for_attack(0), tir_poing(NULL), poing1(1), poing2(1), pieds(1) {
	hp = 15000;
	xmin = 10428;
	tresor = 0;
}

void EnemyRayman::update() {
	if (blood > 0) blood -= 1;

	if (x < 10930 && game_flag[3] == 0) {
		game_flag[3] = 1;
	}

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			if (game_flag[0] > 0) {
				onAvance();
			}
			break;

		case ETAT_CARBONISE:
		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_SAUTE:
		case ETAT_TOMBE:
			onTombe();
			break;

		case ETAT_TIRE:
			onTire();
			break;
	}

	updateDestroyed();
}

void EnemyRayman::onAvance() {
	if ((game_flag[1] <= dorkemon) && (game_flag[0] == 2) && (game_flag[1] > 0)) {
		if (game_flag[1] == 4) {
			EnemyBulbizarreHero* bulbizarre = new EnemyBulbizarreHero();

			bulbizarre->x = x - 10;
			bulbizarre->y = y - 25;
			bulbizarre->dir = FACING_LEFT;
			list_enemies.emplace_back(bulbizarre);

			nb_enemies_created++;

			mbk_level.play(1);
			game_flag[FLAG_TIMER] = 70;

			dorkemon--;
		} else if (game_flag[1] == 3) {
			EnemyEmberHero* flameche = new EnemyEmberHero();

			flameche->x = x - 10;
			flameche->y = y - 25;
			flameche->dir = FACING_LEFT;
			list_enemies.emplace_back(flameche);

			nb_enemies_created++;

			EnemyCOMHero* Com = new EnemyCOMHero();

			Com->x = x - 10;
			Com->y = y - 25;
			Com->dir = FACING_LEFT;
			list_enemies.emplace_back(Com);

			dorkemon -= 2;

		} else if (game_flag[1] == 1) {
			EnemyPikachuHero* pika = new EnemyPikachuHero();

			nb_enemies_created++;

			pika->x = x - 10;
			pika->y = y - 25;
			pika->dir = FACING_LEFT;
			list_enemies.emplace_back(pika);
			dorkemon--;
		}
	} else if ((game_flag[0] <= 2) && (game_flag[1] == 0)) {
		if (game_flag[0] == 2) {
			if ((hp > PV_SOUS_CRITIQUE) || ((hp > PV_CRITIQUE) && (poing1) && ((x - offset < 80) || (x - offset > 600)))) {
				if (platform_y(x, y) == 0) {
					status = ETAT_TOMBE;
					dy = 0;
					lat_grav = 0;
					onTombe();
				}

				if (x - RAYMAN_SPEED < xmin || solid_wall(x - RAYMAN_SPEED, y)) {
					dir = FACING_RIGHT;
				} else if (x + RAYMAN_SPEED > offset + 600 || solid_wall(x + RAYMAN_SPEED, y)) {
					dir = FACING_LEFT;
				}
				wait_for_attack++;
				if ((wait_for_attack >= attack_delay) && (targeted_player != NULL)) {
					if ((platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y)) &&
						(((dir == FACING_RIGHT) && (x < targeted_player->x)) || ((dir == FACING_LEFT) && (x > targeted_player->x)))) {
						x_cible = targeted_player->x;
						dy = 0;
						status = ETAT_TIRE;
						range_poing = 0;
						anim_step = 0;
						anim_tick = 0;
						attack_step = 0;
						onTire();
						return;
					}
				}
				anim_tick++;
				anim_tick %= 3;
				if (anim_tick == 0) {
					anim_step++;
					anim_step %= 21;
				}
				if (dir == FACING_RIGHT) {
					marche(RAYMAN_SPEED);
					pic = pbk_enemies[188 + anim_step];
				} else {
					marche(-RAYMAN_SPEED);
					pic = pbk_enemies[209 + anim_step];
				}
			} else if ((hp > PV_CRITIQUE) && (x - offset >= 80) && (x - offset <= 600)) {
				if (poing1) {
					Sprite* s;
					if (x - offset > 320) {
						s = new PieceRayman(230, -1 - rand() % 3, -3 - rand() % 3);
					} else {
						s = new PieceRayman(230, 1 + rand() % 3, -3 - rand() % 3);
					}

					s->dir = dir;
					s->y = y - 22;
					s->x = x;

					list_splatters.emplace_back(s);

					game_flag[0] = 1;
					poing1 = 0;
				}
				wait_for_attack++;
				if ((wait_for_attack >= attack_delay) && (targeted_player != NULL)) {
					if (platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y))

					{
						if (x < targeted_player->x) {
							dir = FACING_RIGHT;

						} else {
							dir = FACING_LEFT;
						}
						x_cible = targeted_player->x;
						dy = 0;
						status = ETAT_TIRE;
						range_poing = 0;
						anim_step = 0;
						anim_tick = 0;
						attack_step = 0;
						onTire();
						return;
					}
				}
				if (dir == FACING_RIGHT) {
					pic = pbk_enemies[268];
				} else {
					pic = pbk_enemies[269];
				}
			} else if ((hp > PV_SUR_CRITIQUE)) {
				if (poing2) {
					Sprite* s;
					if (x - offset > 320) {
						s = new PieceRayman(231, -1 - rand() % 3, -3 - rand() % 3);
					} else {
						s = new PieceRayman(231, 1 + rand() % 3, -3 - rand() % 3);
					}

					s->dir = dir;
					s->y = y - 22;
					s->x = x;

					list_splatters.emplace_back(s);
					game_flag[0] = 1;
					poing2 = 0;
				}

				if (platform_y(x, y) == 0) {
					status = ETAT_TOMBE;
					dy = 0;
					lat_grav = 0;
					onTombe();
				}

				if (x - RAYMAN_SPEED < xmin || solid_wall(x - RAYMAN_SPEED, y)) {
					dir = FACING_RIGHT;
				} else if (x + RAYMAN_SPEED > offset + 600 || solid_wall(x + RAYMAN_SPEED, y)) {
					dir = FACING_LEFT;
				}

				anim_tick++;
				anim_tick %= 3;
				if (anim_tick == 0) {
					anim_step++;
					anim_step %= 8;
				}
				if (dir == FACING_RIGHT) {
					marche(RAYMAN_CRITICAL_SPEED);
					pic = pbk_enemies[250 + anim_step];
				} else {
					marche(-RAYMAN_CRITICAL_SPEED);
					pic = pbk_enemies[258 + anim_step];
				}

			} else if ((x - offset >= 80) && (x - offset <= 600)) {
				if (pieds) {
					int vy = -3 - rand() % 3;

					Sprite* s = new PieceRayman(270, 1 + rand() % 3, vy);

					s->dir = dir;
					s->y = y - 22;
					s->x = x + 10;

					list_splatters.emplace_back(s);

					s = new PieceRayman(271, -1 - rand() % 3, vy);

					s->dir = dir;
					s->y = y - 22;
					s->x = x - 10;

					list_splatters.emplace_back(s);

					game_flag[0] = 1;
					pieds = 0;
				}

				if (dir == FACING_RIGHT) {
					pic = pbk_enemies[266];
				} else {
					pic = pbk_enemies[267];
				}
			}
		}
		colFromPic();
	} else if (x - offset > 445) {
		anim_tick++;
		anim_tick %= 3;
		if (anim_tick == 0) {
			anim_step++;
			anim_step %= 21;
		}
		if (dir == FACING_RIGHT) {
			marche(RAYMAN_SPEED);
			pic = pbk_enemies[188 + anim_step];
		} else {
			marche(-RAYMAN_SPEED);
			pic = pbk_enemies[209 + anim_step];
		}

		if (game_flag[3] == 1) {
			col_on = false;
		}
	}
}

void EnemyRayman::onMeure() {
	anim_tick++;
	anim_tick %= 6;

	if ((anim_step == 0) && (anim_tick == 1)) {
		Sprite* tete;
		if (x - offset > 320) {
			tete = new PieceRaymanHead(-1 - rand() % 3, -3 - rand() % 3);
		} else {
			tete = new PieceRaymanHead(1 + rand() % 3, -3 - rand() % 3);
		}
		tete->x = x;
		tete->y = y - 60;
		list_splatters.emplace_back(tete);
		game_flag[3] = 2;
	}

	if (anim_tick == 0) {
		anim_step++;
		if (anim_step >= 6) {
			destroyed = true;
		} else {
			if ((anim_step < 2) && (dir == FACING_RIGHT)) {
				pic = pbk_enemies[494 + anim_step];
			} else {
				pic = pbk_enemies[488 + anim_step];
			}
		}
	}
}

void EnemyRayman::onTire() {
	if (hp > PV_SOUS_CRITIQUE) {
		if (attack_step == 0) {
			anim_tick++;
			anim_tick %= 3;
			if (anim_tick == 0) {
				anim_step++;
				anim_step %= 6;
				range_poing += 30;
				if ((dir == FACING_RIGHT) && (x_cible - x < range_poing)) {
					tir_poing = new ShotRaymanFist(RAYMAN_POING1_SPEED, x_cible);
					tir_poing->setDir(dir);
					tir_poing->x = x + 13;
					tir_poing->y = y - 13;
					list_enemy_shots.emplace_back(tir_poing);

					attack_step = 1;
					anim_tick = 0;
					anim_step = 6;
				} else if ((dir == FACING_LEFT) && (x - x_cible < range_poing)) {
					tir_poing = new ShotRaymanFist(-RAYMAN_POING1_SPEED, x_cible);
					tir_poing->setDir(dir);
					tir_poing->x = x - 13;
					tir_poing->y = y - 13;
					list_enemy_shots.emplace_back(tir_poing);

					attack_step = 1;
					anim_tick = 0;
					anim_step = 6;
				}
			}

		} else if (attack_step == 1) {
			anim_tick++;
			if (anim_tick == 20) {
				anim_step = 7;
			}

			if ((dir == FACING_RIGHT) && (tir_poing->x < x)) {
				status = ETAT_AVANCE;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				tir_poing->destroyed = true;
				attack_delay = 50 + rand() % 150;
				onAvance();
				return;
			} else if ((dir == FACING_LEFT) && (tir_poing->x > x)) {
				status = ETAT_AVANCE;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				tir_poing->destroyed = true;
				attack_delay = 50 + rand() % 150;
				onAvance();
				return;
			}
		}

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[172 + anim_step];
		} else {
			pic = pbk_enemies[180 + anim_step];
		}
	} else {
		if (attack_step == 0) {
			anim_tick++;
			anim_tick %= 3;
			if (anim_tick == 0) {
				anim_step++;
				anim_step %= 7;
				range_poing += 30;
				if ((dir == FACING_RIGHT) && (x_cible - x < range_poing)) {
					tir_poing = new ShotRaymanFist(RAYMAN_POING2_SPEED, x_cible);
					tir_poing->setDir(dir);
					tir_poing->x = x + 13;
					tir_poing->y = y - 13;
					list_enemy_shots.emplace_back(tir_poing);

					attack_step = 1;
					anim_tick = 0;
					anim_step = 7;
				} else if ((dir == FACING_LEFT) && (x - x_cible < range_poing)) {
					tir_poing = new ShotRaymanFist(-RAYMAN_POING2_SPEED, x_cible);
					tir_poing->setDir(dir);
					tir_poing->x = x - 13;
					tir_poing->y = y - 13;
					list_enemy_shots.emplace_back(tir_poing);

					attack_step = 1;
					anim_tick = 0;
					anim_step = 7;
				}
			}

		} else if (attack_step == 1) {
			anim_tick++;
			if (anim_tick == 20) {
				anim_step = 8;
			}

			if ((dir == FACING_RIGHT) && (tir_poing->x < x)) {
				status = ETAT_AVANCE;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				tir_poing->destroyed = true;
				attack_delay = 10 + rand() % 50;
				onAvance();
				return;
			} else if ((dir == FACING_LEFT) && (tir_poing->x > x)) {
				status = ETAT_AVANCE;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				tir_poing->destroyed = true;
				attack_delay = 10 + rand() % 50;
				onAvance();
				return;
			}
		}

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[232 + anim_step];
		} else {
			pic = pbk_enemies[241 + anim_step];
		}
	}

	colFromPic();
}

void EnemyRayman::onTombe() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		onAvance();
		return;
	}

	colFromPic();
}

void EnemyRayman::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	if (game_flag[1] == 0) {
		Enemy::takeHit(shot);
		gicle(shot, dx_giclure, dy_giclure);
	}
}
