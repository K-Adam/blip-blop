#include "entities/enemies/enemy_yoshi.h"
#include "entities/shots/shot_shell.h"

const int anim_yoshi_walk_right[] = {0, 1, 2, 1};
const int anim_yoshi_walk_left[] = {0, 1, 2, 1};
const int anim_yoshi_attack_right[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
const int anim_yoshi_attack_left[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

EnemyYoshi::EnemyYoshi() : attack_delay(50 + rnd() % 250), wait_for_attack(0) {
	treasure = 10;
	hp = 500;
}

void EnemyYoshi::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;

		case STATE_FALL:
			onFall();
			break;

		case STATE_SHOOT:  // ETAT_TIRE is the tongue or shell attack
			if (attack) {
				onShoot();
			} else {
				onMelee();
			}
			break;
	}

	updateDestroyed();
}

void EnemyYoshi::onWalk() {
	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		anim_step = 0;
		anim_tick = 0;
		onFall();
		return;
	}

	wait_for_attack++;
	if (wait_for_attack >= attack_delay && x <= level().camera.offset + 640) {
		wait_for_attack = 0;
		attack_delay = 50 + rnd() % 250;
		anim_step = 0;
		status = STATE_SHOOT;
		attack = true;
		anim_step = 0;
		anim_tick = 0;
		assets().sounds.level.play(35);
		onShoot();
		return;
	}

	if ((level().terrain.platform_y2(session().targeted_player->x, session().targeted_player->y) == level().terrain.platform_y2(x, y)) &&
		(((dir == FACING_RIGHT) && (x > (session().targeted_player->x - 150)) && (session().targeted_player->x > x)) ||
		 ((dir == FACING_LEFT) && (x < (session().targeted_player->x + 150)) && (session().targeted_player->x < x)))) {
		anim_step = 0;
		status = STATE_SHOOT;
		attack = false;
		anim_step = 0;
		anim_tick = 0;
		assets().sounds.level.play(37);
		onMelee();
		return;
	}

	if (x - YOSHI_SPEED < xmin || level().terrain.solid_wall(x - YOSHI_SPEED, y)) {
		dir = FACING_RIGHT;
	} else if (x + YOSHI_SPEED > level().camera.offset + 640 || level().terrain.solid_wall(x + YOSHI_SPEED, y)) {
		dir = FACING_LEFT;
	}

	if (dir == FACING_RIGHT) {
		walk(YOSHI_SPEED);
		pic = assets().pics.enemies.anim("yoshi_walk_right")[animate(anim_yoshi_walk_right, 3, 3)];
	} else {
		walk(-YOSHI_SPEED);
		pic = assets().pics.enemies.anim("yoshi_walk_left")[animate(anim_yoshi_walk_left, 3, 3)];
	}
	colFromPic();
}

void EnemyYoshi::onFall() {
	int yp;

	fall();

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}

	if (dir == FACING_RIGHT) {
		x++;
		pic = assets().pics.enemies.anim("yoshi_walk_right")[0];
	} else {
		x--;
		pic = assets().pics.enemies.anim("yoshi_walk_left")[0];
	}

	colFromPic();
}

void EnemyYoshi::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 9) anim_step += 1;

	if (anim_step >= 9) {
		int yy = level().terrain.platform_y(x, y);

		if (yy != 0 && yy != level().terrain.platforms[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		pic = facing(assets().pics.enemies, "yoshi_die")[anim_step];
	}
}

void EnemyYoshi::onMelee() {
	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("yoshi_tongue_right")[animate(anim_yoshi_attack_right, 18, 2)];
	} else {
		pic = assets().pics.enemies.anim("yoshi_tongue_left")[animate(anim_yoshi_attack_left, 18, 1)];
	}
	if (anim_step >= 17) {
		status = STATE_WALK;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}
	colFromPic();
}

void EnemyYoshi::onShoot() {
	anim_tick += 1;
	anim_tick %= 15;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 2) {
		auto shot = std::make_unique<ShotShell>();

		shot->setDir(dir);

		if (dir == FACING_LEFT)
			shot->x = x - 23;
		else
			shot->x = x + 23;

		shot->y = y - 23;

		level().entities.enemy_shots.emplace_back(std::move(shot));

		status = STATE_WALK;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}

	pic = facing(assets().pics.enemies, "yoshi_throw")[anim_step];

	colFromPic();
}

void EnemyYoshi::onCharred() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step++;

	if (anim_step >= 14)
		destroyed = true;
	else {
		pic = facing(assets().pics.enemies, "yoshi_burn")[anim_step];
	}
}

void EnemyYoshi::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		assets().sounds.misc.play(6 + rnd() % 3);
	}
}
