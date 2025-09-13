#include "entities/enemies/enemy_ember_hero.h"

EnemyEmberHero::EnemyEmberHero() : dorkeball(2) {
	hp = 1000;
	dy = -7;
	dx = -1;
}

void EnemyEmberHero::update() {
	if (dorkeball == 2) {
		x += dx;
		fall();
		int yp;
		if ((dy > 0) && (yp = platform_y(x, y + dy)) != 0) {
			status = STATE_NORMAL;
			dy = 0;
			y = yp;
			anim_step = 0;
			anim_tick = 0;
			dorkeball = 1;
			return;
		} else {
			anim_tick++;
			anim_tick %= 5;
			if (anim_tick == 0) {
				anim_step++;
				anim_step %= 4;
			}
			pic = pbk_enemies[272 + anim_step];
		}
	} else if (dorkeball == 1) {
		anim_tick++;
		anim_tick %= 8;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step > 3) {
				dorkeball = 0;
				anim_step = 0;
				anim_tick = 0;
			} else {
				pic = pbk_enemies[380 + anim_step];
			}
		}

	} else {
		if (blood > 0) blood -= 1;

		switch (status) {
			case STATE_NORMAL:
			case ETAT_AVANCE:
				onAvance();
				break;

			case ETAT_MEURE:
			case ETAT_CARBONISE:
				onMeure();
				break;
		}

		updateDestroyed();
	}
}

void EnemyEmberHero::onAvance() {
	fall();

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (x - 1 < xmin || solid_wall(x - 1, y))
		dir = FACING_RIGHT;
	else if (x + 1 > offset + 640 || solid_wall(x + 1, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(2);
		pic = pbk_enemies[306 + anim_step];
	} else {
		marche(-2);
		pic = pbk_enemies[308 + anim_step];
	}
	colFromPic();
}

void EnemyEmberHero::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0 && anim_step < 14) anim_step += 1;

	if (anim_step >= 14) {
		game_flag[1]--;
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[344 + anim_step];
		else
			pic = pbk_enemies[330 + anim_step];
	}
}

void EnemyEmberHero::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	if (dorkeball == 0) {
		Enemy::takeHit(shot);
		gicle(shot, dx_giclure, dy_giclure);
	}
}
