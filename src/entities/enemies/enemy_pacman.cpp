#include "entities/enemies/enemy_pacman.h"

EnemyPacman::EnemyPacman() : ss_etape_tir(0), etape_tir(0), shot(false), dx(-2) {
	hp = 1000;
	pic = pbk_enemies[223];
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

		pic = pbk_enemies[223 + anim_step];

		if (shot) {
			ss_etape_tir++;
			ss_etape_tir %= 4;
			if (ss_etape_tir == 0) {
				etape_tir++;
				if (etape_tir >= 3) {
					shot = false;
					ss_etape_tir = 0;
					etape_tir = 0;
				}
			}
		} else {
			ss_etape_tir++;
			if (ss_etape_tir >= 13) {
				shot = true;
				ss_etape_tir = 0;
				etape_tir = 0;
			}
		}

		if (platform_y2(x, y) == 3) {
			anim_step = 0;
			anim_tick = 0;
			game_flag[2] = 6;
		}
	} else if (game_flag[2] == 6) {
		pic = pbk_enemies[230];
		anim_step++;
		if (anim_step > 40) {
			game_flag[2] = 7;
			dy = 0;
		}
	} else if (game_flag[2] == 7) {
		pic = pbk_enemies[223];
		tombe2();
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
			pic = pbk_enemies[230 + anim_step];
		}
	} else if (game_flag[2] == 9) {
		pic = pbk_enemies[230];
	}
}

int EnemyPacman::damage() { return 0; };

void EnemyPacman::render() {
	if (shot) {
		draw(x + 30, y - 20, pbk_enemies[232 + etape_tir]);
	}

	Sprite::render();
}

void EnemyPacman::takeHit(Shot* shot) {}
