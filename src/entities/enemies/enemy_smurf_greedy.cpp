#include "entities/enemies/enemy_smurf_greedy.h"
#include "entities/shots/shot_cake.h"

#define ETAT_HOLD 15

EnemySmurfGreedy::EnemySmurfGreedy() : wait_for_shoot(0) {
	hp = 150;
	shoot_delay = 50 + rand() % 100;
}

void EnemySmurfGreedy::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
			onNormal();
			break;

		case ETAT_TIRE:
			onTire();
			break;

		case ETAT_HOLD:
			onHold();
			break;

		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	if (x < offset - 20) destroyed = true;

	updateDestroyed();
}

void EnemySmurfGreedy::onNormal() {
	fall();

	wait_for_shoot += 1;

	if (wait_for_shoot > shoot_delay && x > offset && x < offset + 640 && targeted_player != NULL) {
		sbk_level.play(14 + rand() % 2);
		status = ETAT_TIRE;
		wait_for_shoot = 0;
		shoot_delay = 100 + rand() % 250;
		anim_step = 0;
		anim_tick = 0;
		xcible = targeted_player->x;
		ycible = targeted_player->y;

		if (xcible > x)
			dir = FACING_RIGHT;
		else
			dir = FACING_LEFT;

		onTire();
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[182];
	} else {
		pic = pbk_enemies[188];
	}

	colFromPic();
}

void EnemySmurfGreedy::onTire() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 5) {
		ShotCake* shot = new ShotCake();

		shot->setDir(dir);

		if (dir == FACING_LEFT)
			shot->x = x - 13;
		else
			shot->x = x + 13;

		shot->y = y - 28;

		tirEnCloche(shot->x, shot->y, xcible, ycible, shot->dx, shot->dy);

		list_enemy_shots.emplace_back(shot);

		anim_step = 0;
		status = ETAT_HOLD;
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[182 + anim_step];
	} else {
		pic = pbk_enemies[188 + anim_step];
	}

	colFromPic();
}

void EnemySmurfGreedy::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 11) anim_step += 1;

	if (dir == FACING_RIGHT) {
		if (!solid_wall(x - 2, y)) x -= 2;

		pic = pbk_enemies[195 + anim_step];
	} else {
		if (!solid_wall(x + 2, y)) x += 2;

		pic = pbk_enemies[207 + anim_step];
	}

	if (anim_step >= 11 && platform_y(x, y) != 0) {
		destroyed = true;
		engrave(x, y, pic);
	}
}

void EnemySmurfGreedy::takeHit(Shot* shot) {
	static const int dx_giclure_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_smurf, dy_giclure_smurf);

	if (shot->getGiclureDir() < 4)
		dir = FACING_LEFT;
	else
		dir = FACING_RIGHT;

	if (status == ETAT_MEURE) sbk_level.play(rand() % 4);
}

void EnemySmurfGreedy::onHold() {
	anim_step += 1;

	if (anim_step >= 50) {
		anim_step = 0;
		status = STATE_NORMAL;
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[187];
	} else {
		pic = pbk_enemies[193];
	}

	colFromPic();
}

void EnemySmurfGreedy::onCarbonise() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 6) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies[34 + anim_step];
		else
			pic = pbk_enemies[40 + anim_step];
	}
}
