#include "entities/enemies/enemy_bisou_zombie.h"
#include "entities/gore_feet_bisou.h"

#define STATE_CHARRED_TOP 102

#define STATE_CRAWL 100
#define STATE_SEVERED 101

namespace {
// One moan every 200 frames for the whole horde, not one per zombie
struct ZombieChorus {
	int moan_timer = 199;
	int num_moan = 0;
	bool local_phase = false;
};
}  // namespace

EnemyBisouZombie::EnemyBisouZombie() : anim_step2(0) {
	hp = 1000;
	treasure = 10;
}

void EnemyBisouZombie::update() {
	if (blood > 0) blood -= 1;

	if (level().rules.flags[0] == 7 && status != STATE_CHARRED && status != STATE_CHARRED_TOP) {
		anim_step = anim_tick = 0;

		if (status == STATE_SEVERED)
			status = STATE_CHARRED_TOP;
		else
			status = STATE_CHARRED;
	}

	ZombieChorus& chorus = level().shared<ZombieChorus>();
	if (chorus.local_phase != session().phase && level().rules.flags[2] == 0) {
		chorus.moan_timer += 1;
		chorus.moan_timer %= 200;

		if (chorus.moan_timer == 0) {
			chorus.num_moan += 1;
			chorus.num_moan %= 3;

			assets().sounds.level.play(10 + chorus.num_moan);
		}

		chorus.local_phase = session().phase;
	}

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_SEVERED:
			onSevered();
			break;

		case STATE_FALL:
			onFall();
			break;

		case STATE_CRAWL:
			onCrawl();
			break;

		case STATE_DYING:
			onDie();
			break;

		case STATE_CHARRED:
			onCharred();
			break;

		case STATE_CHARRED_TOP:
			onCharredTop();
			break;
	}

	updateDestroyed();
}

void EnemyBisouZombie::onWalk() {
	static const int SPEED = 1;

	fall();

	step(15, 8);

	my_phase = !my_phase;

	if (my_phase) {
		if (x - SPEED < xmin || level().terrain.solid_wall(x - SPEED, y))
			dir = FACING_RIGHT;
		else if (x + SPEED > level().camera.offset + 640 || level().terrain.solid_wall(x + SPEED, y))
			dir = FACING_LEFT;

		if (dir == FACING_RIGHT) {
			walk(SPEED);
			pic = assets().pics.enemies.anim("zombie_walk_right")[anim_step];
		} else {
			walk(-SPEED);
			pic = assets().pics.enemies.anim("zombie_walk_left")[anim_step];
		}
	}

	colFromPic();
}

void EnemyBisouZombie::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	static const int dy_splatter2[] = {0, -10, -10, -10, -20, -10, -10, -10};

	int dmg = shot->damage();

	if (dmg < hp) {
		shot->onHit(dmg);
		hp -= dmg;

		if (hp < 500 && (status == STATE_NORMAL || status == STATE_WALK) && !shot->enflame()) {
			anim_step = anim_tick = 0;
			col_on = false;
			status = STATE_SEVERED;
		}
	} else {
		shot->onHit(hp);
		shot->onKill();
		hp = 0;
		col_on = false;

		level().rules.flags[FLAG_NB_KILL] += 1;

		if (status == STATE_CRAWL) {
			anim_step = 0;
			anim_tick = 0;

			if (shot->enflame() == 2) {
				anim_step = anim_tick = 0;
				col_on = false;
				status = STATE_CHARRED_TOP;
			} else {
				anim_step = anim_tick = 0;
				col_on = false;
				status = STATE_DYING;
			}
		} else {
			if (shot->enflame()) {
				anim_step = anim_tick = 0;
				col_on = false;
				status = STATE_CHARRED;
			} else if (status != STATE_SEVERED) {
				anim_step = anim_tick = 0;
				col_on = false;
				status = STATE_SEVERED;
			}
		}
	}

	if (status == STATE_CRAWL)
		splatter(shot, dx_splatter, dy_splatter2);
	else
		splatter(shot, dx_splatter, dy_splatter);
}

void EnemyBisouZombie::onFall() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("zombie_fall_right")[anim_step];
		x += 1;
	} else {
		pic = assets().pics.enemies.anim("zombie_fall_left")[anim_step];
		x -= 1;
	}

	fall();

	if (level().terrain.platform_y(x, y) != 0 && anim_step >= 3) {
		anim_step = anim_tick = 0;
		status = STATE_CRAWL;
		col_on = true;
		onCrawl();
	}
}

void EnemyBisouZombie::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.anim("zombie_die_right")[anim_step];
	else
		pic = assets().pics.enemies.anim("zombie_die_left")[anim_step];

	if (anim_step >= 3) {
		engrave(x, y, pic);
		destroyed = true;
	}
}

void EnemyBisouZombie::onSevered() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4) {
		GoreFeetBisou* feet = new GoreFeetBisou("zombie_feet_right", "zombie_feet_left");

		feet->x = x;
		feet->dir = dir;
		feet->y = y;

		level().entities.gore.emplace_back(feet);

		y -= 19;
		dy = 0;
		anim_step = 0;
		anim_tick = 0;
		status = STATE_FALL;
		onFall();
	} else {
		if (dir == FACING_RIGHT)
			pic = assets().pics.enemies.anim("zombie_severed_right")[anim_step];
		else
			pic = assets().pics.enemies.anim("zombie_severed_left")[anim_step];
	}
}

void EnemyBisouZombie::onCrawl() {
	static const int anim_right[] = {0, 1, 2, 1};
	static const int anim_left[] = {0, 1, 2, 1};
	static const int SPEED = 1;

	if (hp <= 0) {
		anim_step = anim_tick = 0;
		status = STATE_DYING;
		col_on = false;
		onDie();
		return;
	}

	fall();

	anim_step2 = (anim_step2 + 1) % 4;

	if (anim_step2 == 0) {
		if (x - SPEED < xmin || level().terrain.solid_wall(x - SPEED, y))
			dir = FACING_RIGHT;
		else if (x + SPEED > level().camera.offset + 640 || level().terrain.solid_wall(x + SPEED, y))
			dir = FACING_LEFT;

		if (dir == FACING_RIGHT)
			walk(SPEED);
		else
			walk(-SPEED);

		colFromPic();
	}

	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.anim("zombie_crawl_right")[animate(anim_right, 4, 16)];
	else
		pic = assets().pics.enemies.anim("zombie_crawl_left")[animate(anim_left, 4, 16)];
}

void EnemyBisouZombie::onCharred() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 6) {
			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.anim("zombie_charred_right")[anim_step];
	else
		pic = assets().pics.enemies.anim("zombie_charred_left")[anim_step];
}

void EnemyBisouZombie::onCharredTop() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 7) {
			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.anim("zombie_charred_top_right")[anim_step];
	else
		pic = assets().pics.enemies.anim("zombie_charred_top_left")[anim_step];
}
