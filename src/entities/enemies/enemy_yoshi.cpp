#include "entities/enemies/enemy_yoshi.h"
#include "entities/shots/shot_shell.h"

const int anim_yoshi_walk_right[] = {0, 1, 2, 1};
const int anim_yoshi_walk_left[] = {0, 1, 2, 1};
const int anim_yoshi_attack_right[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
const int anim_yoshi_attack_left[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

EnemyYoshi::EnemyYoshi() : attack_delay(50 + rand() % 250), wait_for_attack(0) {
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
	if (platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		anim_step = 0;
		anim_tick = 0;
		onFall();
		return;
	}

	wait_for_attack++;
	if (wait_for_attack >= attack_delay && x <= offset + 640) {
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		anim_step = 0;
		status = STATE_SHOOT;
		attack = true;
		anim_step = 0;
		anim_tick = 0;
		sbk_level.play(35);
		onShoot();
		return;
	}

	if ((platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y)) &&
		(((dir == FACING_RIGHT) && (x > (targeted_player->x - 150)) && (targeted_player->x > x)) ||
		 ((dir == FACING_LEFT) && (x < (targeted_player->x + 150)) && (targeted_player->x < x)))) {
		anim_step = 0;
		status = STATE_SHOOT;
		attack = false;
		anim_step = 0;
		anim_tick = 0;
		sbk_level.play(37);
		onMelee();
		return;
	}

	if (x - YOSHI_SPEED < xmin || solid_wall(x - YOSHI_SPEED, y)) {
		dir = FACING_RIGHT;
	} else if (x + YOSHI_SPEED > offset + 640 || solid_wall(x + YOSHI_SPEED, y)) {
		dir = FACING_LEFT;
	}

	if (dir == FACING_RIGHT) {
		walk(YOSHI_SPEED);
		pic = pbk_enemies.anim("yoshi_walk_right")[animate(anim_yoshi_walk_right, 3, 3)];
	} else {
		walk(-YOSHI_SPEED);
		pic = pbk_enemies.anim("yoshi_walk_left")[animate(anim_yoshi_walk_left, 3, 3)];
	}
	colFromPic();
}

void EnemyYoshi::onFall() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
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
		pic = pbk_enemies.anim("yoshi_walk_right")[0];
	} else {
		x--;
		pic = pbk_enemies.anim("yoshi_walk_left")[0];
	}

	colFromPic();
}

void EnemyYoshi::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 9) anim_step += 1;

	if (anim_step >= 9) {
		int yy = platform_y(x, y);

		if (yy != 0 && yy != y_platform[4][x]) engrave(x, y, pic);

		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("yoshi_die_left")[anim_step];
		else
			pic = pbk_enemies.anim("yoshi_die_right")[anim_step];
	}
}

void EnemyYoshi::onMelee() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.anim("yoshi_tongue_right")[animate(anim_yoshi_attack_right, 18, 2)];
	} else {
		pic = pbk_enemies.anim("yoshi_tongue_left")[animate(anim_yoshi_attack_left, 18, 1)];
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
		ShotShell* shot = new ShotShell();

		shot->setDir(dir);

		if (dir == FACING_LEFT)
			shot->x = x - 23;
		else
			shot->x = x + 23;

		shot->y = y - 23;

		list_enemy_shots.emplace_back(shot);

		status = STATE_WALK;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.anim("yoshi_throw_right")[anim_step];
	} else {
		pic = pbk_enemies.anim("yoshi_throw_left")[anim_step];
	}

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
		if (dir == FACING_RIGHT)
			pic = pbk_enemies.anim("yoshi_burn_right")[anim_step];
		else
			pic = pbk_enemies.anim("yoshi_burn_left")[anim_step];
	}
}

void EnemyYoshi::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		sbk_misc.play(6 + rand() % 3);
	}
}
