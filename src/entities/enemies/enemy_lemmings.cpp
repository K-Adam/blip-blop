#include "entities/enemies/enemy_lemmings.h"
#include "entities/bonus_item.h"

namespace {
// Every 20th lemming death plays the "no" line
struct LemmingsVoice {
	int wait_no = 0;
};
}  // namespace

EnemyLemmings::EnemyLemmings() : suicide(false) { hp = 100; }

void EnemyLemmings::update() {
	if (blood != 0) blood -= 1;

	// After 200 kills the lemmings suicide and the generators stop
	if (level().rules.flags[FLAG_NB_KILL] >= 200 && status != STATE_DYING && status != STATE_CHARRED) {
		suicide = true;
		col_on = false;
		anim_tick = 0;
		anim_step = 0;
		status = STATE_DYING;
		level().rules.flags[FLAG_GEN_OFF] = 1;
	}

	if (level().rules.flags[3] == 1) {
		assets().sounds.level.play(5);
		level().rules.flags[3] = 0;
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

		pic = assets().pics.enemies.anim("lemming_walk_left")[anim_step];

		if (level().terrain.solid_wall(x, y) || x < 5) dir = FACING_RIGHT;
	} else {
		x += LEM_SPEED;

		fall();

		pic = assets().pics.enemies.anim("lemming_walk_right")[anim_step];

		if (level().terrain.solid_wall(x, y)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemyLemmings::onFall() {
	fall();

	if (level().terrain.platform_y(x, y) != 0) status = STATE_WALK;

	colFromPic();
}

void EnemyLemmings::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step >= 6 && (level().terrain.platform_y(x - 80, y) == 0 || level().terrain.platform_y(x + 80, y) == 0)) destroyed = true;

	if (anim_step == 10) {
		if (level().terrain.platform_y(x, y) != 0 && level().terrain.platform_y(x - 80, y) != 0 && level().terrain.platform_y(x + 80, y) != 0)
			engrave(x, y, pic);

		destroyed = true;

		if (suicide && level().rules.flags[0] == 0) {
			Bonus* bonus = new BonusCow();
			bonus->x = 1500;
			bonus->y = -50;

			level().entities.bonus.emplace_back(bonus);
			level().rules.flags[0] = 1;
		}
	} else {
		if (dir == FACING_LEFT)
			pic = assets().pics.enemies.anim("lemming_die_left")[anim_step];
		else
			pic = assets().pics.enemies.anim("lemming_die_right")[anim_step];
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
			pic = assets().pics.enemies.anim("lemming_burn_left")[anim_step];
		else
			pic = assets().pics.enemies.anim("lemming_burn_right")[anim_step];
	}
}

void EnemyLemmings::takeHit(Shot* shot) {
	static const int dx_splatter_lem[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_lem[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_lem, dy_splatter_lem);

	if (status == STATE_DYING) {
		int& wait_no = level().shared<LemmingsVoice>().wait_no;
		wait_no += 1;
		wait_no %= 20;

		if (wait_no == 0) {
			assets().sounds.level.play(1);
		} else {
			assets().sounds.misc.play(7 + rnd() % 3);
		}
	}
}
