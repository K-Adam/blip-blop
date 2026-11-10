#include "entities/enemies/enemy_snorky_governor.h"
#include "entities/shots/shot_snorky_governor.h"

const int anim_governor_walk_right[] = {0, 1, 2, 3, 2, 1};
const int anim_governor_walk_left[] = {0, 1, 2, 3, 2, 1};

EnemySnorkyGovernor::EnemySnorkyGovernor() : wait_for_shoot(0), shoot_delay(50 + rand() % 200) {
	hp = 150;
	pic = assets().pics.enemies.anim("snorky_walk_right")[0];
	dy = 0;
}

void EnemySnorkyGovernor::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_WALK:
		case STATE_NORMAL:
			onWalk();
			break;

		case STATE_FALL:
		case STATE_JUMP:
			onFall();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;

		case STATE_SHOOT:
			onShoot();
			break;
	}

	updateDestroyed();
}

void EnemySnorkyGovernor::onWalk() {
	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onFall();
		return;
	}

	wait_for_shoot++;
	if ((wait_for_shoot > shoot_delay) &&
		((dir == FACING_RIGHT && x > level().camera.offset + 40) || (dir == FACING_LEFT && x < level().camera.offset + 600))) {
		wait_for_shoot = 0;
		shoot_delay = 50 + rand() % 200;
		anim_step = 0;
		anim_tick = 0;
		nb_shots = 0;
		status = STATE_SHOOT;
		onShoot();
		return;
	}

	if (dir == FACING_LEFT) {
		walk(-GOVERNOR_SPEED);

		pic = assets().pics.enemies.anim("governor_walk_left")[animate(anim_governor_walk_left, 6, 4)];

		if (level().terrain.solid_wall(x - GOVERNOR_SPEED, y) || (x - GOVERNOR_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		walk(GOVERNOR_SPEED);

		pic = assets().pics.enemies.anim("governor_walk_right")[animate(anim_governor_walk_right, 6, 4)];

		if (level().terrain.solid_wall(x + GOVERNOR_SPEED, y) || (x + GOVERNOR_SPEED > level().camera.offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkyGovernor::onFall() {
	fall();

	if (level().terrain.platform_y(x, y) != 0) status = STATE_WALK;

	colFromPic();
}

void EnemySnorkyGovernor::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 5) {
		if (level().terrain.platform_y(x, y) != 0) engrave(x, y, pic);

		destroyed = true;
		return;
	}

	if (dir == FACING_LEFT) {
		pic = assets().pics.enemies.anim("governor_die_left")[anim_step];
	} else {
		pic = assets().pics.enemies.anim("governor_die_right")[anim_step];
	}
}

void EnemySnorkyGovernor::onShoot() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step == 10) {
		anim_step = 0;
		anim_tick = 0;
		status = STATE_WALK;
		onWalk();
		return;
	}

	if (anim_step == 6 && anim_tick == 0) {
		if (session().targeted_player != NULL) {
			ShotSnorkyGovernor* shot = new ShotSnorkyGovernor(session().targeted_player, rand() % 6 - 2, -5 - rand() % 5);

			shot->setDir(dir);
			shot->x = x;
			shot->y = y - 71;

			level().entities.enemy_shots.emplace_back(shot);
			nb_shots++;
		}

		if (nb_shots == 0) {
			assets().sounds.level.play(6);
		}
	}
	if ((anim_step == 7) && (nb_shots < 5) && (anim_tick == 0)) {
		anim_step -= 4;
	}

	if (anim_step < 7) {
		if (dir == FACING_RIGHT) {
			pic = assets().pics.enemies.anim("governor_shoot_right")[anim_step];
		} else {
			pic = assets().pics.enemies.anim("governor_shoot_left")[anim_step];
		}
	}
}

void EnemySnorkyGovernor::onCharred() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 8)
		destroyed = true;

	else {
		if (dir == FACING_LEFT)
			pic = assets().pics.enemies.anim("governor_burn_left")[anim_step];

		else
			pic = assets().pics.enemies.anim("governor_burn_right")[anim_step];
	}
}

void EnemySnorkyGovernor::takeHit(Shot* shot) {
	static const int dx_splatter_governor[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_governor[] = {-15, -15, -15, -15, -35, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_governor, dy_splatter_governor);

	if (status == STATE_DYING) {
		assets().sounds.level.play(4 + rand() % 2);
	}
}
