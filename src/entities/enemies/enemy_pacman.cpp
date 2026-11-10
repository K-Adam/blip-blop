#include "entities/enemies/enemy_pacman.h"

EnemyPacman::EnemyPacman() : shot(false), shot_step(0), shot_tick(0), dx(-2) {
	hp = 1000;
	pic = assets().pics.enemies.anim("pacman_walk")[0];
	col_on = false;
	dy = -8;
}

void EnemyPacman::update() {
	if (level().rules.flags[2] == 5) {
		x += dx;
		fall();
		anim_tick++;
		anim_tick %= 5;
		if (anim_tick == 0) {
			anim_step++;
			anim_step %= 7;
		}

		pic = assets().pics.enemies.anim("pacman_walk")[anim_step];

		if (shot) {
			shot_tick++;
			shot_tick %= 4;
			if (shot_tick == 0) {
				shot_step++;
				if (shot_step >= 3) {
					shot = false;
					shot_tick = 0;
					shot_step = 0;
				}
			}
		} else {
			shot_tick++;
			if (shot_tick >= 13) {
				shot = true;
				shot_tick = 0;
				shot_step = 0;
			}
		}

		if (level().terrain.platform_y2(x, y) == 3) {
			anim_step = 0;
			anim_tick = 0;
			level().rules.flags[2] = 6;
		}
	} else if (level().rules.flags[2] == 6) {
		pic = assets().pics.enemies.anim("pacman_eat")[0];
		anim_step++;
		if (anim_step > 40) {
			level().rules.flags[2] = 7;
			dy = 0;
		}
	} else if (level().rules.flags[2] == 7) {
		pic = assets().pics.enemies.anim("pacman_walk")[0];
		fall2();
		if (y + 2 * dy > level().terrain.platforms[0][x]) {
			y = level().terrain.platforms[0][x];
			level().rules.flags[2] = 8;
			anim_step = 0;
			anim_tick = 0;
		}
	} else if (level().rules.flags[2] == 8) {
		anim_tick++;
		anim_tick %= 40;
		if (anim_tick == 0) {
			anim_step++;
		}

		if (anim_step >= 2) {
			level().rules.flags[2] = 9;
			anim_step = 0;
			anim_tick = 0;
		} else {
			pic = assets().pics.enemies.anim("pacman_eat")[anim_step];
		}
	} else if (level().rules.flags[2] == 9) {
		pic = assets().pics.enemies.anim("pacman_eat")[0];
	}
}

int EnemyPacman::damage() { return 0; };

void EnemyPacman::render() {
	if (shot) {
		draw(x + 30, y - 20, assets().pics.enemies.anim("pacman_slash")[shot_step]);
	}

	Sprite::render();
}

void EnemyPacman::takeHit(Shot* shot) {}
