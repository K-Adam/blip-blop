#include "entities/enemies/enemy_sonic.h"
#include "entities/shots/shot_thorn.h"
#include "entities/shots/shot_sonic_lightning.h"
#include "entities/piece_sonic_ring.h"
#include "core/debug.h"
#include "level/world.h"
#include "core/assets.h"

const int sonic_anim_boule_droite[] = {46, 47, 50, 48, 49, 50};
const int sonic_anim_boule_gauche[] = {51, 52, 55, 53, 54, 55};

const int sonic_anim_super_saien[] = {108, 109, 110, 109, 108, 109, 110, 111, 110, 109, 110, 111, 112, 111, 110, 111, 112, 113, 112, 111, 112, 113,
									  114, 113, 112, 113, 114, 115, 114, 113, 114, 115, 116, 115, 114, 115, 116, 117, 116, 115, 116, 117, 118};

EnemySonic::EnemySonic()
	: speed(1),
	  etape_speed(0),
	  attack_delay(50 + rand() % 250),
	  wait_for_attack(0),
	  vie_sous_critique(3750),
	  vie_critique(2500),
	  vie_sur_critique(1250),
	  ring(90) {
	hp = 10000;
	xmin = 1280;
	ss_etape_attack = 0;
	etape_attack = 0;
	attack_phase = 0;
	nb_tir = 0;
	wait_sang_tombe = 0;
}

void EnemySonic::update() {
	if (game_flag[0] == 7) destroyed = true;

	if (wait_sang_tombe < 400) {
		wait_sang_tombe++;
		return;
	} else if (game_flag[0] < 4) {
		game_flag[0] = 4;
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
					onAttackhorizontal();
					break;

				case 1:
					onAttackvertical();
					break;

				case 2:
					onAttackepine();
					break;

				case 3:
					onAttackeclair();
					break;
			}
			break;
	}
}

void EnemySonic::onAvance() {
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
	if ((wait_for_attack >= attack_delay) && (ring == 0)) {
		if (targeted_player != NULL) {
			if ((targeted_player->y > 0) &&
				(((dir == FACING_RIGHT) && (((targeted_player->x) - x) > 0)) || ((dir == FACING_LEFT) && (((targeted_player->x) - x) < 0)))) {
				int rnd = rand() % 3;
				switch (rnd) {
					case 0:
						attack_type = 0;
						speed = 12;
						status = ETAT_TIRE;
						x_cible = targeted_player->x;
						y_target = targeted_player->y;
						nb_tir = 0;
						attack_phase = -1;
						anim_tick = 0;
						anim_step = 0;
						onAttackhorizontal();
						return;
						break;

					case 1:
						attack_phase = -1;
						attack_type = 1;
						speed = 12;
						status = ETAT_TIRE;
						x_cible = targeted_player->x;
						y_target = targeted_player->y;
						nb_tir = 0;
						anim_tick = 0;
						anim_step = 0;
						onAttackvertical();
						return;
						break;
					case 2:
						attack_type = 3;
						anim_tick = 0;
						anim_step = 0;
						nb_tir = 0;
						status = ETAT_TIRE;
						onAttackeclair();
						return;
						break;
				}
			}
		}
	}

	if ((ring > 0) && (hp < 9000)) {
		if (ring % 30 == 0) {
			onBalancering();
		}
		ring--;
	}

	anim_tick += 1;
	anim_tick %= (10 - speed);
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}
	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[30 + anim_step];
	} else {
		marche(-speed);
		pic = pbk_enemies[38 + anim_step];
	}
	if (speed < SONIC_SPEED) {
		etape_speed++;
		if (etape_speed >= 15) {
			etape_speed = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemySonic::onMeure() {
	if (game_flag[2] == 0) {
		fall();
		if (x - speed < xmin || solid_wall(x - speed, y)) {
			dir = FACING_RIGHT;
			speed = 1;
		} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
			dir = FACING_LEFT;
			speed = 1;
		}
		anim_tick += 1;
		anim_tick %= (10 - speed);
		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 8;
		}
		if (dir == FACING_RIGHT) {
			marche(speed);
			pic = pbk_enemies[30 + anim_step];
		} else {
			marche(-speed);
			pic = pbk_enemies[38 + anim_step];
		}
		if (speed < SONIC_SPEED) {
			etape_speed++;
			if (etape_speed >= 15) {
				etape_speed = 0;
				speed++;
			}
		}

		if ((platform_y2(x, y) == 0) && (x > 1600) && (x < 1630)) {
			game_flag[2] = 1;
			anim_step = 0;
			anim_tick = 0;
			dir = FACING_LEFT;
		}
	} else if (game_flag[2] == 1) {
		y -= 1;
		anim_tick++;
		anim_tick %= 5;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step >= 43) {
				game_flag[2] = 2;
				destroyed = true;
			} else {
				pic = pbk_enemies[sonic_anim_super_saien[anim_step]];
			}
		}
	}
}

