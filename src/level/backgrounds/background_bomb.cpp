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

BackgroundBomb::BackgroundBomb() : etape_compteur(0), etape_warning(0), ss_etape_warning(0), etape_nuke(0), ss_etape_nuke(0), armed(true) {
	pic = pbk_level[52];
}

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

		ss_etape_nuke++;
		ss_etape_nuke %= 3;

		if (ss_etape_nuke == 0) {
			etape_nuke++;
			etape_nuke %= 35;
		}

		etape_compteur += 2;
		if (etape_compteur >= 100) {
			etape_compteur = 0;
		}

		if (game_flag[FLAG_TIMER] < 40) {
			ss_etape_warning++;
			if (game_flag[FLAG_TIMER] / 3 > 0) {
				ss_etape_warning %= game_flag[FLAG_TIMER] / 3;
			} else {
				ss_etape_warning = 0;
			}

			if (ss_etape_warning == 0) {
				etape_warning++;
				etape_warning %= 10;
			}
		}
		seconde = game_flag[FLAG_TIMER];
		centieme = etape_compteur;
	} else if (game_flag[0] == 3) {
		game_flag[FLAG_TIMER] = save_time;
	}
}

void BackgroundBomb::render() {
	Sprite::render();
	if (armed) {
		draw(x, y, pbk_level[121 + anim_step]);

		draw(x, y, pbk_level[anim_nuke[etape_nuke]]);

		if (game_flag[FLAG_TIMER] >= 100) {
			draw(x + 6, y - 27, pbk_level[80]);
			draw(x + 11, y - 27, pbk_level[80]);
			draw(x + 16, y - 27, pbk_level[80]);
			draw(x + 21, y - 27, pbk_level[80]);
		} else {
			draw(x + 6, y - 27, pbk_level[71 + game_flag[FLAG_TIMER] / 10]);
			draw(x + 11, y - 27, pbk_level[71 + game_flag[FLAG_TIMER] % 10]);
			draw(x + 16, y - 27, pbk_level[80 - etape_compteur / 10]);
			draw(x + 21, y - 27, pbk_level[80 - etape_compteur % 10]);
		}

		if (game_flag[FLAG_TIMER] < 40) {
			draw(x, y - 35, pbk_level[anim_warning[etape_warning]]);
		} else if (game_flag[FLAG_TIMER] < 60) {
			draw(x, y - 35, pbk_level[118]);
		}
	} else {
		draw(x, y, pbk_level[anim_nuke[etape_nuke]]);
		draw(x + 6, y - 27, pbk_level[71 + seconde / 10]);
		draw(x + 11, y - 27, pbk_level[71 + seconde % 10]);
		draw(x + 16, y - 27, pbk_level[80 - centieme / 10]);
		draw(x + 21, y - 27, pbk_level[80 - centieme % 10]);
	}
}
