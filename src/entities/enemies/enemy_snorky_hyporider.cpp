#include "entities/enemies/enemy_snorky_hyporider.h"
#include "entities/piece_snorky_head.h"

EnemySnorkyHyporider::EnemySnorkyHyporider() : wait_for_charge(0), charge_delay(50 + rand() % 200), nageoire(1), attack_step(0) {
	hp = 350;
	dy = 0;
}

void EnemySnorkyHyporider::update() {
	switch (status) {
		case ETAT_TOMBE:
		case ETAT_SAUTE:

		case ETAT_AVANCE:
		case STATE_NORMAL:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;

		case ETAT_TIRE:
			onCharge();
			break;
	}

	updateDestroyed();
}

void EnemySnorkyHyporider::onAvance() {
	wait_for_charge++;
	if ((wait_for_charge > charge_delay) && ((dir == FACING_RIGHT && x > offset + 80) || (dir == FACING_LEFT && x < offset + 560))) {
		wait_for_charge = 0;
		charge_delay = 50 + rand() % 200;
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_TIRE;
		attack_step = 0;
		onCharge();
		return;
	}

	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 4;
	}

	if (dir == FACING_LEFT) {
		x -= HYPORIDER_SPEED;
		pic = pbk_enemies[291];

		if (solid_wall(x - HYPORIDER_SPEED, y) || (x - HYPORIDER_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		x += HYPORIDER_SPEED;

		pic = pbk_enemies[286];

		if (solid_wall(x + HYPORIDER_SPEED, y) || (x + HYPORIDER_SPEED > offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkyHyporider::onMeure() {
	anim_tick += 1;
	if (anim_step < 7) {
		anim_tick %= 3;
	} else {
		anim_tick %= 5;
	}

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 7 && anim_tick == 0) {
		Sprite* s;
		if (dir == FACING_RIGHT) {
			s = new PieceSnorkyHyporiderBody(-1);
		} else {
			s = new PieceSnorkyHyporiderBody(1);
		}

		s->dir = dir;
		s->y = y;
		s->x = x;

		list_splatters.emplace_back(s);

		sbk_level.play(8);
		dy = 0;
	}
	if (anim_step >= 7) {
		nageoire = 0;
		if (dir == FACING_LEFT) {
			x -= 4;
			pic = pbk_enemies[325];
		} else {
			x += 4;
			pic = pbk_enemies[312];
		}
	} else {
		if (dir == FACING_LEFT) {
			x -= HYPORIDER_SPEED;
			pic = pbk_enemies[313 + anim_step];
		} else {
			x += HYPORIDER_SPEED;
			pic = pbk_enemies[300 + anim_step];
		}
	}
}

void EnemySnorkyHyporider::onCharge() {
	anim_tick++;
	anim_tick %= 2;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 4;
	}

	attack_step++;

	if (attack_step > 40) {
		nageoire = 0;
		if (dir == FACING_RIGHT) {
			if (solid_wall(x + HYPORIDER_SPEED, y) || (x + HYPORIDER_SPEED > offset + 640)) {
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_AVANCE;
				nageoire = 1;
				onAvance();
				return;
				dir = FACING_RIGHT;
			}

			x += HYPORIDER_CHARGE_SPEED;
			pic = pbk_enemies[297];
		} else {
			if (solid_wall(x - HYPORIDER_CHARGE_SPEED, y) || (x - HYPORIDER_CHARGE_SPEED < xmin)) {
				anim_step = 0;
				anim_tick = 0;
				status = ETAT_AVANCE;
				nageoire = 1;
				onAvance();
				return;
				dir = FACING_RIGHT;
			}

			x -= HYPORIDER_CHARGE_SPEED;
			pic = pbk_enemies[299];
		}
	} else {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[296];
		} else {
			pic = pbk_enemies[298];
		}
	}
	colFromPic();
}

void EnemySnorkyHyporider::onCarbonise() {
	nageoire = 0;
	anim_tick++;
	anim_tick %= 4;

	if (anim_tick == 0) anim_step++;

	if (dir == FACING_RIGHT) {
		x += 6;
		pic = pbk_enemies[312];
	} else {
		x -= 6;
		pic = pbk_enemies[325];
	}

	if ((x - 6 < xmin) || (x + 6 > offset + 640)) {
		destroyed = true;
	}
}

void EnemySnorkyHyporider::render() {
	Sprite::render();

	if ((status == ETAT_CARBONISE) && (anim_step < 9)) {
		if (dir == FACING_RIGHT) {
			draw(x, y, pbk_enemies[326 + anim_step]);
		} else {
			draw(x, y, pbk_enemies[335 + anim_step]);
		}
	}

	if (nageoire) {
		if (dir == FACING_RIGHT) {
			draw(x + 15, y + 20, pbk_enemies[287 + anim_step % 4]);
		} else {
			draw(x - 15, y + 20, pbk_enemies[292 + anim_step % 4]);
		}
	}
}

void EnemySnorkyHyporider::takeHit(Shot* shot) {
	static const int dx_giclure_hyporider[] = {0, 0, 7, 3, 0, -3, -7, 0};
	static const int dy_giclure_hyporider[] = {-15, -15, -15, -25, -25, -25, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_hyporider, dy_giclure_hyporider);

	if (status == ETAT_MEURE) {
		sbk_level.play(7);
	}
}
