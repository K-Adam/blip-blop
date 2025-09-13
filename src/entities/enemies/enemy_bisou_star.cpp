#include "entities/enemies/enemy_bisou_star.h"
#include "entities/gore_feet_bisou_star.h"
#include "entities/shots/shot_star.h"

EnemyBisouStar::EnemyBisouStar() : etape_shoot(0) {
	hp = 200;
	wait_shoot = 200 + rand() % 250;
}

void EnemyBisouStar::update() {
	if (blood > 0) blood -= 1;

	etape_shoot += 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_MEURE:
			onMeureEntier();
			break;

		case ETAT_MEURE_2:
			onMeureHaut();
			break;

		case ETAT_MEURE_3:
			onMeureHautFin();
			break;

		case ETAT_TIRE:
			onTire();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	updateDestroyed();
}

void EnemyBisouStar::onMeureEntier() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4) {
		GoreFeetBisouStar* pieds = new GoreFeetBisouStar();

		pieds->x = x;
		pieds->dir = dir;
		pieds->y = y;

		list_gore.emplace_back(pieds);

		y -= 19;
		dy = 0;
		anim_step = 0;
		anim_tick = 0;
		status = ETAT_MEURE_2;
		onMeureHaut();
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[114 + anim_step];
		else
			pic = pbk_enemies[118 + anim_step];
	}
}

void EnemyBisouStar::onTire() {
	anim_tick += 1;

	if (anim_tick >= 60) {
		anim_tick = 0;
		status = ETAT_AVANCE;
		wait_shoot = 200 + rand() % 500;
		;
		etape_shoot = 0;
	}
}

void EnemyBisouStar::onMeureHaut() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[122 + anim_step];
		x += 1;
	} else {
		pic = pbk_enemies[126 + anim_step];
		x -= 1;
	}

	fall();

	if (platform_y(x, y) != 0 && anim_step >= 3) {
		anim_step = anim_tick = 0;
		status = ETAT_MEURE_3;
		onMeureHautFin();
	}
}

void EnemyBisouStar::onMeureHautFin() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[106 + anim_step];
	else
		pic = pbk_enemies[110 + anim_step];

	if (anim_step >= 3) {
		engrave(x, y, pic);
		destroyed = true;
	}
}

void EnemyBisouStar::onAvance() {
	static const int SPEED = 1;

	fall();

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (x - SPEED < xmin || solid_wall(x - SPEED, y) || (platform_y(x - SPEED, y) == 0 && platform_y(x - SPEED, y + 5) == 0))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 680 || solid_wall(x + SPEED, y) || (platform_y(x + SPEED, y) == 0 && platform_y(x + SPEED, y + 5) == 0))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(SPEED);
		pic = pbk_enemies[78 + anim_step];
	} else {
		marche(-SPEED);
		pic = pbk_enemies[86 + anim_step];
	}

	etape_shoot += 1;

	if (anim_step == 4 && etape_shoot >= wait_shoot && x > offset) {
		ShotStar* s = new ShotStar();

		s->x = x;
		s->y = y - 30;
		s->dir = dir;

		list_enemy_shots.emplace_back(s);

		anim_tick = 0;
		status = ETAT_TIRE;
	}

	colFromPic();
}

void EnemyBisouStar::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 6) {
			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[559 + anim_step];
	else
		pic = pbk_enemies[566 + anim_step];
}

void EnemyBisouStar::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE) sbk_level.play((rand() % 9));
}
