#include "entities/enemies/enemy_diabolo.h"
#include "core/debug.h"
#include "entities/shots/shot_tornado.h"

EnemyDiabolo::EnemyDiabolo() : wait_for_attack(0), attack_delay(50 + rand() % 100), attack_type(0) {
	pv = 30000;
	dy = 0;
	xmin = 80;
}

void EnemyDiabolo::update() {
	switch (etat) {
		case ETAT_AVANCE:
		case ETAT_NORMAL:
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
	if (plat(x, y) == 0) {
		etat = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onTombe();
		return;
	}

	wait_for_attack++;
	if ((wait_for_attack > attack_delay) && (tete_turc != NULL)) {
		if (((dir == SENS_DROITE) && (tete_turc->x > x)) || ((dir == SENS_GAUCHE) && (tete_turc->x < x))) {
			int dif_y = tete_turc->y - y;
			int dif_x = tete_turc->x - x;

			if ((tete_turc->y <= y) && (tete_turc->y > y - 170)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				etape = 0;
				ss_etape = 0;
				etat = ETAT_TIRE;
				attack_type = 0;
				dy = -3;
				debug << "dx: " << dx << "  dy: " << dy << "\n";
				onCoupdelatte();
				return;
			} else if ((dir == SENS_DROITE) && (dif_x > -40) && (dif_x < 40) && (dif_y + dif_x < 0)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				etape = 0;
				ss_etape = 0;
				etat = ETAT_TIRE;
				attack_type = 2;
				dx = 1;
				dy = -8;
				debug << "dx: " << dx << "  dy: " << dy << "\n";
				onCoupdegenou();
				return;
			} else if ((dir == SENS_GAUCHE) && (dif_x > -40) && (dif_x < 40) && (dif_y - dif_x < 0)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				etape = 0;
				ss_etape = 0;
				etat = ETAT_TIRE;
				attack_type = 2;
				dx = -1;
				dy = -8;
				onCoupdegenou();
				return;
			} else if ((dir == SENS_DROITE) && (dif_y < 0) && (dif_x > 0) && (dif_y + dif_x < 0)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				etape = 0;
				ss_etape = 0;
				etat = ETAT_TIRE;
				attack_type = 2;
				dx = ((dif_x * -8) / dif_y);
				dy = (int)(-8 + dx * 0.25);
				onCoupdegenou();
				return;
			} else if ((dir == SENS_GAUCHE) && (dif_y < 0) && (dif_x < 0) && (dif_y - dif_x < 0)) {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				etape = 0;
				ss_etape = 0;
				etat = ETAT_TIRE;
				attack_type = 2;
				dx = ((dif_x * -8) / dif_y);
				dy = (int)(-8 + dx * 0.25);
				debug << "dx: " << dx << "  dy: " << dy << "\n";
				onCoupdegenou();
				return;
			} else {
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 100;
				etape = 0;
				ss_etape = 0;
				etat = ETAT_TIRE;
				attack_type = 1;
				onTornade();
				return;
			}
		}
	}

	if (x - 10 < xmin || mur_opaque(x - 10, y)) {
		dir = SENS_DROITE;
	} else if (x + 10 > offset + 640 || mur_opaque(x + 10, y)) {
		dir = SENS_GAUCHE;
	}

	if (dir == SENS_GAUCHE) {
		pic = pbk_ennemis[1];
	} else {
		pic = pbk_ennemis[0];
	}

	colFromPic();
}

void EnemyDiabolo::onTombe() {
	tombe();

	if (plat(x, y) != 0) etat = ETAT_AVANCE;

	colFromPic();
}

void EnemyDiabolo::onMeure() {}

void EnemyDiabolo::onAttack() {}

void EnemyDiabolo::onCoupdelatte() {
	if (etape == 15) {
		etape = 0;
		ss_etape = 0;
		etat = ETAT_AVANCE;
		if (dir == SENS_DROITE) {
			dir = SENS_GAUCHE;
		} else {
			dir = SENS_DROITE;
		}
		onAvance();
		return;
	}
	if (etape < 9) {
		ss_etape++;
		ss_etape %= 6;
		if (ss_etape == 0) {
			etape++;
		}
	} else {
		if (dir == SENS_DROITE) {
			if (mur_opaque(x + DIABOLO_KICK_SPEED, y) || (x + DIABOLO_KICK_SPEED > offset + 560)) {
				ss_etape++;
				ss_etape %= 6;
				if (ss_etape == 0) {
					etape++;
				}
			}
		} else {
			if (mur_opaque(x - DIABOLO_KICK_SPEED, y) || (x - DIABOLO_KICK_SPEED < xmin)) {
				ss_etape++;
				ss_etape %= 6;
				if (ss_etape == 0) {
					etape++;
				}
			}
		}
	}
	if (dir == SENS_DROITE) {
		if ((etape >= 4) && (etape <= 9)) {
			tombe();
			x += DIABOLO_KICK_SPEED;
		}
		pic = pbk_ennemis[8 + etape];
	} else {
		if ((etape >= 4) && (etape <= 9)) {
			tombe();
			x -= DIABOLO_KICK_SPEED;
		}
		pic = pbk_ennemis[23 + etape];
	}
	colFromPic();
}

void EnemyDiabolo::onTornade() {
	ss_etape++;
	ss_etape %= 6;
	if (ss_etape == 0) {
		etape++;
	}

	if (etape == 9) {
		etape = 0;
		ss_etape = 0;
		etat = ETAT_AVANCE;
		onAvance();
		return;
	}

	if ((etape == 6) && (ss_etape == 0)) {
		Shot* tornade;

		if (dir == SENS_DROITE) {
			tornade = new ShotTornado(1);
			tornade->x = x + 30;
		} else {
			tornade = new ShotTornado(-1);
			tornade->x = x - 30;
		}

		tornade->setDir(dir);
		tornade->y = y;
		list_tirs_ennemis.emplace_back(tornade);
	}

	if (dir == SENS_DROITE) {
		pic = pbk_ennemis[38 + etape];
	} else {
		pic = pbk_ennemis[47 + etape];
	}
	colFromPic();
}

void EnemyDiabolo::onCoupdegenou() {
	if (dx == 0) {
		if (dy < 0) {
			dy = 0;
		}
		tombe_diabolo();
		if (dir == SENS_DROITE) {
			x = 590;
		} else {
			x = 50;
		}
	} else {
		y += dy;
	}

	if ((y > 300) && (dx != 0)) {
		x += dx;
		if (dir == SENS_DROITE) {
			pic = pbk_ennemis[2];
		} else {
			pic = pbk_ennemis[5];
		}
	} else if ((y > 150) && (dx != 0)) {
		x += dx;
		if (dir == SENS_DROITE) {
			pic = pbk_ennemis[3];
		} else {
			pic = pbk_ennemis[6];
		}
	} else if ((y > -50) && (dx != 0)) {
		x += dx;
		if (dir == SENS_DROITE) {
			pic = pbk_ennemis[4];
		} else {
			pic = pbk_ennemis[7];
		}
	} else {
		dx = 0;
		if ((plat(x + dx, y + dy) != 0) && (y > 400)) {
			if ((ss_etape == 0) && (etape == 0)) {
				tremblement(7);
			}
			ss_etape++;
			ss_etape %= 8;
			if (ss_etape == 0) {
				etape++;
			}
			if (etape == 4) {
				etape = 0;
				ss_etape = 0;
				etat = ETAT_AVANCE;
				if (dir == SENS_DROITE) {
					dir = SENS_GAUCHE;
				} else {
					dir = SENS_DROITE;
				}
				onAvance();
				return;
			}
		} else {
			x += dx;
		}

		if (dir == SENS_DROITE) {
			pic = pbk_ennemis[19 + etape];
		} else {
			pic = pbk_ennemis[34 + etape];
		}
	}
	colFromPic();
}

void EnemyDiabolo::onCarbonise() {}

void EnemyDiabolo::colFromPic() {
	Sprite::colFromPic();

	if (etat == ETAT_AVANCE) {
		if (dir == SENS_DROITE) {
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

	if (dy < 0 && mur_opaque(x, y + dy)) dy = GRAVITE_MAX;

	int ny = plat2(x, y + dy);

	if (ny == 0 && dy > 0)
		y = plat(x, y + dy);
	else
		y += dy;
}

void EnemyDiabolo::estTouche(Shot* tir) {
	static const int dx_giclure_lem[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_lem[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::estTouche(tir);
	gicle(tir, dx_giclure_lem, dy_giclure_lem);
}
