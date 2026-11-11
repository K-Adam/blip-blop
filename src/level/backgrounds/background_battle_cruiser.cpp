#include "level/backgrounds/background_battle_cruiser.h"
#include "entities/sprite.h"
#include "entities/shots/shot_cruiser.h"
#include "entities/shots/shot_cruiser_laser.h"

const int anim_cruiser_right[] = {0, 1, 2, 1};
const int anim_cruiser_left[] = {0, 1, 2, 1};

BackgroundBattleCruiser::BackgroundBattleCruiser() : x_opponent(700), y_opponent(40), attack_step(-2), fire_step(0), fire_tick(0) {
	x = -20;
	y = 80;
}

void BackgroundBattleCruiser::update() {
	fire_tick += 1;
	fire_tick %= 40;
	if (fire_tick == 0) {
		fire_step += 1;
		fire_step %= 6;
	}
	{
		x += 1;
		x_opponent -= 1;
		pic = assets().pics.level.anim("cruiser_fly_right")[animate(anim_cruiser_right, 4, 12)];

		int dif_y = y_opponent - y;
		int dif_x = x_opponent - x;

		if (dif_y != 0 && (dif_x / dif_y < 2) && (dif_x > 0) && (dif_x != 0)) {
			attack_step += 1;

			if (attack_step == 2) attack_step = -2;

			pic = assets().pics.level.anim("cruiser_attack_right")[2 + attack_step];
			Sprite* s = new ShotCruiser((dif_x * SHOT_SPEED) / dif_y, static_cast<int>(SHOT_SPEED - (dif_x * SHOT_SPEED * 0.25) / dif_y),
										assets().pics.level.anim("cruiser_shot")[2 + attack_step]);
			s->x = x - 8 * attack_step;
			s->y = y + 6;
			level().entities.animated_bgs.emplace_back(s);

			s = new ShotCruiser(-(dif_x * SHOT_SPEED) / dif_y, static_cast<int>(-SHOT_SPEED + (dif_x * SHOT_SPEED * 0.25) / dif_y),
								assets().pics.level.anim("cruiser_shot")[1 - attack_step]);
			s->x = x_opponent - 8 * attack_step;
			s->y = y_opponent - 6;
			level().entities.animated_bgs.emplace_back(s);

			if (attack_step == -2) {
				s = new ShotCruiserLaser(1, assets().pics.level.anim("cruiser_laser_right")[3 - (dif_x / dif_y)]);
				s->x = x + 16;
				s->y = y;
				level().entities.animated_bgs.emplace_back(s);
			}
			if (attack_step == 0) {
				s = new ShotCruiserLaser(-1, assets().pics.level.anim("cruiser_laser_left")[3 - (dif_x / dif_y)]);
				s->x = x_opponent - 16;
				s->y = y_opponent;
				level().entities.animated_bgs.emplace_back(s);
			}

		} else {
			attack_step = -3;
		}
	}

	if (x > 720) {
		x = -20;
		attack_step = 1;
		x_opponent = 700;
	}
}

void BackgroundBattleCruiser::render() {
	Sprite::render();
	if (attack_step == -3) {
		draw(x_opponent, y_opponent, assets().pics.level.anim("cruiser_fly_left")[anim_cruiser_left[anim_step]]);
	} else {
		draw(x_opponent, y_opponent, assets().pics.level.anim("cruiser_attack_left")[1 - attack_step]);
	}

	if (((fire_tick >= 0) && (fire_tick <= 2)) || ((fire_tick >= 10) && (fire_tick <= 12))) {
		int tmp = 0;

		if ((fire_tick == 1) || (fire_tick == 11)) tmp = 1;

		switch (fire_step) {
			case 0:
				draw(x + 21, y - 2, assets().pics.level.anim("cruiser_muzzle_right")[tmp]);
				draw(x_opponent - 20, y_opponent - 2, assets().pics.level.anim("cruiser_muzzle_left")[tmp]);
				break;

			case 1:
				draw(x - 10, y - 7, assets().pics.level.anim("cruiser_muzzle_right")[tmp]);
				draw(x_opponent + 11, y_opponent - 7, assets().pics.level.anim("cruiser_muzzle_left")[tmp]);
				break;

			case 2:
				draw(x + 8, y - 6, assets().pics.level.anim("cruiser_muzzle_right")[tmp]);
				draw(x_opponent - 7, y_opponent - 6, assets().pics.level.anim("cruiser_muzzle_left")[tmp]);
				break;

			case 3:
				draw(x - 8, y + 6, assets().pics.level.anim("cruiser_muzzle_right")[tmp]);
				draw(x_opponent + 7, y_opponent + 6, assets().pics.level.anim("cruiser_muzzle_left")[tmp]);
				break;

			case 4:
				draw(x - 10, y - 7, assets().pics.level.anim("cruiser_muzzle_right")[tmp]);
				draw(x_opponent + 11, y_opponent - 7, assets().pics.level.anim("cruiser_muzzle_left")[tmp]);
				break;

			case 5:
				draw(x + 21, y + 2, assets().pics.level.anim("cruiser_muzzle_right")[tmp]);
				draw(x_opponent - 20, y_opponent + 2, assets().pics.level.anim("cruiser_muzzle_left")[tmp]);
				break;
		}
	}
}
