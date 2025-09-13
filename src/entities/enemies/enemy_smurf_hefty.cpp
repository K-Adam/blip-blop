#include "entities/enemies/enemy_smurf_hefty.h"
#include "entities/shots/shot_hadoken.h"

#define ETAT_TIRE_HADOKEN 1000
#define ETAT_TIRE_DPUNCH 1001
#define ETAT_TIRE_HKICK 1002

EnemySmurfHefty::EnemySmurfHefty() : etape_shoot(0), dx(0) {
	wait_shoot = 50 + rand() % 50;
	hp = 2000;
}

void EnemySmurfHefty::update() {
	if (blood > 0) blood -= 1;

	if (game_flag[0] != 0) {
		etape_shoot += 1;

		if (game_flag[2] == 0) {
			mbk_level.play(2);
			game_flag[2] = 1;
			game_flag[FLAG_TIMER] = 40;
		}
	}

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_TIRE_HADOKEN:
			onTireHadoken();
			break;

		case ETAT_TIRE_DPUNCH:
			onTireDragonPunch();
			break;

		case ETAT_TIRE_HKICK:
			onTireHurricanKick();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	updateDestroyed();
}

void EnemySmurfHefty::onAvance() {
	static const int marche_droite[] = {265, 266, 267, 268, 267, 266};
	static const int marche_gauche[] = {269, 270, 271, 272, 271, 270};
	static const int SPEED = 1;

	fall();

	dx = 0;

	if (x - SPEED < xmin || solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(SPEED);
		pic = pbk_enemies[animate(marche_droite, 6, 6)];
	} else {
		marche(-SPEED);
		pic = pbk_enemies[animate(marche_gauche, 6, 6)];
	}

	if (etape_shoot >= wait_shoot && platform_y(x, y) != 0) {
		onTire();
		return;
	}

	colFromPic();
}

void EnemySmurfHefty::takeHit(Shot* shot) {
	static const int dx_giclure_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_smurf, dy_giclure_smurf);

	if (hp <= 0) {
		game_flag[1]++;
	}
}

void EnemySmurfHefty::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 6) {
		destroyed = true;
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[253 + anim_step];
		else
			pic = pbk_enemies[259 + anim_step];
	}
}

void EnemySmurfHefty::onMeure() {
	fall();

	int x2;

	if (platform_y(x, y) == 0)
		x2 = x + dx;
	else
		x2 = x;

	if (!solid_wall(x2, y)) x = x2;

	if (anim_step < 13) {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) anim_step += 1;
	}

	if (anim_step >= 14 && platform_y(x, y) != 0) {
		destroyed = true;
		engrave(x, y, pic);
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[273 + anim_step];
		else
			pic = pbk_enemies[287 + anim_step];
	}
}

void EnemySmurfHefty::onTireHadoken() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step == 4) {
			ShotHadoken* t = new ShotHadoken();

			t->x = x;
			t->y = y - 30;
			t->setDir(dir);

			list_enemy_shots.emplace_back(t);
		}

		if (anim_step > 7) {
			anim_step = anim_tick = 0;
			status = ETAT_AVANCE;
			wait_shoot = 100 + rand() % 50;
			etape_shoot = 0;
			onAvance();
			return;
		}
	}

	if (anim_step <= 4) {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[237 + anim_step];
		else
			pic = pbk_enemies[242 + anim_step];
	}
}

void EnemySmurfHefty::onTireDragonPunch() {
	fall();

	if (anim_step <= 2) {  // Takeoff
		anim_tick += 1;
		anim_tick %= 4;

		if (anim_tick == 0) {
			anim_step += 1;

			if (anim_step > 2) dy = dypunch;
		}
	} else if (dy < 0) {
		if (dir == FACING_RIGHT && x < offset + 640 && !solid_wall(x + 1, y))
			x += 1;
		else if (dir == FACING_LEFT && x > xmin && !solid_wall(x - 1, y))
			x -= 1;
	} else {
		if (anim_step < 6) {
			anim_tick += 1;
			anim_tick %= 12;

			if (anim_tick == 0) anim_step += 1;
		}

		if (platform_y(x, y) != 0) {
			anim_tick = anim_step = 0;
			wait_shoot = 100 + rand() % 50;
			etape_shoot = 0;
			status = STATE_NORMAL;
			onAvance();
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[223 + anim_step];
	else
		pic = pbk_enemies[230 + anim_step];

	colFromPic();
}

void EnemySmurfHefty::onTireHurricanKick() {
	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 6;
	}

	int x2 = x + dx;

	if (!solid_wall(x2, y) && x2 > xmin && x2 < offset + 740) x = x2;

	if (dy > 0 && platform_y(x, y) != 0) {
		anim_tick = anim_step = 0;
		wait_shoot = 100 + rand() % 50;
		etape_shoot = 0;
		status = STATE_NORMAL;
		onAvance();
		return;
	}

	pic = pbk_enemies[247 + anim_step];

	colFromPic();
}

void EnemySmurfHefty::onTire() {
	if (targeted_player == NULL) return;

	anim_step = anim_tick = 0;

	int xc = targeted_player->x;
	int yc = targeted_player->y;

	int ddx = xc - x;
	int ddy = yc - y;

	if (ddx > 0)
		dir = FACING_RIGHT;
	else
		dir = FACING_LEFT;

	if (ddy > -50 && ddy < 50 && x > offset && x < offset + 640) {
		status = ETAT_TIRE_HADOKEN;
		sbk_level.play(19);
		onTireHadoken();
	} else if (ddx < 150 && ddx > -150 && ddy < 0) {
		status = ETAT_TIRE_DPUNCH;
		dypunch = ddy / 10;

		if (dypunch < -10) dypunch = -10;

		sbk_level.play(20);
		onTireDragonPunch();
	} else {
		tirEnCloche(x, y - 5, xc, yc, dx, dy);
		status = ETAT_TIRE_HKICK;
		onTireHurricanKick();
	}
}
