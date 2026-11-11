#include "entities/enemies/enemy_bisou_heart.h"
#include "entities/gore_head_bisou_heart.h"
#include "entities/shots/shot_bisou_heart.h"

EnemyBisouHeart::EnemyBisouHeart() { hp = 300; }

void EnemyBisouHeart::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_DYING_2:
			onLeave();
			break;

		case STATE_CHARRED:
			break;
	}

	updateDestroyed();
}

void EnemyBisouHeart::onWalk() {
	static const int SPEED = 2;

	static const int anim_left[] = {0, 1, 2, 3, 2, 1};
	static const int anim_right[] = {0, 1, 2, 3, 2, 1};

	if (dir == FACING_LEFT) {
		x -= SPEED;

		if (level().terrain.solid_wall(x, y) || x < xmin) dir = FACING_RIGHT;

		pic = assets().pics.enemies.anim("heart_walk_left")[animate(anim_left, 6, 7)];
	} else {
		x += SPEED;

		if (level().terrain.solid_wall(x, y) || x > level().camera.offset + 600) dir = FACING_LEFT;

		pic = assets().pics.enemies.anim("heart_walk_right")[animate(anim_right, 6, 7)];
	}

	if (anim_step == 3 && anim_tick == 0) {
		auto shot = std::make_unique<ShotBisouHeart>();

		if (dir == FACING_RIGHT)
			shot->xbase = x + 10;
		else
			shot->xbase = x - 10;

		shot->y = y;

		level().entities.enemy_shots.emplace_back(std::move(shot));
	}

	colFromPic();
}

void EnemyBisouHeart::onDie() {
	static const int SPEED = 2;

	anim_tick += 1;
	anim_tick %= 3;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 5) {
			status = STATE_DYING_2;
			return;
		}

		if (anim_step == 4) {
			auto head = std::make_unique<GoreHeadBisouHeart>();

			head->y = y - 20;
			head->dir = dir;

			if (dir == FACING_RIGHT)
				head->x = x + 11;
			else
				head->x = x - 11;

			level().entities.gore.emplace_back(std::move(head));
		}
	}

	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("heart_die_right")[anim_step];
		x += SPEED;
	} else {
		pic = assets().pics.enemies.anim("heart_die_left")[anim_step];
		x -= SPEED;
	}
}

void EnemyBisouHeart::onLeave() {
	static const int SPEED = 10;

	if (dir == FACING_RIGHT)
		x += SPEED;
	else
		x -= SPEED;
}

void EnemyBisouHeart::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) assets().sounds.level.play((rnd() % 9));
}
