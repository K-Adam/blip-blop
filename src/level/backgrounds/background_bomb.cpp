#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_bomb.h"
#include "entities/bonus_weapon.h"
#include "entities/enemies/enemy.h"
#include "entities/bonus_cow.h"

int save_time = 0;

const int anim_warning[] = {0, 0, 0, 0, 0, 1, 1, 2, 1, 1};
const int anim_nuke[] = {0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0};

BackgroundBomb::BackgroundBomb() : counter_step(0), warning_step(0), warning_tick(0), nuke_step(0), nuke_tick(0), armed(true) { pic = pbk_level.pic("bomb"); }

void BackgroundBomb::update() {
	if (armed) {
		if (targeted_player != NULL) {
			if ((targeted_player->x > x - 50) && (targeted_player->x < x + 50) && (targeted_player->y > y - 10) && (targeted_player->y < y + 51)) {
				game_flag[0]++;
				armed = false;
				Bonus* bonus = nullptr;
				switch (game_flag[0]) {
					case 1:
						bonus = new BonusShotgun();
						break;

					case 2:
						bonus = new BonusPM();
						break;

					case 3:
						bonus = new BonusCow();
						save_time = game_flag[FLAG_TIMER];
						game_flag[FLAG_GEN_OFF] = 1;
						break;
				}

				if (bonus) {
					bonus->x = x;
					bonus->y = 0;
					list_bonus.emplace_back(bonus);
				}

				sbk_level.play(1);
			}
		}
		anim_tick++;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step++;
			anim_step %= 9;
		}

		nuke_tick++;
		nuke_tick %= 3;

		if (nuke_tick == 0) {
			nuke_step++;
			nuke_step %= 35;
		}

		counter_step += 2;
		if (counter_step >= 100) {
			counter_step = 0;
		}

		if (game_flag[FLAG_TIMER] < 40) {
			warning_tick++;
			if (game_flag[FLAG_TIMER] / 3 > 0) {
				warning_tick %= game_flag[FLAG_TIMER] / 3;
			} else {
				warning_tick = 0;
			}

			if (warning_tick == 0) {
				warning_step++;
				warning_step %= 10;
			}
		}
		seconds = game_flag[FLAG_TIMER];
		hundredths = counter_step;
	} else if (game_flag[0] == 3) {
		game_flag[FLAG_TIMER] = save_time;
	}
}

void BackgroundBomb::render() {
	Sprite::render();
	if (armed) {
		draw(x, y, pbk_level.anim("bomb_lights")[anim_step]);

		draw(x, y, pbk_level.anim("bomb_nuke_sign")[anim_nuke[nuke_step]]);

		if (game_flag[FLAG_TIMER] >= 100) {
			draw(x + 6, y - 27, pbk_level.anim("bomb_digits")[9]);
			draw(x + 11, y - 27, pbk_level.anim("bomb_digits")[9]);
			draw(x + 16, y - 27, pbk_level.anim("bomb_digits")[9]);
			draw(x + 21, y - 27, pbk_level.anim("bomb_digits")[9]);
		} else {
			draw(x + 6, y - 27, pbk_level.anim("bomb_digits")[game_flag[FLAG_TIMER] / 10]);
			draw(x + 11, y - 27, pbk_level.anim("bomb_digits")[game_flag[FLAG_TIMER] % 10]);
			draw(x + 16, y - 27, pbk_level.anim("bomb_digits")[9 - counter_step / 10]);
			draw(x + 21, y - 27, pbk_level.anim("bomb_digits")[9 - counter_step % 10]);
		}

		if (game_flag[FLAG_TIMER] < 40) {
			draw(x, y - 35, pbk_level.anim("bomb_warning")[anim_warning[warning_step]]);
		} else if (game_flag[FLAG_TIMER] < 60) {
			draw(x, y - 35, pbk_level.anim("bomb_warning")[0]);
		}
	} else {
		draw(x, y, pbk_level.anim("bomb_nuke_sign")[anim_nuke[nuke_step]]);
		draw(x + 6, y - 27, pbk_level.anim("bomb_digits")[seconds / 10]);
		draw(x + 11, y - 27, pbk_level.anim("bomb_digits")[seconds % 10]);
		draw(x + 16, y - 27, pbk_level.anim("bomb_digits")[9 - hundredths / 10]);
		draw(x + 21, y - 27, pbk_level.anim("bomb_digits")[9 - hundredths % 10]);
	}
}
