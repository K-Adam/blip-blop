#include "entities/enemies/enemy_pacman_car.h"
#include "entities/enemies/enemy_pacman.h"

EnemyPacmanCar::EnemyPacmanCar() : invoc_pacman(false) {
	pv = 1000;
	pic = pbk_ennemis[216];
	wait_end = 0;
}

void EnemyPacmanCar::update() {
	if ((game_flag[2] == 1) || (game_flag[2] == 2)) {
		etape++;
		if (etape > 160) {
			tombeVoiture();
			if (x < 1350) {
				x += 5;
			} else if (x < 1450) {
				x += 4;
			} else if (x < 1550) {
				x += 3;
			} else if (x < 1600) {
				x += 2;
			} else if (plat2(x, y + dy) == 1) {
				y = plat(x, y + dy);

				game_flag[2] = 3;

				etape = 0;
				ss_etape = 0;
			}
		}
	} else if (game_flag[2] == 3) {
		wait_end++;

		if (wait_end >= 250 && game_flag[0] < 6) {
			game_flag[0] = 6;
		}
	} else {
		x = 1400;
		y = -10;
	}
}

void EnemyPacmanCar::tombeVoiture() {
	lat_grav += 1;
	lat_grav %= LATENCE_GRAVITE_VOITURE;

	if (lat_grav == 0 && dy < GRAVITE_MAX_VOITURE) dy += 1;

	if (dy < 0 && mur_opaque(x, y + dy)) dy = GRAVITE_MAX;

	int ny = plat(x, y + dy);

	if (ny != 0 && dy > 0 && plat(x, y) == 0)
		y = ny;
	else
		y += dy;
}

int EnemyPacmanCar::degats() { return 0; };

void EnemyPacmanCar::affiche() {
	if (game_flag[2] >= 4) {
		draw(x + 48, y - 82, pbk_ennemis[217 + etape]);
	}
	Sprite::affiche();
}

void EnemyPacmanCar::estTouche(Shot* tir) {}
