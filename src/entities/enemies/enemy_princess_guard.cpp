#include "entities/enemies/enemy_princess_guard.h"
#include "entities/enemies/enemy_princess.h"

const int anim_guard_walk_left[] = {0, 1, 2, 1};

EnemyPrincessGuard::EnemyPrincessGuard(EnemyPrincess* Princess, int distance) : cover_distance(distance), princess(Princess) {
	hp = 2500;
	treasure = 100;
	dir = FACING_LEFT;
}

void EnemyPrincessGuard::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_CHARRED:
		case STATE_DYING:
			onDie();
			break;
	}

	updateDestroyed();
}

void EnemyPrincessGuard::onWalk() {
	if (princess->destroyed == true) {
		destroyed = true;
	}

	if (level().rules.flags[2]) {
		status = STATE_DYING;
		onDie();
		return;
	}
	fall();
	if (princess != NULL) {
		if ((x + cover_distance + GUARD_SPEED) < princess->x) {
			walk(GUARD_SPEED);
			pic = assets().pics.enemies.anim("princess_guard_walk_left")[animate(anim_guard_walk_left, 4, 6)];
		} else if ((x + cover_distance - GUARD_SPEED) > princess->x) {
			walk(-GUARD_SPEED * 3);
			pic = assets().pics.enemies.anim("princess_guard_walk_left")[animate(anim_guard_walk_left, 4, 6)];
		} else {
			pic = assets().pics.enemies.anim("princess_guard_walk_left")[0];
		}
	} else {
		destroyed = true;
	}

	colFromPic();
}

void EnemyPrincessGuard::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 10) anim_step += 1;

	if (anim_step >= 10) {
		int yy = level().terrain.platform_y(x, y);

		if (yy != 0 && yy != level().terrain.platforms[4][x]) {
			engrave(x, y, pic);
			destroyed = true;
			level().rules.flags[3] -= 1;
		}
	} else {
		pic = assets().pics.enemies.anim("princess_guard_die")[anim_step];
	}
}

void EnemyPrincessGuard::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);
}
