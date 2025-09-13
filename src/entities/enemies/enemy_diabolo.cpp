#include "entities/enemies/enemy_diabolo.h"
#include "core/debug.h"
#include "entities/shots/shot_tornado.h"

EnemyDiabolo::EnemyDiabolo() : wait_for_attack(0), attack_delay(50 + rand() % 100), attack_type(0) {
	hp = 30000;
	dy = 0;
	xmin = 80;
}

void EnemyDiabolo::update() {
	switch (status) {
		case ETAT_AVANCE:
		case STATE_NORMAL:
			onAvance();
			break;

		case ETAT_TOMBE:
		case ETAT_SAUTE:
			onTombe();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;

		case ETAT_TIRE:
			switch (attack_type) {
				case 0:
					onCoupdelatte();
					break;

				case 1:
					onTornade();
					break;

				case 2:
					onCoupdegenou();
					break;
			}
			break;
	}
}

void EnemyDiabolo::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onTombe();
		return;
	}

	wait_for_attack++;
	if ((wait_for_attack > attack_delay) && (targeted_player != NULL)) {
		if (((dir == FACING_RIGHT) && (targeted_player->x > x)) || ((dir == FACING_LEFT) && (targeted_player->x < x))) {
			int dif_y = targeted_player->y - y;
			int dif_x = targeted_player->x - x;

			if ((targeted_player->y <= y) && (targeted_player->y > y - 170)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_TIRE;
				attack_type = 0;
				dy = -3;
				debug << "dx: " << dx << "  dy: " << dy << "\n";
				onCoupdelatte();
				return;
			} else if ((dir == FACING_RIGHT) && (dif_x > -40) && (dif_x < 40) && (dif_y + dif_x < 0)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_TIRE;
				attack_type = 2;
				dx = 1;
				dy = -8;
				debug << "dx: " << dx << "  dy: " << dy << "\n";
				onCoupdegenou();
				return;
			} else if ((dir == FACING_LEFT) && (dif_x > -40) && (dif_x < 40) && (dif_y - dif_x < 0)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_TIRE;
				attack_type = 2;
				dx = -1;
				dy = -8;
				onCoupdegenou();
				return;
			} else if ((dir == FACING_RIGHT) && (dif_y < 0) && (dif_x > 0) && (dif_y + dif_x < 0)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_TIRE;
				attack_type = 2;
				dx = ((dif_x * -8) / dif_y);
				dy = (int)(-8 + dx * 0.25);
				onCoupdegenou();
				return;
			} else if ((dir == FACING_LEFT) && (dif_y < 0) && (dif_x < 0) && (dif_y - dif_x < 0)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_TIRE;
				attack_type = 2;
				dx = ((dif_x * -8) / dif_y);
				dy = (int)(-8 + dx * 0.25);
				debug << "dx: " << dx << "  dy: " << dy << "\n";
				onCoupdegenou();
				return;
			} else {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_TIRE;
				attack_type = 1;
				onTornade();
				return;
			}
		}
	}

	if (x - 10 < xmin || solid_wall(x - 10, y)) {
		dir = FACING_RIGHT;
	} else if (x + 10 > offset + 640 || solid_wall(x + 10, y)) {
		dir = FACING_LEFT;
	}

	if (dir == FACING_LEFT) {
		pic = pbk_enemies[1];
	} else {
		pic = pbk_enemies[0];
	}

	colFromPic();
}

void EnemyDiabolo::onTombe() {
	fall();

	if (platform_y(x, y) != 0) status = ETAT_AVANCE;

	colFromPic();
}

void EnemyDiabolo::onMeure() {}

void EnemyDiabolo::onAttack() {}

