#include "entities/enemies/enemy_smurf_hefty.h"
#include "entities/shots/shot_hadoken.h"

#define STATE_SHOOT_HADOKEN 1000
#define STATE_SHOOT_DPUNCH 1001
#define STATE_SHOOT_HKICK 1002

EnemySmurfHefty::EnemySmurfHefty() : shoot_step(0), dx(0) {
	wait_shoot = 50 + rand() % 50;
	hp = 2000;
}

void EnemySmurfHefty::update() {
	if (blood > 0) blood -= 1;

	if (game_flag[0] != 0) {
		shoot_step += 1;

		if (game_flag[2] == 0) {
			mbk_level.play(2);
			game_flag[2] = 1;
			game_flag[FLAG_TIMER] = 40;
		}
	}

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_SHOOT_HADOKEN:
			onShootHadoken();
			break;

		case STATE_SHOOT_DPUNCH:
			onShootDragonPunch();
			break;

		case STATE_SHOOT_HKICK:
			onShootHurricaneKick();
			break;

		case STATE_CHARRED:
			onCharred();
			break;
	}

	updateDestroyed();
}

void EnemySmurfHefty::onWalk() {
	static const int walk_right[] = {0, 1, 2, 3, 2, 1};
	static const int walk_left[] = {0, 1, 2, 3, 2, 1};
	static const int SPEED = 1;

	fall();

	dx = 0;

	if (x - SPEED < xmin || solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > offset + 640 || solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(SPEED);
		pic = pbk_enemies.anim("smurf_hefty_walk_right")[animate(walk_right, 6, 6)];
	} else {
		walk(-SPEED);
		pic = pbk_enemies.anim("smurf_hefty_walk_left")[animate(walk_left, 6, 6)];
	}

	if (shoot_step >= wait_shoot && platform_y(x, y) != 0) {
		onShoot();
		return;
	}

	colFromPic();
}

void EnemySmurfHefty::takeHit(Shot* shot) {
	static const int dx_splatter_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_smurf, dy_splatter_smurf);

	if (hp <= 0) {
		game_flag[1]++;
	}
}

void EnemySmurfHefty::onCharred() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 6) {
		destroyed = true;
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("smurf_hefty_burn_right")[anim_step];
		else
			pic = pbk_enemies.anim("smurf_hefty_burn_left")[anim_step];
	}
}

void EnemySmurfHefty::onDie() {
	fall();

	int x2;

	if (platform_y(x, y) == 0)
		x2 = x + dx;
	else
		x2 = x;

	if (!solid_wall(x2, y)) x = x2;

	if (anim_step < 13) {
		anim_tick += 1;
		anim_tick %= 3;

		if (anim_tick == 0) anim_step += 1;
	}

	if (anim_step >= 14 && platform_y(x, y) != 0) {
		destroyed = true;
		engrave(x, y, pic);
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("smurf_hefty_die_right")[anim_step];
		else
			pic = pbk_enemies.anim("smurf_hefty_die_left")[anim_step];
	}
}

void EnemySmurfHefty::onShootHadoken() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step == 4) {
			ShotHadoken* t = new ShotHadoken();

			t->x = x;
			t->y = y - 30;
			t->setDir(dir);

			list_enemy_shots.emplace_back(t);
		}

		if (anim_step > 7) {
			anim_step = anim_tick = 0;
			status = STATE_WALK;
			wait_shoot = 100 + rand() % 50;
			shoot_step = 0;
			onWalk();
			return;
		}
	}

	if (anim_step <= 4) {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("smurf_hefty_throw_right")[anim_step];
		else
			pic = pbk_enemies.anim("smurf_hefty_throw_left")[anim_step];
	}
}

void EnemySmurfHefty::onShootDragonPunch() {
	fall();

	if (anim_step <= 2) {  // Takeoff
		anim_tick += 1;
		anim_tick %= 4;

		if (anim_tick == 0) {
			anim_step += 1;

			if (anim_step > 2) dy = dypunch;
		}
	} else if (dy < 0) {
		if (dir == FACING_RIGHT && x < offset + 640 && !solid_wall(x + 1, y))
			x += 1;
		else if (dir == FACING_LEFT && x > xmin && !solid_wall(x - 1, y))
			x -= 1;
	} else {
		if (anim_step < 6) {
			anim_tick += 1;
			anim_tick %= 12;

			if (anim_tick == 0) anim_step += 1;
		}

		if (platform_y(x, y) != 0) {
			anim_tick = anim_step = 0;
			wait_shoot = 100 + rand() % 50;
			shoot_step = 0;
			status = STATE_NORMAL;
			onWalk();
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies.anim("smurf_hefty_uppercut_right")[anim_step];
	else
		pic = pbk_enemies.anim("smurf_hefty_uppercut_left")[anim_step];

	colFromPic();
}

void EnemySmurfHefty::onShootHurricaneKick() {
	fall();

	step(4, 6);

	int x2 = x + dx;

	if (!solid_wall(x2, y) && x2 > xmin && x2 < offset + 740) x = x2;

	if (dy > 0 && platform_y(x, y) != 0) {
		anim_tick = anim_step = 0;
		wait_shoot = 100 + rand() % 50;
		shoot_step = 0;
		status = STATE_NORMAL;
		onWalk();
		return;
	}

	pic = pbk_enemies.anim("smurf_hefty_spin_kick")[anim_step];

	colFromPic();
}

void EnemySmurfHefty::onShoot() {
	if (targeted_player == NULL) return;

	anim_step = anim_tick = 0;

	int xc = targeted_player->x;
	int yc = targeted_player->y;

	int ddx = xc - x;
	int ddy = yc - y;

	if (ddx > 0)
		dir = FACING_RIGHT;
	else
		dir = FACING_LEFT;

	if (ddy > -50 && ddy < 50 && x > offset && x < offset + 640) {
		status = STATE_SHOOT_HADOKEN;
		sbk_level.play(19);
		onShootHadoken();
	} else if (ddx < 150 && ddx > -150 && ddy < 0) {
		status = STATE_SHOOT_DPUNCH;
		dypunch = ddy / 10;

		if (dypunch < -10) dypunch = -10;

		sbk_level.play(20);
		onShootDragonPunch();
	} else {
		lobShot(x, y - 5, xc, yc, dx, dy);
		status = STATE_SHOOT_HKICK;
		onShootHurricaneKick();
	}
}
