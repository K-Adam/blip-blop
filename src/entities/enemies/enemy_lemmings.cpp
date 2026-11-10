#include "entities/enemies/enemy_lemmings.h"
#include "entities/bonus_item.h"

static int wait_no = 0;

EnemyLemmings::EnemyLemmings() : suicide(false) { hp = 100; }

void EnemyLemmings::update() {
	if (blood != 0) blood -= 1;

	// After 200 kills the lemmings suicide and the generators stop
	if (game_flag[FLAG_NB_KILL] >= 200 && status != STATE_DYING && status != STATE_CHARRED) {
		suicide = true;
		col_on = false;
		anim_tick = 0;
		anim_step = 0;
		status = STATE_DYING;
		game_flag[FLAG_GEN_OFF] = 1;
	}

	if (game_flag[3] == 1) {
		sbk_level.play(5);
		game_flag[3] = 0;
	}

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
	}

	updateDestroyed();
}

void EnemyLemmings::onWalk() {
	step(5, 14);

	if (dir == FACING_LEFT) {
		x -= LEM_SPEED;

		fall();

		pic = pbk_enemies.anim("lemming_walk_left")[anim_step];

		if (solid_wall(x, y) || x < 5) dir = FACING_RIGHT;
	} else {
		x += LEM_SPEED;

		fall();

		pic = pbk_enemies.anim("lemming_walk_right")[anim_step];

		if (solid_wall(x, y)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemyLemmings::onFall() {
	fall();

	if (platform_y(x, y) != 0) status = STATE_WALK;

	colFromPic();
}

void EnemyLemmings::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 6 && (platform_y(x - 80, y) == 0 || platform_y(x + 80, y) == 0)) destroyed = true;

	if (anim_step == 10) {
		if (platform_y(x, y) != 0 && platform_y(x - 80, y) != 0 && platform_y(x + 80, y) != 0) engrave(x, y, pic);

		destroyed = true;

		if (suicide && game_flag[0] == 0) {
			Bonus* bonus = new BonusCow();
			bonus->x = 1500;
			bonus->y = -50;

			list_bonus.emplace_back(bonus);
			game_flag[0] = 1;
		}
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("lemming_die_left")[anim_step];
		else
			pic = pbk_enemies.anim("lemming_die_right")[anim_step];
	}
}

void EnemyLemmings::onCharred() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 14) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = pbk_enemies.anim("lemming_burn_left")[anim_step];
		else
			pic = pbk_enemies.anim("lemming_burn_right")[anim_step];
	}
}

void EnemyLemmings::takeHit(Shot* shot) {
	static const int dx_splatter_lem[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_lem[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_lem, dy_splatter_lem);

	if (status == STATE_DYING) {
		wait_no += 1;
		wait_no %= 20;

		if (wait_no == 0) {
			sbk_level.play(1);
		} else {
			sbk_misc.play(7 + rand() % 3);
		}
	}
}