void EnemySonic::onCharge() {}

void EnemySonic::onSaute() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		anim_tick = 0;
		anim_step = 0;
		onAvance();
		return;
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

	anim_tick += 1;
	anim_tick %= (10 - speed);
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}
	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[30 + anim_step];
	} else {
		marche(-speed);
		pic = pbk_enemies[38 + anim_step];
	}
	colFromPic();
}

void EnemySonic::onAttackhorizontal() {
	if (attack_phase == -1) {
		if (hp < vie_sous_critique) {
			ss_etape_attack += 1;
			ss_etape_attack %= 2;
			if (ss_etape_attack == 0) {
				etape_attack += 1;
				etape_attack %= 16;
			}
		}
		nb_tir += 1;
		if (nb_tir == 50) {
			attack_phase = 0;
		} else {
			anim_tick += 1;
			anim_tick %= 2;
			if (anim_tick == 0) {
				anim_step += 1;
				anim_step %= 5;
			}
			if (dir == FACING_RIGHT) {
				pic = pbk_enemies[85 + anim_step];
			} else {
				pic = pbk_enemies[91 + anim_step];
			}
		}

	} else {
		if (attack_phase == 0) {
			if (hp < vie_critique) {
				ss_etape_attack += 1;
				ss_etape_attack %= 2;
				if (ss_etape_attack == 0) {
					etape_attack += 1;
					etape_attack %= 16;
				}
			}
			fall();
			if ((dir == FACING_RIGHT) && (x + speed > 1900)) {
				speed = 0;
				dy = -12;
				attack_phase = 1;
			}
			if ((dir == FACING_LEFT) && (x - speed < 1300)) {
				speed = 0;
				dy = -12;
				attack_phase = 1;
			}
		} else if (attack_phase == 1) {
			fall();
			if (hp < vie_sur_critique) {
				ss_etape_attack += 1;
				ss_etape_attack %= 2;
				if (ss_etape_attack == 0) {
					etape_attack += 1;
					etape_attack %= 16;
				}
			}
			if (y + dy < y_target) {
				if (dir == FACING_RIGHT) {
					attack_phase = 2;
					dir = FACING_LEFT;
					dy = 0;
					speed = 12;
				} else {
					attack_phase = 2;
					dir = FACING_RIGHT;
					dy = 0;
					speed = 12;
				}
			}
		} else if (attack_phase == 2) {
			if (x - speed < xmin || solid_wall(x - speed, y)) {
				dir = FACING_RIGHT;
				speed = 1;
				status = ETAT_AVANCE;
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				anim_tick = 0;
				anim_step = 0;
				onAvance();
				return;
			} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
				dir = FACING_LEFT;
				speed = 1;
				status = ETAT_AVANCE;
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				anim_tick = 0;
				anim_step = 0;
				onAvance();
				return;
			}
		}
		if (dir == FACING_RIGHT) {
			x += speed;
			pic = pbk_enemies[animate(sonic_anim_boule_droite, 6, 3)];
		} else {
			x -= speed;
			pic = pbk_enemies[animate(sonic_anim_boule_gauche, 6, 3)];
		}
	}
	colFromPic();
}

