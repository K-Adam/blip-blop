#include "entities/enemies/enemy_snorky_base_2.h"
#include "entities/shots/shot_snorky_base_2.h"

const int anim_snorky_base_marche_droite[] = {114, 115, 116, 117, 118, 117, 116, 115};
const int anim_snorky_base_marche_gauche[] = {119, 120, 121, 122, 123, 122, 121, 120};

EnemySnorkyBase2::EnemySnorkyBase2() : wait_for_shoot(0), shoot_delay(50 + rand() % 200) {
	hp = 150;
	pic = pbk_enemies[0];
	dy = 0;
}

void EnemySnorkyBase2::update() {
	if (blood > 0) blood -= 1;

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

void EnemySnorkyBase2::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onTombe();
		return;
	}

	wait_for_shoot++;
	if ((targeted_player != NULL) && (wait_for_shoot > shoot_delay) && ((dir == FACING_RIGHT && x > offset + 80) || (dir == FACING_LEFT && x < offset + 560))) {
		wait_for_shoot = 0;
		shoot_delay = 50 + rand() % 200;
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_TIRE;
		x_cible = targeted_player->x;
		y_target = targeted_player->y;
		onTire();
		return;
	}

	if (dir == FACING_LEFT) {
		marche(-SNORKY_BASE_SPEED);

		pic = pbk_enemies[animate(anim_snorky_base_marche_gauche, 8, 4)];

		if (solid_wall(x - SNORKY_BASE_SPEED, y) || (x - SNORKY_BASE_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		marche(SNORKY_BASE_SPEED);

		pic = pbk_enemies[animate(anim_snorky_base_marche_droite, 8, 4)];

		if (solid_wall(x + SNORKY_BASE_SPEED, y) || (x + SNORKY_BASE_SPEED > offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkyBase2::onTombe() {
	fall();

	if (platform_y(x, y) != 0) status = ETAT_AVANCE;

	colFromPic();
}

void EnemySnorkyBase2::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if ((anim_step == 5) && (anim_tick == 0)) {
		dy = 0;
		y -= 17;

		if (dir == FACING_RIGHT)
			x += 10;

		else
			x -= 10;
	}
	if (anim_step >= 5) {
		fall();

		if (dir == FACING_RIGHT)
			x += 1;

		else
			x -= 1;
	}

	if (anim_step == 10) {
		if (platform_y(x, y) != 0) engrave(x, y, pic);

		destroyed = true;
	}

	if (dir == FACING_LEFT) {
		pic = pbk_enemies[154 + anim_step];
	} else {
		pic = pbk_enemies[144 + anim_step];
	}
}

void EnemySnorkyBase2::onTire() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step == 12) {
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_AVANCE;
		onAvance();
		return;
	}

	if (anim_step == 10 && anim_tick == 0) {
		if (dir == FACING_RIGHT) {
			if ((x_cible > x) && (x_cible - x != 0)) {
				int vy = ((y_target - y + 50) * 6) / (x_cible - x);
				if (vy > 3) vy = 3;

				if (vy < -3) vy = -3;

				ShotSnorkyBase2* shot = new ShotSnorkyBase2(6, vy);
				shot->setDir(dir);
				shot->x = x + 35;
				shot->y = y - 42;

				list_enemy_shots.emplace_back(shot);

			} else {
				ShotSnorkyBase2* shot = new ShotSnorkyBase2(6, 1);
				shot->setDir(dir);
				shot->x = x + 35;
				shot->y = y - 42;

				list_enemy_shots.emplace_back(shot);
			}
			pic = pbk_enemies[124];
		} else {
			if ((x_cible < x) && (x_cible - x != 0)) {
				int vy = ((y_target - y + 50) * 6) / (x_cible - x);
				if (vy > 3) vy = 3;

				if (vy < -3) vy = -3;

				ShotSnorkyBase2* shot = new ShotSnorkyBase2(-6, -vy);
				shot->setDir(dir);
				shot->x = x - 35;
				shot->y = y - 42;

				list_enemy_shots.emplace_back(shot);

			} else {
				ShotSnorkyBase2* shot = new ShotSnorkyBase2(-6, 1);
				shot->setDir(dir);
				shot->x = x - 35;
				shot->y = y - 42;

				list_enemy_shots.emplace_back(shot);
			}
			pic = pbk_enemies[134];
		}
	}

	if (anim_step < 10) {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[124 + anim_step];
		} else {
			pic = pbk_enemies[134 + anim_step];
		}
	}
	colFromPic();
}

void EnemySnorkyBase2::onCarbonise() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 9) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[173 + anim_step];
		else
			pic = pbk_enemies[164 + anim_step];
	}
}

void EnemySnorkyBase2::takeHit(Shot* shot) {
	static const int dx_giclure_snorkbase2[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_snorkbase2[] = {-15, -15, -15, -15, -35, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_snorkbase2, dy_giclure_snorkbase2);

	if (status == ETAT_MEURE) {
		sbk_level.play(18);
	}
}
