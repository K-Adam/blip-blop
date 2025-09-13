#include "entities/enemies/enemy_bisou_boom.h"
#include "entities/gore_bisou_boom.h"

EnemyBisouBoom::EnemyBisouBoom() { hp = 200; }

void EnemyBisouBoom::update() {
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

void EnemyBisouBoom::onAvance() {
	static const int SPEED = 3;

	fall();

	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (x - SPEED < xmin || solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		marche(SPEED);
		pic = pbk_enemies[154 + anim_step];
	} else {
		marche(-SPEED);
		pic = pbk_enemies[162 + anim_step];
	}

	if (targeted_player != NULL) {
		int ddx = x - targeted_player->x;
		int ddy = y - targeted_player->y;

		if (ddx > -60 && ddx < 60 && ddy > -40 && ddy < 20) {
			anim_tick = anim_step = 0;
			status = ETAT_MEURE;
			sbk_misc.play(11);
			onMeure();
		}
	}

	colFromPic();
}

void EnemyBisouBoom::onMeure() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 7 && anim_tick == 0) {
		Sprite* s = new GoreFeetBisouBoom();

		if (dir == FACING_RIGHT) {
			s->x = x - 60;
			s->y = y - 22;
			s->dir = dir;
			s->dy = -(rand() % 4);
		} else {
			s->x = x + 60;
			s->y = y - 26;
			s->dir = dir;
			s->dy = -(rand() % 4);
		}

		list_gore.emplace_back(s);

		s = new GoreHeadBisouBoom();

		if (dir == FACING_RIGHT) {
			s->x = x + 46;
			s->y = y - 70;
			s->dir = dir;
			s->dy = -1;
		} else {
			s->x = x - 45;
			s->y = y - 62;
			s->dir = dir;
			s->dy = -1;
		}

		list_gore.emplace_back(s);
	} else if (anim_step == 8 && anim_tick == 0) {
		Sprite* s = new GoreBellyBisouBoom();

		if (dir == FACING_RIGHT) {
			s->x = x + 59;
			s->y = y - 3;
			s->dir = dir;
			s->dy = 0;
		} else {
			s->x = x - 58;
			s->y = y - 7;
			s->dir = dir;
			s->dy = 0;
		}

		list_gore.emplace_back(s);
	} else if (anim_step >= 10) {
		destroyed = true;
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[170 + anim_step];
		else
			pic = pbk_enemies[180 + anim_step];

		colFromPic();
	}
}

void EnemyBisouBoom::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	if (status == ETAT_MEURE || status == ETAT_CARBONISE) return;

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	if (status == ETAT_MEURE || status == ETAT_CARBONISE) sbk_misc.play(11);
}
