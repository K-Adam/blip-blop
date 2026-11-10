#include "entities/enemies/enemy_bisou_boom.h"
#include "entities/gore_bisou_boom.h"

EnemyBisouBoom::EnemyBisouBoom() { hp = 200; }

void EnemyBisouBoom::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
		case STATE_CHARRED:
			onDie();
			break;
	}

	updateDestroyed();
}

void EnemyBisouBoom::onWalk() {
	static const int SPEED = 3;

	fall();

	step(3, 8);

	if (x - SPEED < xmin || solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(SPEED);
		pic = pbk_enemies.anim("boom_walk_right")[anim_step];
	} else {
		walk(-SPEED);
		pic = pbk_enemies.anim("boom_walk_left")[anim_step];
	}

	if (targeted_player != NULL) {
		int ddx = x - targeted_player->x;
		int ddy = y - targeted_player->y;

		if (ddx > -60 && ddx < 60 && ddy > -40 && ddy < 20) {
			anim_tick = anim_step = 0;
			status = STATE_DYING;
			sbk_misc.play(11);
			onDie();
		}
	}

	colFromPic();
}

void EnemyBisouBoom::onDie() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 7 && anim_tick == 0) {
		Sprite* s = new GoreBisouBoom("boom_feet_right", "boom_feet_left", -2);

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

		s = new GoreBisouBoom("boom_head_right", "boom_head_left", 2);

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
		Sprite* s = new GoreBisouBoom("boom_belly_right", "boom_belly_left", 1);

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
			pic = pbk_enemies.anim("boom_explode_right")[anim_step];
		else
			pic = pbk_enemies.anim("boom_explode_left")[anim_step];

		colFromPic();
	}
}

void EnemyBisouBoom::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	if (status == STATE_DYING || status == STATE_CHARRED) return;

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING || status == STATE_CHARRED) sbk_misc.play(11);
}
