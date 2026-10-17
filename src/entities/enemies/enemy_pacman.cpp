#include "entities/enemies/enemy_pacman.h"

EnemyPacman::EnemyPacman() : shot_tick(0), shot_step(0), shot(false), dx(-2) {
	hp = 1000;
	pic = pbk_enemies.anim("pacman_walk")[0];
	col_on = false;
	dy = -8;
}

void EnemyPacman::update() {
	if (game_flag[2] == 5) {
		x += dx;
		fall();
		anim_tick++;
		anim_tick %= 5;
		if (anim_tick == 0) {
			anim_step++;
			anim_step %= 7;
		}

		pic = pbk_enemies.anim("pacman_walk")[anim_step];

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

		if (platform_y2(x, y) == 3) {
			anim_step = 0;
			anim_tick = 0;
			game_flag[2] = 6;
		}
	} else if (game_flag[2] == 6) {
		pic = pbk_enemies.anim("pacman_eat")[0];
		anim_step++;
		if (anim_step > 40) {
			game_flag[2] = 7;
			dy = 0;
		}
	} else if (game_flag[2] == 7) {
		pic = pbk_enemies.anim("pacman_walk")[0];
		fall2();
		if (y + 2 * dy > y_platform[0][x]) {
			y = y_platform[0][x];
			game_flag[2] = 8;
			anim_step = 0;
			anim_tick = 0;
		}
	} else if (game_flag[2] == 8) {
		anim_tick++;
		anim_tick %= 40;
		if (anim_tick == 0) {
			anim_step++;
		}

		if (anim_step >= 2) {
			game_flag[2] = 9;
			anim_step = 0;
			anim_tick = 0;
		} else {
			pic = pbk_enemies.anim("pacman_eat")[anim_step];
		}
	} else if (game_flag[2] == 9) {
		pic = pbk_enemies.anim("pacman_eat")[0];
	}
}

int EnemyPacman::damage() { return 0; };

void EnemyPacman::render() {
	if (shot) {
		draw(x + 30, y - 20, pbk_enemies.anim("pacman_slash")[shot_step]);
	}

	Sprite::render();
}

void EnemyPacman::takeHit(Shot* shot) {}
