#include "entities/enemies/enemy_smurf_jokey.h"
#include "entities/shots/shot_gift.h"

const int anim_smurf_jokey_walk_right[] = {0, 1, 2, 3, 4, 3, 2, 1};
const int anim_smurf_jokey_walk_left[] = {0, 1, 2, 3, 4, 3, 2, 1};

EnemySmurfJokey::EnemySmurfJokey() {
	hp = 150;
	bomb_delay = 100 + rand() % 400;
	wait_for_bomb = 0;
	carries_bomb = true;
	wait_for_new_bomb = 0;
	new_bomb_delay = 50 + rand() % 200;
	gift_dy = 5;
}

void EnemySmurfJokey::update() {
	if (blood != 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;
		case STATE_DYING:
			onDie();
			break;
		case STATE_WAIT:
			onDropGift();
			break;
		case STATE_DRAW_WEAPON:
			onDrawWeapon();
			break;
		case STATE_CHARRED:
			onCharred();
			break;
	}
	updateDestroyed();
}

void EnemySmurfJokey::onDrawWeapon() {
	anim_tick += 1;
	anim_tick %= 8;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step > 4) {
		status = STATE_NORMAL;
		carries_bomb = true;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies.anim("smurf_jokey_draw_right")[anim_step];
	else
		pic = pbk_enemies.anim("smurf_jokey_draw_left")[anim_step];
}

void EnemySmurfJokey::onDropGift() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) gift_dy -= 1;

	if (gift_dy < 0) {
		status = STATE_NORMAL;
		carries_bomb = false;
		gift_dy = 5;
		wait_for_new_bomb = 0;
		new_bomb_delay = 50 + rand() % 200;

		ShotGift* shot = new ShotGift();
		if (dir == FACING_RIGHT)
			shot->x = x + 26;
		else
			shot->x = x - 26;
		shot->y = y;
		list_enemy_shots.emplace_back(shot);

		anim_step = 0;
		anim_tick = 0;
	}
}

void EnemySmurfJokey::onWalk() {
	fall();

	if (carries_bomb) {
		wait_for_bomb += 1;

		if (wait_for_bomb > bomb_delay && platform_y(x, y) != 0) {
			status = STATE_WAIT;
			wait_for_bomb = 0;
			bomb_delay = 50 + rand() % 250;
			anim_step = 0;
			anim_tick = 0;
		}
	} else if (platform_y(x, y) != 0) {
		wait_for_new_bomb += 1;
		if (wait_for_new_bomb > new_bomb_delay) {
			wait_for_new_bomb = 0;
			new_bomb_delay = 50 + rand() % 200;
			status = STATE_DRAW_WEAPON;
			anim_step = 0;
			anim_tick = 0;
		}
	}

	if (x - SMURF_JOKEY_SPEED < xmin || solid_wall(x - SMURF_JOKEY_SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SMURF_JOKEY_SPEED > offset + 640 || solid_wall(x + SMURF_JOKEY_SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(SMURF_JOKEY_SPEED);
		if (carries_bomb)
			pic = pbk_enemies.anim("smurf_jokey_walk_right")[animate(anim_smurf_jokey_walk_right, 8, 4)];
		else
			pic = pbk_enemies.anim("smurf_walk_right")[animate(anim_smurf_walk_right, 8, 4)];
	} else {
		walk(-SMURF_JOKEY_SPEED);
		if (carries_bomb)
			pic = pbk_enemies.anim("smurf_jokey_walk_left")[animate(anim_smurf_jokey_walk_left, 8, 4)];
		else
			pic = pbk_enemies.anim("smurf_walk_left")[animate(anim_smurf_walk_left, 8, 4)];
	}
	colFromPic();
}

void EnemySmurfJokey::onDie() {
	fall();

	if (carries_bomb) {
		status = STATE_CHARRED;
		carries_bomb = false;
		ShotGift* shot = new ShotGift();
		shot->anim_step = 20;
		shot->y = y - 5;
		if (dir == FACING_RIGHT) {
			shot->x = x + 26;
		} else {
			shot->x = x - 26;
		}
		list_enemy_shots.emplace_back(shot);
		onCharred();
	} else {
		anim_tick += 1;
		anim_tick %= 4;

		if (anim_tick == 0) anim_step += 1;

		if (anim_step > 10) {
			destroyed = true;
			return;
		}

		if (dir == FACING_LEFT) {
			if (!solid_wall(x - SMURF_JOKEY_SPEED, y) && platform_y(x, y) == 0) x -= SMURF_JOKEY_SPEED;

			pic = pbk_enemies.anim("smurf_die_left")[anim_step];
		} else {
			if (!solid_wall(x + SMURF_JOKEY_SPEED, y) && platform_y(x, y) == 0) x += SMURF_JOKEY_SPEED;

			pic = pbk_enemies.anim("smurf_die_right")[anim_step];
		}
	}
}

void EnemySmurfJokey::onCharred() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 11) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("smurf_jokey_blown_left")[anim_step];
		else
			pic = pbk_enemies.anim("smurf_jokey_blown_right")[anim_step];
	}
}

void EnemySmurfJokey::takeHit(Shot* shot) {
	static const int dx_splatter_smurf_jokey[] = {0, 0, 6, 0, 0, 0, -6, 0};
	static const int dy_splatter_smurf_jokey[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_smurf_jokey, dy_splatter_smurf_jokey);

	if (status == STATE_DYING) {
		if (carries_bomb)
			sbk_misc.play(11);
		else
			sbk_level.play(4 + rand() % 10);
	}
}
