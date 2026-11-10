#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_bomb.h"
#include "entities/bonus_weapon.h"
#include "entities/enemies/enemy.h"
#include "entities/bonus_item.h"

namespace {
// Timer left when the cow bonus stopped the countdown, restored once the cow is done
struct BombTimer {
	int saved = 0;
};
}  // namespace

const int anim_warning[] = {0, 0, 0, 0, 0, 1, 1, 2, 1, 1};
const int anim_nuke[] = {0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0};

BackgroundBomb::BackgroundBomb() : counter_step(0), armed(true), warning_step(0), warning_tick(0), nuke_step(0), nuke_tick(0) {
	pic = assets().pics.level.pic("bomb");
}

void BackgroundBomb::update() {
	if (armed) {
		if (session().targeted_player != NULL) {
			if ((session().targeted_player->x > x - 50) && (session().targeted_player->x < x + 50) && (session().targeted_player->y > y - 10) &&
				(session().targeted_player->y < y + 51)) {
				level().rules.flags[0]++;
				armed = false;
				Bonus* bonus = nullptr;
				switch (level().rules.flags[0]) {
					case 1:
						bonus = new BonusShotgun();
						break;

					case 2:
						bonus = new BonusPM();
						break;

					case 3:
						bonus = new BonusCow();
						level().shared<BombTimer>().saved = level().rules.flags[FLAG_TIMER];
						level().rules.flags[FLAG_GEN_OFF] = 1;
						break;
				}

				if (bonus) {
					bonus->x = x;
					bonus->y = 0;
					level().entities.bonus.emplace_back(bonus);
				}

				assets().sounds.level.play(1);
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

		if (level().rules.flags[FLAG_TIMER] < 40) {
			warning_tick++;
			if (level().rules.flags[FLAG_TIMER] / 3 > 0) {
				warning_tick %= level().rules.flags[FLAG_TIMER] / 3;
			} else {
				warning_tick = 0;
			}

			if (warning_tick == 0) {
				warning_step++;
				warning_step %= 10;
			}
		}
		seconds = level().rules.flags[FLAG_TIMER];
		hundredths = counter_step;
	} else if (level().rules.flags[0] == 3) {
		level().rules.flags[FLAG_TIMER] = level().shared<BombTimer>().saved;
	}
}

void BackgroundBomb::render() {
	Sprite::render();
	if (armed) {
		draw(x, y, assets().pics.level.anim("bomb_lights")[anim_step]);

		draw(x, y, assets().pics.level.anim("bomb_nuke_sign")[anim_nuke[nuke_step]]);

		if (level().rules.flags[FLAG_TIMER] >= 100) {
			draw(x + 6, y - 27, assets().pics.level.anim("bomb_digits")[9]);
			draw(x + 11, y - 27, assets().pics.level.anim("bomb_digits")[9]);
			draw(x + 16, y - 27, assets().pics.level.anim("bomb_digits")[9]);
			draw(x + 21, y - 27, assets().pics.level.anim("bomb_digits")[9]);
		} else {
			draw(x + 6, y - 27, assets().pics.level.anim("bomb_digits")[level().rules.flags[FLAG_TIMER] / 10]);
			draw(x + 11, y - 27, assets().pics.level.anim("bomb_digits")[level().rules.flags[FLAG_TIMER] % 10]);
			draw(x + 16, y - 27, assets().pics.level.anim("bomb_digits")[9 - counter_step / 10]);
			draw(x + 21, y - 27, assets().pics.level.anim("bomb_digits")[9 - counter_step % 10]);
		}

		if (level().rules.flags[FLAG_TIMER] < 40) {
			draw(x, y - 35, assets().pics.level.anim("bomb_warning")[anim_warning[warning_step]]);
		} else if (level().rules.flags[FLAG_TIMER] < 60) {
			draw(x, y - 35, assets().pics.level.anim("bomb_warning")[0]);
		}
	} else {
		draw(x, y, assets().pics.level.anim("bomb_nuke_sign")[anim_nuke[nuke_step]]);
		draw(x + 6, y - 27, assets().pics.level.anim("bomb_digits")[seconds / 10]);
		draw(x + 11, y - 27, assets().pics.level.anim("bomb_digits")[seconds % 10]);
		draw(x + 16, y - 27, assets().pics.level.anim("bomb_digits")[9 - hundredths / 10]);
		draw(x + 21, y - 27, assets().pics.level.anim("bomb_digits")[9 - hundredths % 10]);
	}
}
