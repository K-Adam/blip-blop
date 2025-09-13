#include "entities/enemies/enemy_knuckles.h"

EnemyKnuckles::EnemyKnuckles() : speed(1), etape_speed(0), charge_delay(100 + rand() % 150), wait_for_charge(0) {
	tresor = 7;
	hp = 150;
}

void EnemyKnuckles::update() {
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
			onSaute();
			break;

		case ETAT_TOMBE:
			onTombe();
			break;

		case ETAT_TIRE:	 // ETAT_TIRE is Knuckles charging
			onCharge();
			break;
	}
	updateDestroyed();
}

void EnemyKnuckles::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		onTombe();
		return;
	}

	if (x - speed < xmin || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > offset + 640 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}
	wait_for_charge++;
	if (wait_for_charge >= charge_delay && targeted_player != NULL) {
		if ((platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y)) &&
			(((dir == FACING_RIGHT) && (x > (targeted_player->x - 200)) && (targeted_player->x > x)) ||
			 ((dir == FACING_LEFT) && (x < (targeted_player->x + 200)) && (targeted_player->x < x)))) {
			status = ETAT_TIRE;
			sbk_level.play(6);
			speed = KNUKLES_CHARGE_SPEED;
			anim_step = 0;
			anim_tick = 0;
			onCharge();
			return;
		} else if ((targeted_player->y < y) && (targeted_player->x > x - 30) && (targeted_player->x < x + 30)) {
			status = ETAT_SAUTE;
			speed = 0;
			anim_step = 0;
			anim_tick = 0;
			lat_grav = 0;  // Otherwise jump heights vary
			dy = 0;
			sbk_level.play(7);
			onSaute();
			return;
		} else {
			wait_for_charge = 0;
			charge_delay = 10 + rand() % 10;
		}
	}

	anim_tick += 1;
	anim_tick %= 10 - 2 * speed;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[anim_step + 152];
	} else {
		marche(-speed);
		pic = pbk_enemies[anim_step + 160];
	}
	if (speed < KNUKLES_SPEED) {
		etape_speed++;
		if (etape_speed >= 20) {
			etape_speed = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyKnuckles::onMeure() {
	fall();
	{
		anim_tick += 1;
		anim_tick %= 6;

		if (anim_tick == 0 && anim_step < 10) anim_step += 1;

		if (anim_step >= 10) {
			int yy = platform_y(x, y);

			if (yy != 0) {
				engrave(x, y, pic);
				destroyed = true;
			} else {
				if (dir == FACING_LEFT && !solid_wall(x - speed, y))
					x -= speed;
				else if (!solid_wall(x + speed, y))
					x += speed;
			}
		} else {
			if (dir == FACING_LEFT) {
				if (!solid_wall(x - speed, y)) x -= speed;
				pic = pbk_enemies[514 + anim_step];
			} else {
				if (!solid_wall(x + speed, y)) x += speed;
				pic = pbk_enemies[504 + anim_step];
			}
		}
	}
}

void EnemyKnuckles::onSaute() {
	anim_tick++;
	anim_tick %= 60;
	if ((anim_tick == 0) && (anim_step == 0)) {
		anim_step++;
		dy = -KNUKLES_JUMP_SPEED;
	}

	fall();

	int yp;
	if ((dy > 0 && (yp = platform_y(x, y + dy)) != 0) && anim_step > 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		speed = 1;
		wait_for_charge = 0;
		anim_step = 0;
		anim_tick = 0;
		charge_delay = 50 + rand() % 150;
		onAvance();
		return;
	}
	if (dir == FACING_LEFT) {
		pic = pbk_enemies[170 + anim_step];
	} else {
		pic = pbk_enemies[168 + anim_step];
	}
	colFromPic();
}

void EnemyKnuckles::onCharge() {
	fall();
	anim_tick++;
	anim_tick %= 40;
	if (anim_tick % 10 == 0) {
		speed--;
	}
	if (anim_step == 2) {
		status = ETAT_AVANCE;
		speed = 1;
		wait_for_charge = 0;
		charge_delay = 50 + rand() % 150;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	}
	if (anim_tick == 0) {
		anim_step++;
		speed = KNUKLES_CHARGE_SPEED - 1;
	}

	if (dir == FACING_LEFT) {
		if (!solid_wall(x - speed, y)) {
			marche(-speed);
		} else {
			status = ETAT_AVANCE;
			wait_for_charge = 0;
			charge_delay = 50 + rand() % 150;
			anim_step = 0;
			anim_tick = 0;
			onAvance();
			return;
		}
		pic = pbk_enemies[150 + anim_step];
	} else {
		if (!solid_wall(x + speed, y)) {
			marche(speed);
		} else {
			status = ETAT_AVANCE;
			wait_for_charge = 0;
			charge_delay = 50 + rand() % 150;
			anim_step = 0;
			anim_tick = 0;
			onAvance();
			return;
		}
		pic = pbk_enemies[148 + anim_step];
	}
	colFromPic();
}

void EnemyKnuckles::onTombe() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;

		onAvance();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		pic = pbk_enemies[152];
	} else {
		x -= speed;
		pic = pbk_enemies[160];
	}

	colFromPic();
}

void EnemyKnuckles::onCarbonise() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step++;

	if (anim_step >= 10)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[392 + anim_step];
		else
			pic = pbk_enemies[402 + anim_step];
	}
}

void EnemyKnuckles::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) {
		sbk_misc.play(10);
	}
}

void EnemyKnuckles::fall() {
	lat_grav += 1;
	lat_grav %= GRAVITY_DELAY;

	if (lat_grav == 0 && dy < KNUKLES_GRAVITE_MAX) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = KNUKLES_GRAVITE_MAX;

	int ny = platform_y(x, y + dy);

	if (ny != 0 && dy > 0)
		y = ny;
	else
		y += dy;
}
