#include "entities/enemies/enemy_bisou_jedi.h"
#include "entities/shots/shot_laser_sword.h"

EnemyBisouJedi::EnemyBisouJedi() : etape_shoot(0), a_epee(true) {
	hp = 250;
	wait_shoot = 200 + rand() % 250;
}

void EnemyBisouJedi::update() {
	if (blood > 0) blood -= 1;

	if (a_epee) etape_shoot += 1;

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
	}

	updateDestroyed();
}

void EnemyBisouJedi::onAvance() {
	static const int SPEED = 1;

	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (x - SPEED < offset + 20 || solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 600 || solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(SPEED);
		pic = pbk_enemies[467 + anim_step];
	} else {
		marche(-SPEED);
		pic = pbk_enemies[475 + anim_step];
	}

	if (etape_shoot > wait_shoot && targeted_player != NULL) {
		ShotLaserSword* t = new ShotLaserSword;

		t->x = x;
		t->y = y - 40;
		t->dy = 0;
		t->dx = -20;
		t->lanceur = this;
		t->cible = targeted_player;

		list_enemy_shots.emplace_back(t);

		etape_shoot = 0;
		wait_shoot = 200 + rand() % 250;
		a_epee = false;
	}

	colFromPic();
}

void EnemyBisouJedi::onMeure() {
	if (a_epee) {
		ShotLaserSword* t = new ShotLaserSword;

		t->x = x;
		t->y = y - 40;
		t->dy = 0;
		t->dx = -20;
		t->lanceur = this;

		if (targeted_player == NULL)
			t->isdead = true;
		else
			t->cible = targeted_player;

		list_enemy_shots.emplace_back(t);

		etape_shoot = 0;
		wait_shoot = 200 + rand() % 250;
		a_epee = false;
	}

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 20) {
			if (platform_y(x, y) != 0) engrave(x, y, pic);

			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[483 + anim_step];
	else
		pic = pbk_enemies[504 + anim_step];
}

void EnemyBisouJedi::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);
}

void EnemyBisouJedi::onCarbonise() {
	if (a_epee) {
		ShotLaserSword* t = new ShotLaserSword;

		t->x = x;
		t->y = y - 40;
		t->dy = 0;
		t->dx = -20;
		t->lanceur = this;

		if (targeted_player == NULL)
			t->isdead = true;
		else
			t->cible = targeted_player;

		list_enemy_shots.emplace_back(t);

		etape_shoot = 0;
		wait_shoot = 200 + rand() % 250;
		a_epee = false;
	}

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 7) {
			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[589 + anim_step];
	else
		pic = pbk_enemies[597 + anim_step];
}

void EnemyBisouJedi::render() {
	static const int dx_epee[] = {8, 4, -1, -6, -6, -6, -1, 3};
	static const int dy_epee[] = {-19, -19, -19, -19, -18, -18, -19, -19};
	static const int etape_epee[] = {0, 1, 2, 3, 4, 3, 2, 1};

	Sprite::render();

	if (a_epee) {
		if (dir == FACING_RIGHT)
			draw(x + dx_epee[anim_step], y + dy_epee[anim_step], pbk_enemies[549 + etape_epee[anim_step]]);
		else
			draw(x - dx_epee[anim_step], y + dy_epee[anim_step], pbk_enemies[554 + etape_epee[anim_step]]);
	}
}
