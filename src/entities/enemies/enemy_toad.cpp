#include "entities/enemies/enemy_toad.h"

const int anim_toad_marche_droite[] = {0, 1, 2, 1};
const int anim_toad_marche_gauche[] = {4, 5, 6, 5};

static int wait_brain_hurts = 1000;
static bool local_phase;
static int brain_hurts = 9;

EnemyToad::EnemyToad() : speed(0), etape_speed(0), charge_delay(50 + rand() % 250), wait_for_charge(0), hokuto(rand() % 5) {
	tresor = 7;
	hp = 300;
}

void EnemyToad::update() {
	if (local_phase != phase) {
		wait_brain_hurts++;
		local_phase = phase;
	}

	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;

		case ETAT_SAUTE:
		case ETAT_TOMBE:
			onSaute();
			break;

		case ETAT_TIRE:	 // ETAT_TIRE is Toad charging
			onCharge();
			break;
	}

	updateDestroyed();
}

void EnemyToad::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		anim_step = 0;
		anim_tick = 0;
		lat_grav = 0;
		onSaute();
	}

	if (x - speed < xmin || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}
	wait_for_charge++;
	if (wait_for_charge >= charge_delay) {
		if ((platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y)) &&
			(((dir == FACING_RIGHT) && (x < targeted_player->x)) || ((dir == FACING_LEFT) && (x > targeted_player->x)))) {
			charge_etape = 0;
			speed = 0;
			anim_step = 0;
			anim_tick = 0;
			dy = 0;
			status = ETAT_TIRE;
			sbk_level.play(34);
			onCharge();
			return;
		} else {
			wait_for_charge = 0;
			charge_delay = 50 + rand() % 250;
			anim_step = 0;
			anim_tick = 0;
			status = ETAT_SAUTE;
			dy = -7;
			lat_grav = 0;  // Otherwise jump heights vary
			onSaute();
			return;
		}
	}
	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[animate(anim_toad_marche_droite, 4, 12 - 3 * speed)];
	} else {
		marche(-speed);
		pic = pbk_enemies[animate(anim_toad_marche_gauche, 4, 12 - 3 * speed)];
	}
	if (speed < TOAD_SPEED) {
		etape_speed++;
		if (etape_speed >= 30) {
			etape_speed = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyToad::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 10) anim_step += 1;
	if ((anim_step > 4) && (hokuto > 0)) {
		hokuto--;
		anim_step -= 4;
	}

	if (anim_step == 5 && anim_tick == 0) {
		sbk_misc.play(9 + rand() % 2);
	}

	if (anim_step >= 10) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) {
			engrave(x, y, pic);
			destroyed = true;
		}
	} else {
		if (dir == FACING_LEFT) {
			if (!solid_wall(x - speed, y) && platform_y(x, y) == 0) x -= speed;
			pic = pbk_enemies[18 + anim_step];
		} else {
			if (!solid_wall(x + speed, y) && platform_y(x, y) == 0) x += speed;
			pic = pbk_enemies[8 + anim_step];
		}
	}
}

void EnemyToad::onCharge() {
	speed = TOAD_CHARGE_SPEED;
	if (x - speed < xmin || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
		status = ETAT_AVANCE;
		wait_for_charge = 0;
		charge_delay = 50 + rand() % 250;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
		status = ETAT_AVANCE;
		wait_for_charge = 0;
		charge_delay = 50 + rand() % 250;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	}
	charge_etape += 1;
	if (charge_etape < 80) {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[animate(anim_toad_marche_droite, 4, 3)];
		} else {
			pic = pbk_enemies[animate(anim_toad_marche_gauche, 4, 3)];
		}
	} else {
		if (dir == FACING_RIGHT) {
			marche(speed);
			pic = pbk_enemies[animate(anim_toad_marche_droite, 4, 2)];
		} else {
			marche(-speed);
			pic = pbk_enemies[animate(anim_toad_marche_gauche, 4, 2)];
		}
	}

	colFromPic();
}

void EnemyToad::onSaute() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		pic = pbk_enemies[3];
	} else {
		x -= speed;
		pic = pbk_enemies[7];
	}

	colFromPic();
}

void EnemyToad::onCarbonise() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step >= 13)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[434 + anim_step];
		else
			pic = pbk_enemies[447 + anim_step];
	}
}

void EnemyToad::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) {
		if (wait_brain_hurts >= 140 && wait_brain_hurts <= 200) {
			wait_brain_hurts = 250;
			sbk_level.play(29);
			hokuto = 7;
		} else {
			brain_hurts += 1;
			brain_hurts %= 10;

			if (brain_hurts == 0) {
				wait_brain_hurts = 0;
				hokuto = 6;
				sbk_level.play(28);
			} else {
				sbk_level.play(30 + rand() % 2);
			}
		}
	}
}
