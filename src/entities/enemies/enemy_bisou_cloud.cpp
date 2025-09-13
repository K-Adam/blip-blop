#include "entities/enemies/enemy_bisou_cloud.h"
#include "entities/shots/shot_cloud.h"

EnemyBisouCloud::EnemyBisouCloud() : etape_shoot(0) {
	hp = 250;
	wait_shoot = 150 + rand() % 150;
}

void EnemyBisouCloud::update() {
	if (blood > 0) blood -= 1;

	etape_shoot += 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onNormal();
			break;

		case ETAT_MEURE:
		case ETAT_CARBONISE:
			onMeureWait();
			break;

		case ETAT_MEURE_2:
			onMeureTombe();
			break;

		case ETAT_MEURE_4:
			onMeureFin();
			break;
	}

	updateDestroyed();
}

void EnemyBisouCloud::onMeureFin() {
	if (anim_step < 6) {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) anim_step += 1;

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[236 + anim_step];
		} else {
			pic = pbk_enemies[243 + anim_step];
		}
	} else {
		engrave(x, y, pic);
		destroyed = true;
	}
}

void EnemyBisouCloud::onMeureTombe() {
	fall();

	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 2;
	}

	if (dir == FACING_LEFT)
		pic = pbk_enemies[234 + anim_step];
	else
		pic = pbk_enemies[232 + anim_step];

	if (platform_y(x, y) != 0) {
		anim_step = anim_tick = 0;
		status = ETAT_MEURE_4;
	}
}

void EnemyBisouCloud::onMeureWait() {
	anim_step += 1;

	if (dir == FACING_LEFT)
		pic = pbk_enemies[261];
	else
		pic = pbk_enemies[260];

	if (anim_step >= 30) {
		anim_step = anim_tick = 0;
		status = ETAT_MEURE_2;
	}
}

void EnemyBisouCloud::onNormal() {
	static const int anim_gauche[] = {229, 230, 231, 230};
	static const int anim_droite[] = {226, 227, 228, 227};

	if (x > (offset + 320)) {
		dir = FACING_LEFT;
		pic = pbk_enemies[animate(anim_gauche, 4, 10)];
	} else {
		dir = FACING_RIGHT;
		pic = pbk_enemies[animate(anim_droite, 4, 10)];
	}

	if (etape_shoot >= wait_shoot && x <= offset + 640) {
		ShotCloud* s = new ShotCloud();

		s->x = x;
		s->y = y - 17;
		s->dir = dir;

		list_enemy_shots.emplace_back(s);

		etape_shoot = 0;
		wait_shoot = 250 + rand() % 250;
	}

	colFromPic();
}
