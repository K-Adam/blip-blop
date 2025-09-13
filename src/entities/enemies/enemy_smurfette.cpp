#include "entities/enemies/enemy_smurfette.h"
#include "entities/shots/shot_bisou_deadly.h"

EnemySmurfette::EnemySmurfette() : etape_shoot(0) {
	wait_shoot = 100 + rand() % 100;
	hp = 200;
}

void EnemySmurfette::update() {
	if (blood > 0) blood -= 1;

	etape_shoot += 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_TIRE:
			onTire();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	if (x < offset - 20) destroyed = true;

	updateDestroyed();
}

void EnemySmurfette::onAvance() {
	static const int marche_droite[] = {315, 316, 317, 318, 319, 318, 317, 316};
	static const int marche_gauche[] = {320, 321, 322, 323, 324, 323, 322, 321};
	static const int SPEED = 1;

	fall();

	if (x - SPEED < xmin || solid_wall(x - SPEED, y) || (platform_y(x - SPEED, y) == 0 && platform_y(x - SPEED, y + 5) == 0))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y) || (platform_y(x + SPEED, y) == 0 && platform_y(x + SPEED, y + 5) == 0))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(SPEED);
		pic = pbk_enemies[animate(marche_droite, 8, 6)];
	} else {
		marche(-SPEED);
		pic = pbk_enemies[animate(marche_gauche, 8, 6)];
	}

	if (etape_shoot >= wait_shoot && x > offset + 10 && x < offset + 630) {
		anim_step = anim_tick = 0;
		status = ETAT_TIRE;
		onTire();
		return;
	}

	colFromPic();
}

void EnemySmurfette::onMeure() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step >= 11) {
			destroyed = true;
			engrave(x, y, pic);
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[325 + anim_step];
	else
		pic = pbk_enemies[337 + anim_step];
}

void EnemySmurfette::onTire() {
	static const int tire_droite[] = {301, 302, 303, 304, 305, 304, 303, 302, 301};
	static const int tire_gauche[] = {306, 307, 308, 309, 310, 309, 308, 307, 306};

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step == 4 && anim_tick == 0 && targeted_player != NULL) {
			ShotBisouDeadly* t = new ShotBisouDeadly();

			t->x = x;
			t->y = y - 25;
			t->setDir(dir);
			t->cible = targeted_player;

			list_enemy_shots.emplace_back(t);
		} else if (anim_step >= 9) {
			status = ETAT_AVANCE;
			anim_step = anim_tick = etape_shoot = 0;
			wait_shoot = 200 + rand() % 100;
			onAvance();
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[tire_droite[anim_step]];
	else
		pic = pbk_enemies[tire_gauche[anim_step]];

	colFromPic();
}

void EnemySmurfette::takeHit(Shot* shot) {
	static const int dx_giclure_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_smurf, dy_giclure_smurf);

	if (status == ETAT_MEURE) {
		sbk_level.play(21);
	}
}

void EnemySmurfette::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 6) {
		destroyed = true;
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[34 + anim_step];
		else
			pic = pbk_enemies[40 + anim_step];
	}
}
