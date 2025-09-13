#include "entities/enemies/enemy_tails.h"

EnemyTails::EnemyTails() : speed(1), etape_speed(0), fly_delay(100 + rand() % 350), wait_for_fly(0), fly(false) {
	tresor = 8;
	hp = 150;
}

void EnemyTails::update() {
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
			onTombe();
			break;

		case ETAT_TIRE:	 // ETAT_TIRE is Tails flying
			onVolle();
			break;
	}
	updateDestroyed();
}

void EnemyTails::onAvance() {
	if (platform_y(x, y) == 0) {
		status = ETAT_TOMBE;
		dy = 0;
		lat_grav = 0;
		fly = true;
		anim_step = 0;
		anim_tick = 0;
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
	wait_for_fly++;
	if (wait_for_fly >= fly_delay) {
		status = ETAT_TIRE;
		attack = false;
		speed = 0;
		anim_step = 0;
		anim_tick = 0;

		fly = true;
		x_cible = targeted_player->x;
		y_target = targeted_player->y;
		if (x_cible > x) {
			dir = FACING_RIGHT;
			direction = 1;
		} else {
			dir = FACING_LEFT;
			direction = -1;
		}
		onVolle();
		return;
	}

	anim_tick += 1;
	anim_tick %= 10 - 2 * speed;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (dir == FACING_RIGHT) {
		marche(speed);
		pic = pbk_enemies[anim_step + 80];
	} else {
		marche(-speed);
		pic = pbk_enemies[anim_step + 88];
	}
	if (speed < TAILS_SPEED) {
		etape_speed++;
		if (etape_speed >= 30) {
			etape_speed = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyTails::onMeure() {
	fall();

	if (fly) {
		int yp;
		anim_tick += 1;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 2;
		}
		if (dir == FACING_RIGHT) {
			if (!solid_wall(x + speed, y)) marche(speed);
			pic = pbk_enemies[122 + anim_step];
		} else {
			if (!solid_wall(x - speed, y)) marche(-speed);
			pic = pbk_enemies[124 + anim_step];
		}
		if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
			dy = 0;
			y = yp;
			fly = false;
			anim_step = 7;
		}
	} else {
		anim_tick += 1;
		anim_tick %= 6;

		if (anim_tick == 0 && anim_step < 11) anim_step += 1;

		if (anim_step >= 11) {
			int yy = platform_y(x, y);

			if (yy != 0 && yy != y_platform[4][x]) {
				engrave(x, y, pic);
				destroyed = true;
			} else {
				if (dir == FACING_LEFT && !solid_wall(x - speed, y))
					marche(-speed);
				else if (!solid_wall(x + speed, y))
					marche(speed);
			}
		} else {
			if (dir == FACING_LEFT) {
				if (!solid_wall(x - speed, y)) marche(-speed);
				pic = pbk_enemies[111 + anim_step];
			} else {
				if (!solid_wall(x + speed, y)) marche(speed);
				pic = pbk_enemies[100 + anim_step];
			}
		}
	}
}

void EnemyTails::onVolle() {
	if (attack && (x - speed < xmin || solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
		status = ETAT_AVANCE;
		wait_for_fly = 0;
		fly_delay = 100 + rand() % 350;
		fly = false;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	} else if (attack && (x + speed > offset + 640 || solid_wall(x + speed, y))) {
		dir = FACING_LEFT;
		speed = 1;
		status = ETAT_AVANCE;
		wait_for_fly = 0;
		fly = false;
		fly_delay = 100 + rand() % 350;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	}

	anim_step += 1;
	anim_step %= 2;

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[anim_step + 96];
	} else {
		pic = pbk_enemies[anim_step + 98];
	}

	{}

	if (attack) {
		if ((y_target <= y) || (y >= y_platform[0][x])) {
			speed = TAILS_FLY_SPEED;
		} else if ((y_target - 80 > y) && (y > y_target - 110)) {
			y += 4;
			speed = 1;
		} else if ((y_target - 60 > y) && (y >= y_target - 80)) {
			y += 3;
			speed = 2;
		} else if ((y_target - 40 > y) && (y >= y_target - 60)) {
			y += 2;
			speed = 3;
		} else if ((y_target - 20 > y) && (y >= y_target - 40)) {
			y += 1;
			speed = 4;
		} else {
			y += 1;
			speed = 5;
		}
		x += speed * direction;
	} else {
		if (y < (y_target - 110)) {
			y += 5;
		} else if (y > (y_target - 100)) {
			y -= 3;
		} else {
			attack = true;
		}
	}

	colFromPic();
}

void EnemyTails::onTombe() {
	int yp;

	fall();
	anim_tick += 1;
	anim_tick %= 3;
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		fly = false;
		anim_step = 0;
		anim_tick = 0;
		onAvance();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		pic = pbk_enemies[96 + anim_step];
	} else {
		x -= speed;
		pic = pbk_enemies[98 + anim_step];
	}

	colFromPic();
}

void EnemyTails::onCarbonise() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step++;

	if (anim_step >= 11)
		destroyed = true;
	else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[412 + anim_step];
		else
			pic = pbk_enemies[423 + anim_step];
	}
}

void EnemyTails::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) {
		sbk_misc.play(10);
	}
}
