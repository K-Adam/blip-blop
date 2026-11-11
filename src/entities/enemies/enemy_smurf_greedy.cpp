#include "entities/enemies/enemy_smurf_greedy.h"
#include "entities/shots/shot_cake.h"

#define STATE_HOLD 15

EnemySmurfGreedy::EnemySmurfGreedy() : wait_for_shoot(0) {
	hp = 150;
	shoot_delay = 50 + rnd() % 100;
}

void EnemySmurfGreedy::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
			onNormal();
			break;

		case STATE_SHOOT:
			onShoot();
			break;

		case STATE_HOLD:
			onHold();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;
	}

	if (x < level().camera.offset - 20) destroyed = true;

	updateDestroyed();
}

void EnemySmurfGreedy::onNormal() {
	fall();

	wait_for_shoot += 1;

	if (wait_for_shoot > shoot_delay && x > level().camera.offset && x < level().camera.offset + 640 && session().targeted_player != nullptr) {
		assets().sounds.level.play(14 + rnd() % 2);
		status = STATE_SHOOT;
		wait_for_shoot = 0;
		shoot_delay = 100 + rnd() % 250;
		anim_step = 0;
		anim_tick = 0;
		xtarget = session().targeted_player->x;
		ytarget = session().targeted_player->y;

		if (xtarget > x)
			dir = FACING_RIGHT;
		else
			dir = FACING_LEFT;

		onShoot();
	}

	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("smurf_greedy_throw_right")[0];
	} else {
		pic = assets().pics.enemies.anim("smurf_greedy_throw_left")[0];
	}

	colFromPic();
}

void EnemySmurfGreedy::onShoot() {
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

		lobShot(shot->x, shot->y, xtarget, ytarget, shot->dx, shot->dy);

		level().entities.enemy_shots.emplace_back(shot);

		anim_step = 0;
		status = STATE_HOLD;
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("smurf_greedy_throw_right")[anim_step];
	} else {
		pic = assets().pics.enemies.anim("smurf_greedy_throw_left")[anim_step];
	}

	colFromPic();
}

void EnemySmurfGreedy::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 11) anim_step += 1;

	if (dir == FACING_RIGHT) {
		if (!level().terrain.solid_wall(x - 2, y)) x -= 2;

		pic = assets().pics.enemies.anim("smurf_greedy_die_right")[anim_step];
	} else {
		if (!level().terrain.solid_wall(x + 2, y)) x += 2;

		pic = assets().pics.enemies.anim("smurf_greedy_die_left")[anim_step];
	}

	if (anim_step >= 11 && level().terrain.platform_y(x, y) != 0) {
		destroyed = true;
		engrave(x, y, pic);
	}
}

void EnemySmurfGreedy::takeHit(Shot* shot) {
	static const int dx_splatter_smurf[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_smurf[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_smurf, dy_splatter_smurf);

	if (shot->getSplatterDir() < 4)
		dir = FACING_LEFT;
	else
		dir = FACING_RIGHT;

	if (status == STATE_DYING) assets().sounds.level.play(rnd() % 4);
}

void EnemySmurfGreedy::onHold() {
	anim_step += 1;

	if (anim_step >= 50) {
		anim_step = 0;
		status = STATE_NORMAL;
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("smurf_greedy_throw_right")[5];
	} else {
		pic = assets().pics.enemies.anim("smurf_greedy_throw_left")[5];
	}

	colFromPic();
}

void EnemySmurfGreedy::onCharred() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 6) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = assets().pics.enemies.anim("smurf_burn_left")[anim_step];
		else
			pic = assets().pics.enemies.anim("smurf_burn_right")[anim_step];
	}
}
