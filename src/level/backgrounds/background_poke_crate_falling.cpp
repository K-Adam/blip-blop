#include "level/backgrounds/background_poke_crate_falling.h"
#include "level/backgrounds/background_poke_crate.h"
#include "level/backgrounds/background_poke_crate_platform.h"
#include "level/backgrounds/background_poke_crate_platform_2.h"

const int anim_lock[] = {0, 1, 2, 3, 4, 5, 4, 3, 2, 1};

BackgroundPokeCrateFalling::BackgroundPokeCrateFalling() { pic = pbk_level.pic("crate_hoist"); }

void BackgroundPokeCrateFalling::update() {
	if (game_flag[2] > 2) {
		if ((y < 100) && (anim_step <= UNLOCK_DELAY)) {
			y += 2;
		} else {
			anim_tick++;
			anim_tick %= 5;
			if (anim_tick == 0) {
				anim_step++;
			}

			if ((anim_step == 0) && (anim_tick == 1)) {
				BackgroundPokeCratePlatform* s2 = new BackgroundPokeCratePlatform(180);
				s2->x = x - 250;
				s2->y = -150;

				list_moving_platforms.emplace_back(s2);

				s2 = new BackgroundPokeCratePlatform(180);
				s2->x = x - 470;
				s2->y = -150;

				list_moving_platforms.emplace_back(s2);
			}

			if (anim_step > UNLOCK_DELAY + 4) {
				y -= 3;
				if (y < -160) {
					destroyed = true;
				}

				if ((anim_step == UNLOCK_DELAY + 5) && (anim_tick == 0)) {
					pic = pbk_level.pic("crate_hoist_empty");

					BackgroundPokeCrate* s = new BackgroundPokeCrate();
					s->x = x;
					s->y = y + 150;

					list_animated_bgs.emplace_back(s);

					BackgroundPokeCratePlatform2* s3 = new BackgroundPokeCratePlatform2();
					s3->x = x;
					s3->y = -150;

					list_moving_platforms.emplace_back(s3);
				}
			}
		}
	} else {
		y = -160;
	}
}

void BackgroundPokeCrateFalling::render() {
	Sprite::render();
	if (anim_step > UNLOCK_DELAY + 4) {
		draw(x, y, pbk_level.pic("crate_unlock"));
		draw(x, y + 63, pbk_level.anim("crate_claw_open")[4]);
	} else if (anim_step > UNLOCK_DELAY - 1) {
		draw(x, y + 63, pbk_level.anim("crate_claw_open")[anim_step - UNLOCK_DELAY]);
	} else if (anim_step > 0) {
		draw(x, y, pbk_level.anim("crate_lock")[anim_lock[anim_step % 10]]);
		draw(x, y + 63, pbk_level.anim("crate_lights")[anim_step % 16]);
	}
}
