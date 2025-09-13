#include "entities/enemies/enemy_snorky_mage.h"
#include "entities/shots/shot_snorky_mage.h"

const int snorky_mage_oscillation[] = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0};

EnemySnorkyMage::EnemySnorkyMage() : wait_for_shoot(0), shoot_delay(50 + rand() % 200), oscillation_step(0) {
	hp = 200;
	pic = pbk_enemies[0];
	dy = 0;
}

void EnemySnorkyMage::update() {
	switch (status) {
		case STATE_WALK:
		case STATE_NORMAL:
			onWalk();
			break;

		case STATE_FALL:
		case STATE_JUMP:
			onFall();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;

		case STATE_SHOOT:
			onShoot();
			break;
	}

	updateDestroyed();
}

void EnemySnorkyMage::onWalk() {
	oscillation_step++;
	oscillation_step %= 43;
	if (oscillation_step >= 22) {
		y -= snorky_mage_oscillation[oscillation_step - 22];
	} else {
		y += snorky_mage_oscillation[oscillation_step];
	}

	wait_for_shoot++;
	if ((targeted_player != NULL) && (wait_for_shoot > shoot_delay) && ((dir == FACING_RIGHT && x > offset + 30) || (dir == FACING_LEFT && x < offset + 610)))

	{
		int dif_y = targeted_player->y - y;
		int dif_x = targeted_player->x - x;

		if ((dir == FACING_RIGHT) && (dif_y > 0) && (dif_x > 0) && (dif_y - dif_x >= -30) && (dif_y - dif_x <= 30)) {
			anim_step = 0;
			wait_for_shoot = 0;
			shoot_delay = 50 + rand() % 200;
			anim_step = 0;
			anim_tick = 0;
			status = STATE_SHOOT;
			concentration = 0;
			onShoot();
			return;
		} else if ((dir == FACING_LEFT) && (dif_y > 0) && (dif_x < 0) && (dif_y + dif_x >= -30) && (dif_y + dif_x <= 30)) {
			anim_step = 0;
			wait_for_shoot = 0;
			shoot_delay = 50 + rand() % 200;
			anim_step = 0;
			anim_tick = 0;
			status = STATE_SHOOT;
			concentration = 0;
			onShoot();
			return;
		}
	}

	anim_tick++;
	anim_tick %= 5;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 6;
	}

	pic = pbk_enemies[237 + anim_step];

	if (dir == FACING_LEFT) {
		x -= SNORKY_MAGE_SPEED;

		if (solid_wall(x - SNORKY_MAGE_SPEED, y) || (x - SNORKY_MAGE_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		x += SNORKY_MAGE_SPEED;

		if (solid_wall(x + SNORKY_MAGE_SPEED, y) || (x + SNORKY_MAGE_SPEED > offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkyMage::onFall() {
	fall();

	if (platform_y(x, y) != 0) status = STATE_WALK;

	colFromPic();
}

void EnemySnorkyMage::onDie() {
	if (anim_step > 2) {
		fall();
	}

	anim_tick++;
	if (anim_step < 2) {
		anim_tick %= 17;
	} else {
		anim_tick %= 5;
	}

	if (anim_tick == 0) anim_step += 1;

	if (anim_tick == 0 && anim_step == 3) {
		sbk_level.play(13);
	}

	if ((platform_y(x, y) != 0) && (anim_step < 7)) {
		anim_step = 8;
	}
	if (anim_step == 7) {
		anim_step = 4;
	}

	if (anim_step == 11) {
		if (platform_y(x, y) != 0) engrave(x, y, pic);

		destroyed = true;
	} else {
		pic = pbk_enemies[266 + anim_step];
	}
}

void EnemySnorkyMage::onShoot() {
	if (concentration < 3) {
		anim_tick++;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step++;
		}

		if (anim_step == 6) {
			concentration++;
			anim_step = 0;
			if (concentration == 3) {
				anim_step = 0;
				anim_tick = 0;
			}
		}

		pic = pbk_enemies[243 + anim_step];
	} else {
		anim_tick++;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step++;
		}
		if ((anim_step == 2) && (anim_tick == 0)) {
			if (dir == FACING_RIGHT) {
				ShotSnorkyMage* shot = new ShotSnorkyMage();

				shot->setDir(dir);
				shot->x = x + 35;
				shot->y = y - 42;

				list_enemy_shots.emplace_back(shot);
			} else {
				ShotSnorkyMage* shot = new ShotSnorkyMage();

				shot->setDir(dir);
				shot->x = x - 35;
				shot->y = y - 42;

				list_enemy_shots.emplace_back(shot);
			}

			sbk_level.play(12);
		}

		if (anim_step == 5) {
			anim_step = 0;
			anim_tick = 0;
			status = STATE_WALK;
			onWalk();
			return;
		}

		if (anim_step < 3) {
			if (dir == FACING_RIGHT) {
				pic = pbk_enemies[249 + anim_step];
			} else {
				pic = pbk_enemies[257 + anim_step];
			}
		}
	}
}

void EnemySnorkyMage::onCharred() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 7) {
		destroyed = true;
	} else {
		pic = pbk_enemies[278 + anim_step];
	}
}

void EnemySnorkyMage::takeHit(Shot* shot) {
	static const int dx_splatter_mage[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_mage[] = {-35, -45, -45, -45, -55, -45, -45, -45};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_mage, dy_splatter_mage);

	if (status == STATE_DYING) {
		sbk_level.play(10);
	}
}
