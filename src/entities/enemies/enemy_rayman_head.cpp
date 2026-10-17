#include "entities/enemies/enemy_rayman_head.h"

EnemyRaymanHead::EnemyRaymanHead() : dx(3) {
	hp = 1;
	pic = pbk_enemies.anim("rayman_head_spin")[0];
}

void EnemyRaymanHead::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
		case STATE_JUMP:
		case STATE_FALL:
			onWalk();
			break;

		case STATE_CHARRED:
		case STATE_DYING:
			onDie();
			break;
	}
}

void EnemyRaymanHead::onWalk() {
	x += dx;
	fall();
	colFromPic();

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 8;
		pic = pbk_enemies.anim("rayman_head_spin")[anim_step];
	}

	if (dy > 0 && (platform_y(x, y + dy)) != 0) {
		engrave(x, y, pic);
		game_flag[0] = 2;
		destroyed = true;
	}
}

void EnemyRaymanHead::onDie() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
		if (anim_step >= 6) {
			game_flag[0] = 2;
			destroyed = true;
		} else {
			pic = pbk_enemies.anim("rayman_head_die")[anim_step];
		}
	}
}

int EnemyRaymanHead::damage() { return 0; };

void EnemyRaymanHead::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	shot->onHit(9999);
	splatter(shot, dx_splatter, dy_splatter);
}
