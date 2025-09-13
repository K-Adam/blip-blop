#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_bomb.h"
#include "entities/bonus_weapon.h"
#include "entities/enemies/enemy.h"
#include "entities/bonus_cow.h"

int save_time = 0;

const int anim_warning[] = {118, 118, 118, 118, 118, 119, 119, 120, 119, 119};
const int anim_nuke[] = {53, 53, 53, 54, 54, 54, 55, 55, 56, 57, 58, 59, 59, 60, 60, 60, 61, 61,
						 61, 61, 60, 60, 60, 59, 59, 58, 57, 56, 55, 55, 54, 54, 54, 53, 53};

BackgroundBomb::BackgroundBomb() : counter_step(0), warning_step(0), warning_tick(0), nuke_step(0), nuke_tick(0), armed(true) { pic = pbk_level[52]; }

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
		draw(x, y, pbk_level[121 + anim_step]);

		draw(x, y, pbk_level[anim_nuke[nuke_step]]);

		if (game_flag[FLAG_TIMER] >= 100) {
			draw(x + 6, y - 27, pbk_level[80]);
			draw(x + 11, y - 27, pbk_level[80]);
			draw(x + 16, y - 27, pbk_level[80]);
			draw(x + 21, y - 27, pbk_level[80]);
		} else {
			draw(x + 6, y - 27, pbk_level[71 + game_flag[FLAG_TIMER] / 10]);
			draw(x + 11, y - 27, pbk_level[71 + game_flag[FLAG_TIMER] % 10]);
			draw(x + 16, y - 27, pbk_level[80 - counter_step / 10]);
			draw(x + 21, y - 27, pbk_level[80 - counter_step % 10]);
		}

		if (game_flag[FLAG_TIMER] < 40) {
			draw(x, y - 35, pbk_level[anim_warning[warning_step]]);
		} else if (game_flag[FLAG_TIMER] < 60) {
			draw(x, y - 35, pbk_level[118]);
		}
	} else {
		draw(x, y, pbk_level[anim_nuke[nuke_step]]);
		draw(x + 6, y - 27, pbk_level[71 + seconds / 10]);
		draw(x + 11, y - 27, pbk_level[71 + seconds % 10]);
		draw(x + 16, y - 27, pbk_level[80 - hundredths / 10]);
		draw(x + 21, y - 27, pbk_level[80 - hundredths % 10]);
	}
}