void EnemySonic::onAttackvertical() {
	if (attack_phase == -1) {
		if (hp < vie_sous_critique) {
			ss_etape_attack += 1;
			ss_etape_attack %= 2;
			if (ss_etape_attack == 0) {
				etape_attack += 1;
				etape_attack %= 16;
			}
		}
		nb_tir += 1;
		if (nb_tir == 50) {
			attack_phase = 0;
		} else {
			anim_tick += 1;
			anim_tick %= 2;
			if (anim_tick == 0) {
				anim_step += 1;
				anim_step %= 5;
			}
			if (dir == FACING_RIGHT) {
				pic = pbk_enemies[85 + anim_step];
			} else {
				pic = pbk_enemies[91 + anim_step];
			}
		}

	} else {
		if (attack_phase == 0) {
			fall();
			if (hp < vie_critique) {
				ss_etape_attack += 1;
				ss_etape_attack %= 2;
				if (ss_etape_attack == 0) {
					etape_attack += 1;
					etape_attack %= 16;
				}
			}
			if ((dir == FACING_RIGHT) && (x + speed > 1900)) {
				speed = 0;
				dy = -12;
				attack_phase = 1;
			} else if ((dir == FACING_LEFT) && (x - speed < 1300)) {
				speed = 0;
				dy = -12;
				attack_phase = 1;
			}
		} else if (attack_phase == 1) {
			fall();
			if (hp < vie_sur_critique) {
				ss_etape_attack += 1;
				ss_etape_attack %= 2;
				if (ss_etape_attack == 0) {
					etape_attack += 1;
					etape_attack %= 16;
				}
			}
			if (y + dy < 40) {
				if (dir == FACING_RIGHT) {
					attack_phase = 2;
					dir = FACING_LEFT;
					speed = -dy;
					dy = 0;
				} else {
					attack_phase = 2;
					dir = FACING_RIGHT;
					speed = -dy;
					dy = 0;
				}
			}
		} else if (attack_phase == 2) {
			if ((dir == FACING_RIGHT) && (x > x_cible)) {
				attack_phase = 3;
				dy = 12;
				speed = 0;
			} else if ((dir == FACING_LEFT) && (x < x_cible)) {
				attack_phase = 3;
				dy = 12;
				speed = 0;
			}
		} else if (attack_phase == 3) {
			if (y + dy > y_platform[0][x]) {
				fall();
				y = y_platform[0][x];
				dy = 0;
				anim_tick = 0;
				anim_step = 0;
				speed = 1;
				status = ETAT_AVANCE;
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				onAvance();
				return;
			} else {
				y += dy;
			}
		}
		if (dir == FACING_RIGHT) {
			x += speed;
			pic = pbk_enemies[animate(sonic_anim_boule_droite, 6, 3)];
		} else {
			x -= speed;
			pic = pbk_enemies[animate(sonic_anim_boule_gauche, 6, 3)];
		}
	}
	colFromPic();
}

void EnemySonic::onAttackepine() {
	nb_tir += 1;
	if (nb_tir > 200) {
		speed = 1;
		status = ETAT_AVANCE;
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		anim_tick = 0;
		anim_step = 0;
		onAvance();
		return;
	}

	anim_tick += 1;
	if (hp > vie_sous_critique) {
		anim_tick %= 5;
	} else if (hp > vie_critique) {
		anim_tick %= 4;
	} else if (hp > vie_sur_critique) {
		anim_tick %= 3;
	} else {
		anim_tick %= 2;
	}

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 5;
	}
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[85 + anim_step];
	} else {
		pic = pbk_enemies[91 + anim_step];
	}
	ss_etape_attack += 1;
	ss_etape_attack %= 1;
	if (ss_etape_attack == 0) {
		etape_attack += 1;
		etape_attack %= 16;
	}
	colFromPic();
}

void EnemySonic::onAttackeclair() {
	anim_tick++;
	anim_tick %= 5;
	if (anim_tick == 0) {
		anim_step++;
	}
	if (anim_step == 23) {
		speed = 1;
		status = ETAT_AVANCE;
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		anim_tick = 0;
		anim_step = 0;
		onAvance();
		return;
	}

	if ((anim_step == 5) && (anim_tick == 0)) {
		if ((nb_tir <= 1) && (hp < vie_sous_critique)) {
			anim_step = 0;
			anim_tick = 1;
		} else if ((nb_tir <= 2) && (hp < vie_critique)) {
			anim_step = 0;
			anim_tick = 1;
		} else if ((nb_tir <= 3) && (hp < vie_sur_critique)) {
			anim_step = 0;
			anim_tick = 1;
		}
	}

	if ((anim_step == 0) && (anim_tick == 1) && (targeted_player != NULL)) {
		ShotSonicLightning* shot;
		shot = new ShotSonicLightning();
		shot->setDir(dir);
		shot->x = targeted_player->x;
		shot->y = 0;
		list_enemy_shots.emplace_back(shot);
		nb_tir += 1;
	}

	if (dir == FACING_RIGHT) {
		y -= 1;
		pic = pbk_enemies[97 + anim_tick];
	} else {
		y -= 1;
		pic = pbk_enemies[108 + anim_tick];
	}

	colFromPic();
}

