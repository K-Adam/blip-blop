#include "entities/enemies/enemy_bisou_star.h"
#include "entities/gore_feet_bisou_star.h"
#include "entities/shots/shot_star.h"

EnemyBisouStar::EnemyBisouStar() : shoot_step(0) {
	hp = 200;
	wait_shoot = 200 + rand() % 250;
}

void EnemyBisouStar::update() {
	if (blood > 0) blood -= 1;

	shoot_step += 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
			onDieWhole();
			break;

		case STATE_DYING_2:
			onDieTop();
			break;

		case STATE_DYING_3:
			onDieTopEnd();
			break;

		case STATE_SHOOT:
			onShoot();
			break;

		case STATE_CHARRED:
			onCharred();
			break;
	}

	updateDestroyed();
}

void EnemyBisouStar::onDieWhole() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4) {
		GoreFeetBisouStar* feet = new GoreFeetBisouStar();

		feet->x = x;
		feet->dir = dir;
		feet->y = y;

		list_gore.emplace_back(feet);

		y -= 19;
		dy = 0;
		anim_step = 0;
		anim_tick = 0;
		status = STATE_DYING_2;
		onDieTop();
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("star_die_right")[anim_step];
		else
			pic = pbk_enemies.anim("star_die_left")[anim_step];
	}
}

void EnemyBisouStar::onShoot() {
	anim_tick += 1;

	if (anim_tick >= 60) {
		anim_tick = 0;
		status = STATE_WALK;
		wait_shoot = 200 + rand() % 500;
		;
		shoot_step = 0;
	}
}

void EnemyBisouStar::onDieTop() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.anim("star_die_top_right")[anim_step];
		x += 1;
	} else {
		pic = pbk_enemies.anim("star_die_top_left")[anim_step];
		x -= 1;
	}

	fall();

	if (platform_y(x, y) != 0 && anim_step >= 3) {
		anim_step = anim_tick = 0;
		status = STATE_DYING_3;
		onDieTopEnd();
	}
}

void EnemyBisouStar::onDieTopEnd() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT)
		pic = pbk_enemies.anim("star_die_top_end_right")[anim_step];
	else
		pic = pbk_enemies.anim("star_die_top_end_left")[anim_step];

	if (anim_step >= 3) {
		engrave(x, y, pic);
		destroyed = true;
	}
}

void EnemyBisouStar::onWalk() {
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
		walk(SPEED);
		pic = pbk_enemies.anim("star_walk_right")[anim_step];
	} else {
		walk(-SPEED);
		pic = pbk_enemies.anim("star_walk_left")[anim_step];
	}

	shoot_step += 1;

	if (anim_step == 4 && shoot_step >= wait_shoot && x > offset) {
		ShotStar* s = new ShotStar();

		s->x = x;
		s->y = y - 30;
		s->dir = dir;

		list_enemy_shots.emplace_back(s);

		anim_tick = 0;
		status = STATE_SHOOT;
	}

	colFromPic();
}

void EnemyBisouStar::onCharred() {
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
		pic = pbk_enemies.anim("sky_star_charred_right")[anim_step];
	else
		pic = pbk_enemies.anim("sky_star_charred_left")[anim_step];
}

void EnemyBisouStar::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) sbk_level.play((rand() % 9));
}
