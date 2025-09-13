#include "entities/enemies/enemy_pacman_car.h"
#include "entities/enemies/enemy_pacman.h"

EnemyPacmanCar::EnemyPacmanCar() : invoc_pacman(false) {
	hp = 1000;
	pic = pbk_enemies[216];
	wait_end = 0;
}

void EnemyPacmanCar::update() {
	if ((game_flag[2] == 1) || (game_flag[2] == 2)) {
		anim_step++;
		if (anim_step > 160) {
			tombeVoiture();
			if (x < 1350) {
				x += 5;
			} else if (x < 1450) {
				x += 4;
			} else if (x < 1550) {
				x += 3;
			} else if (x < 1600) {
				x += 2;
			} else if (platform_y2(x, y + dy) == 1) {
				y = platform_y(x, y + dy);

				game_flag[2] = 3;

				anim_step = 0;
				anim_tick = 0;
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

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = platform_y(x, y + dy);

	if (ny != 0 && dy > 0 && platform_y(x, y) == 0)
		y = ny;
	else
		y += dy;
}

int EnemyPacmanCar::damage() { return 0; };

void EnemyPacmanCar::render() {
	if (game_flag[2] >= 4) {
		draw(x + 48, y - 82, pbk_enemies[217 + anim_step]);
	}
	Sprite::render();
}

void EnemyPacmanCar::takeHit(Shot* shot) {}