void EnemyDiabolo::onCoupdelatte() {
	if (anim_step == 15) {
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_AVANCE;
		if (dir == FACING_RIGHT) {
			dir = FACING_LEFT;
		} else {
			dir = FACING_RIGHT;
		}
		onAvance();
		return;
	}
	if (anim_step < 9) {
		anim_tick++;
		anim_tick %= 6;
		if (anim_tick == 0) {
			anim_step++;
		}
	} else {
		if (dir == FACING_RIGHT) {
			if (solid_wall(x + DIABOLO_KICK_SPEED, y) || (x + DIABOLO_KICK_SPEED > offset + 560)) {
				anim_tick++;
				anim_tick %= 6;
				if (anim_tick == 0) {
					anim_step++;
				}
			}
		} else {
			if (solid_wall(x - DIABOLO_KICK_SPEED, y) || (x - DIABOLO_KICK_SPEED < xmin)) {
				anim_tick++;
				anim_tick %= 6;
				if (anim_tick == 0) {
					anim_step++;
				}
			}
		}
	}
	if (dir == FACING_RIGHT) {
		if ((anim_step >= 4) && (anim_step <= 9)) {
			fall();
			x += DIABOLO_KICK_SPEED;
		}
		pic = pbk_enemies[8 + anim_step];
	} else {
		if ((anim_step >= 4) && (anim_step <= 9)) {
			fall();
			x -= DIABOLO_KICK_SPEED;
		}
		pic = pbk_enemies[23 + anim_step];
	}
	colFromPic();
}

void EnemyDiabolo::onTornade() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step == 9) {
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_AVANCE;
		onAvance();
		return;
	}

	if ((anim_step == 6) && (anim_tick == 0)) {
		Shot* tornade;

		if (dir == FACING_RIGHT) {
			tornade = new ShotTornado(1);
			tornade->x = x + 30;
		} else {
			tornade = new ShotTornado(-1);
			tornade->x = x - 30;
		}

		tornade->setDir(dir);
		tornade->y = y;
		list_enemy_shots.emplace_back(tornade);
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[38 + anim_step];
	} else {
		pic = pbk_enemies[47 + anim_step];
	}
	colFromPic();
}

void EnemyDiabolo::onCoupdegenou() {
	if (dx == 0) {
		if (dy < 0) {
			dy = 0;
		}
		tombe_diabolo();
		if (dir == FACING_RIGHT) {
			x = 590;
		} else {
			x = 50;
		}
	} else {
		y += dy;
	}

	if ((y > 300) && (dx != 0)) {
		x += dx;
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[2];
		} else {
			pic = pbk_enemies[5];
		}
	} else if ((y > 150) && (dx != 0)) {
		x += dx;
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[3];
		} else {
			pic = pbk_enemies[6];
		}
	} else if ((y > -50) && (dx != 0)) {
		x += dx;
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[4];
		} else {
			pic = pbk_enemies[7];
		}
	} else {
		dx = 0;
		if ((platform_y(x + dx, y + dy) != 0) && (y > 400)) {
			anim_tick++;
			anim_tick %= 8;
			if (anim_tick == 0) {
				anim_step++;
			}
			if (anim_step == 4) {
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_AVANCE;
				if (dir == FACING_RIGHT) {
					dir = FACING_LEFT;
				} else {
					dir = FACING_RIGHT;
				}
				onAvance();
				return;
			}
		} else {
			x += dx;
		}

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[19 + anim_step];
		} else {
			pic = pbk_enemies[34 + anim_step];
		}
	}
	colFromPic();
}

void EnemyDiabolo::onCarbonise() {}

void EnemyDiabolo::colFromPic() {
	Sprite::colFromPic();

	if (status == ETAT_AVANCE) {
		if (dir == FACING_RIGHT) {
			x2 -= 45;
		} else {
			x1 += 45;
		}
	}
}

void EnemyDiabolo::tombe_diabolo() {
	lat_grav += 1;
	lat_grav %= LATENCE_DIABOLO_GRAVITE;

	if (lat_grav == 0 && dy < DIABOLO_GRAVITE_MAX) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = platform_y2(x, y + dy);

	if (ny == 0 && dy > 0)
		y = platform_y(x, y + dy);
	else
		y += dy;
}

void EnemyDiabolo::takeHit(Shot* shot) {
	static const int dx_giclure_lem[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_lem[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_lem, dy_giclure_lem);
}
