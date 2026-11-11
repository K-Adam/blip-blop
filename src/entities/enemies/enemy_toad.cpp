#include "entities/enemies/enemy_toad.h"

const int anim_toad_walk_right[] = {0, 1, 2, 1};
const int anim_toad_walk_left[] = {0, 1, 2, 1};

namespace {
// Shared by all toads so the special death lines play at most once in a while
struct ToadBrain {
	int wait_brain_hurts = 1000;
	bool local_phase = false;
	int brain_hurts = 9;
};
}  // namespace

EnemyToad::EnemyToad() : speed(0), speed_tick(0), charge_delay(50 + rnd() % 250), wait_for_charge(0), hokuto(rnd() % 5) {
	treasure = 7;
	hp = 300;
}

void EnemyToad::update() {
	ToadBrain& brain = level().shared<ToadBrain>();
	if (brain.local_phase != session().phase) {
		brain.wait_brain_hurts++;
		brain.local_phase = session().phase;
	}

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

		case STATE_JUMP:
		case STATE_FALL:
			onJump();
			break;

		case STATE_SHOOT:  // ETAT_TIRE is Toad charging
			onCharge();
			break;
	}

	updateDestroyed();
}

void EnemyToad::onWalk() {
	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		anim_step = 0;
		anim_tick = 0;
		grav_tick = 0;
		onJump();
	}

	if (x - speed < xmin || level().terrain.solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}
	wait_for_charge++;
	if (wait_for_charge >= charge_delay) {
		if ((level().terrain.platform_y2(session().targeted_player->x, session().targeted_player->y) == level().terrain.platform_y2(x, y)) &&
			(((dir == FACING_RIGHT) && (x < session().targeted_player->x)) || ((dir == FACING_LEFT) && (x > session().targeted_player->x)))) {
			charge_step = 0;
			speed = 0;
			anim_step = 0;
			anim_tick = 0;
			dy = 0;
			status = STATE_SHOOT;
			assets().sounds.level.play(34);
			onCharge();
			return;
		} else {
			wait_for_charge = 0;
			charge_delay = 50 + rnd() % 250;
			anim_step = 0;
			anim_tick = 0;
			status = STATE_JUMP;
			dy = -7;
			grav_tick = 0;	// Otherwise jump heights vary
			onJump();
			return;
		}
	}
	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = assets().pics.enemies.anim("toad_walk_right")[animate(anim_toad_walk_right, 4, 12 - 3 * speed)];
	} else {
		walk(-speed);
		pic = assets().pics.enemies.anim("toad_walk_left")[animate(anim_toad_walk_left, 4, 12 - 3 * speed)];
	}
	if (speed < TOAD_SPEED) {
		speed_tick++;
		if (speed_tick >= 30) {
			speed_tick = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyToad::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 10) anim_step += 1;
	if ((anim_step > 4) && (hokuto > 0)) {
		hokuto--;
		anim_step -= 4;
	}

	if (anim_step == 5 && anim_tick == 0) {
		assets().sounds.misc.play(9 + rnd() % 2);
	}

	if (anim_step >= 10) {
		int yy = level().terrain.platform_y(x, y);

		if (yy != 0 && yy != level().terrain.platforms[4][x]) {
			engrave(x, y, pic);
			destroyed = true;
		}
	} else {
		if (dir == FACING_LEFT) {
			if (!level().terrain.solid_wall(x - speed, y) && level().terrain.platform_y(x, y) == 0) x -= speed;
			pic = assets().pics.enemies.anim("toad_die_left")[anim_step];
		} else {
			if (!level().terrain.solid_wall(x + speed, y) && level().terrain.platform_y(x, y) == 0) x += speed;
			pic = assets().pics.enemies.anim("toad_die_right")[anim_step];
		}
	}
}

void EnemyToad::onCharge() {
	speed = TOAD_CHARGE_SPEED;
	if (x - speed < xmin || level().terrain.solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
		status = STATE_WALK;
		wait_for_charge = 0;
		charge_delay = 50 + rnd() % 250;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	} else if (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
		status = STATE_WALK;
		wait_for_charge = 0;
		charge_delay = 50 + rnd() % 250;
		anim_step = 0;
		anim_tick = 0;
		onWalk();
		return;
	}
	charge_step += 1;
	if (charge_step < 80) {
		if (dir == FACING_RIGHT) {
			pic = assets().pics.enemies.anim("toad_walk_right")[animate(anim_toad_walk_right, 4, 3)];
		} else {
			pic = assets().pics.enemies.anim("toad_walk_left")[animate(anim_toad_walk_left, 4, 3)];
		}
	} else {
		if (dir == FACING_RIGHT) {
			walk(speed);
			pic = assets().pics.enemies.anim("toad_walk_right")[animate(anim_toad_walk_right, 4, 2)];
		} else {
			walk(-speed);
			pic = assets().pics.enemies.anim("toad_walk_left")[animate(anim_toad_walk_left, 4, 2)];
		}
	}

	colFromPic();
}

void EnemyToad::onJump() {
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
		x += speed;
		pic = assets().pics.enemies.pic("toad_jump_right");
	} else {
		x -= speed;
		pic = assets().pics.enemies.pic("toad_jump_left");
	}

	colFromPic();
}

void EnemyToad::onCharred() {
	fall();
	anim_tick++;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step >= 13)
		destroyed = true;
	else {
		pic = facing(assets().pics.enemies, "toad_burn")[anim_step];
	}
}

void EnemyToad::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) {
		ToadBrain& brain = level().shared<ToadBrain>();
		if (brain.wait_brain_hurts >= 140 && brain.wait_brain_hurts <= 200) {
			brain.wait_brain_hurts = 250;
			assets().sounds.level.play(29);
			hokuto = 7;
		} else {
			brain.brain_hurts += 1;
			brain.brain_hurts %= 10;

			if (brain.brain_hurts == 0) {
				brain.wait_brain_hurts = 0;
				hokuto = 6;
				assets().sounds.level.play(28);
			} else {
				assets().sounds.level.play(30 + rnd() % 2);
			}
		}
	}
}