void EnemySonic::onBalancering() {
	Sprite* s;
	int i, mx = 1, my = 1;

	for (i = 0; i < 4; i++) {
		switch (i) {
			case 1:
				mx = -1;
				break;

			case 2:
				my = -1;
				break;

			case 3:
				mx = 1;
				break;
		}
		s = new PieceSonicRing(1 * mx, 10 * my);
		s->x = x;
		s->y = y - 40;
		list_splatters.emplace_back(s);

		s = new PieceSonicRing(3 * mx, 9 * my);
		s->x = x;
		s->y = y - 40;
		list_splatters.emplace_back(s);

		s = new PieceSonicRing(4 * mx, 7 * my);
		s->x = x;
		s->y = y - 40;
		list_splatters.emplace_back(s);

		s = new PieceSonicRing(5 * mx, 6 * my);
		s->x = x;
		s->y = y - 40;
		list_splatters.emplace_back(s);

		s = new PieceSonicRing(6 * mx, 5 * my);
		s->x = x;
		s->y = y - 40;
		list_splatters.emplace_back(s);

		s = new PieceSonicRing(7 * mx, 4 * my);
		s->x = x;
		s->y = y - 40;
		list_splatters.emplace_back(s);

		s = new PieceSonicRing(9 * mx, 3 * my);
		s->x = x;
		s->y = y - 40;
		list_splatters.emplace_back(s);

		s = new PieceSonicRing(10 * mx, 1 * my);
		s->x = x;
		s->y = y - 40;
		list_splatters.emplace_back(s);
	}
}

void EnemySonic::balance_epines() {
	ShotThorn* shot;
	switch (etape_attack) {
		case 0:
			shot = new ShotThorn(59, 12, 0);
			shot->setDir(dir);
			shot->x = x + 23;
			shot->y = y - 13;
			list_enemy_shots.emplace_back(shot);
			break;

		case 1:
			shot = new ShotThorn(60, 9, -2);
			shot->setDir(dir);
			shot->x = x + 23;
			shot->y = y - 18;
			list_enemy_shots.emplace_back(shot);
			break;

		case 2:
			shot = new ShotThorn(61, 8, -8);
			shot->setDir(dir);
			shot->x = x + 18;
			shot->y = y - 23;
			list_enemy_shots.emplace_back(shot);
			break;

		case 3:
			shot = new ShotThorn(62, 2, -9);
			shot->setDir(dir);
			shot->x = x + 13;
			shot->y = y - 23;
			list_enemy_shots.emplace_back(shot);
			break;

		case 4:
			shot = new ShotThorn(63, 0, -12);
			shot->setDir(dir);
			shot->x = x + 0;
			shot->y = y - 23;
			list_enemy_shots.emplace_back(shot);
			break;

		case 5:
			shot = new ShotThorn(64, -2, -9);
			shot->setDir(dir);
			shot->x = x - 13;
			shot->y = y - 23;
			list_enemy_shots.emplace_back(shot);
			break;

		case 6:
			shot = new ShotThorn(65, -8, -8);
			shot->setDir(dir);
			shot->x = x - 18;
			shot->y = y - 23;
			list_enemy_shots.emplace_back(shot);
			break;

		case 7:
			shot = new ShotThorn(66, -9, -2);
			shot->setDir(dir);
			shot->x = x - 23;
			shot->y = y - 18;
			list_enemy_shots.emplace_back(shot);
			break;

		case 8:
			shot = new ShotThorn(67, -12, 0);
			shot->setDir(dir);
			shot->x = x - 23;
			shot->y = y - 13;
			list_enemy_shots.emplace_back(shot);
			break;

		case 9:
			shot = new ShotThorn(68, -9, 2);
			shot->setDir(dir);
			shot->x = x - 23;
			shot->y = y - 8;
			list_enemy_shots.emplace_back(shot);
			break;

		case 10:
			shot = new ShotThorn(69, -8, 8);
			shot->setDir(dir);
			shot->x = x - 18;
			shot->y = y - 3;
			list_enemy_shots.emplace_back(shot);
			break;

		case 11:
			shot = new ShotThorn(70, -2, 9);
			shot->setDir(dir);
			shot->x = x - 13;
			shot->y = y - 3;
			list_enemy_shots.emplace_back(shot);
			break;

		case 12:
			shot = new ShotThorn(71, 0, 12);
			shot->setDir(dir);
			shot->x = x - 0;
			shot->y = y - 0;
			list_enemy_shots.emplace_back(shot);
			break;

		case 13:
			shot = new ShotThorn(72, 2, 9);
			shot->setDir(dir);
			shot->x = x + 13;
			shot->y = y - 3;
			list_enemy_shots.emplace_back(shot);
			break;

		case 14:
			shot = new ShotThorn(73, 8, 8);
			shot->setDir(dir);
			shot->x = x + 18;
			shot->y = y - 8;
			list_enemy_shots.emplace_back(shot);
			break;

		case 15:
			shot = new ShotThorn(74, 9, 2);
			shot->setDir(dir);
			shot->x = x + 18;
			shot->y = y - 13;
			list_enemy_shots.emplace_back(shot);
			break;
	}
}

void EnemySonic::onCarbonise() { destroyed = true; }

void EnemySonic::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);
}
