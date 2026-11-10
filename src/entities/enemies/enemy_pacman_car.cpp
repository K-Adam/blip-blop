#include "entities/enemies/enemy_pacman_car.h"
#include "entities/enemies/enemy_pacman.h"

EnemyPacmanCar::EnemyPacmanCar() {
	hp = 1000;
	pic = assets().pics.enemies.pic("pacman_car");
	wait_end = 0;
}

void EnemyPacmanCar::update() {
	if ((level().rules.flags[2] == 1) || (level().rules.flags[2] == 2)) {
		anim_step++;
		if (anim_step > 160) {
			fallCar();
			if (x < 1350) {
				x += 5;
			} else if (x < 1450) {
				x += 4;
			} else if (x < 1550) {
				x += 3;
			} else if (x < 1600) {
				x += 2;
			} else if (level().terrain.platform_y2(x, y + dy) == 1) {
				y = level().terrain.platform_y(x, y + dy);

				level().rules.flags[2] = 3;

				anim_step = 0;
				anim_tick = 0;
			}
		}
	} else if (level().rules.flags[2] == 3) {
		wait_end++;

		if (wait_end >= 250 && level().rules.flags[0] < 6) {
			level().rules.flags[0] = 6;
		}
	} else {
		x = 1400;
		y = -10;
	}
}

void EnemyPacmanCar::fallCar() {
	grav_tick += 1;
	grav_tick %= CAR_GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX_CAR) dy += 1;

	if (dy < 0 && level().terrain.solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = level().terrain.platform_y(x, y + dy);

	if (ny != 0 && dy > 0 && level().terrain.platform_y(x, y) == 0)
		y = ny;
	else
		y += dy;
}

int EnemyPacmanCar::damage() { return 0; };

void EnemyPacmanCar::render() {
	if (level().rules.flags[2] >= 4) {
		draw(x + 48, y - 82, assets().pics.enemies.anim("pacman_car_door")[anim_step]);
	}
	Sprite::render();
}

void EnemyPacmanCar::takeHit(Shot* shot) {}
