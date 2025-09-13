#include "entities/enemies/enemy_snorky_mage.h"
#include "entities/shots/shot_snorky_mage.h"

const int snorky_mage_oscilation[] = {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0};

EnemySnorkyMage::EnemySnorkyMage() : wait_for_shoot(0), shoot_delay(50 + rand() % 200), oscilation_etape(0) {
	hp = 200;
	pic = pbk_enemies[0];
	dy = 0;
}

void EnemySnorkyMage::update() {
	switch (status) {
		case ETAT_AVANCE:
		case STATE_NORMAL:
			onAvance();
			break;

		case ETAT_TOMBE:
		case ETAT_SAUTE:
			onTombe();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;

		case ETAT_TIRE:
			onTire();
			break;
	}

	updateDestroyed();
}

void EnemySnorkyMage::onAvance() {
	oscilation_etape++;
	oscilation_etape %= 43;
	if (oscilation_etape >= 22) {
		y -= snorky_mage_oscilation[oscilation_etape - 22];
	} else {
		y += snorky_mage_oscilation[oscilation_etape];
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
			status = ETAT_TIRE;
			concentration = 0;
			onTire();
			return;
		} else if ((dir == FACING_LEFT) && (dif_y > 0) && (dif_x < 0) && (dif_y + dif_x >= -30) && (dif_y + dif_x <= 30)) {
			anim_step = 0;
			wait_for_shoot = 0;
			shoot_delay = 50 + rand() % 200;
			anim_step = 0;
			anim_tick = 0;
			status = ETAT_TIRE;
			concentration = 0;
			onTire();
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

void EnemySnorkyMage::onTombe() {
	fall();

	if (platform_y(x, y) != 0) status = ETAT_AVANCE;

	colFromPic();
}

void EnemySnorkyMage::onMeure() {
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

void EnemySnorkyMage::onTire() {
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
			status = ETAT_AVANCE;
			onAvance();
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

void EnemySnorkyMage::onCarbonise() {
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
	static const int dx_giclure_mage[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_mage[] = {-35, -45, -45, -45, -55, -45, -45, -45};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_mage, dy_giclure_mage);

	if (status == ETAT_MEURE) {
		sbk_level.play(10);
	}
}
