#include "entities/enemies/enemy_bisou_sky.h"
#include "entities/gore_feet_bisou.h"
#include "entities/shots/shot_rainbow.h"

EnemyBisouSky::EnemyBisouSky() : wait_for_pee(0) { hp = 175; }

void EnemyBisouSky::update() {
	if (blood > 0) blood -= 1;

	if (wait_for_pee > 0) wait_for_pee -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_DYING:
			onDieWhole();
			break;

		case STATE_DYING_2:
			onDieTop();
			break;

		case STATE_DYING_3:
			onDieTopEnd();
			break;

		case STATE_SHOOT:
			onShoot();
			break;

		case STATE_CHARRED:
			onCharred();
			break;
	}

	updateDestroyed();
}

void EnemyBisouSky::onDieWhole() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4) {
		auto feet = std::make_unique<GoreFeetBisou>("sky_feet_right", "sky_feet_left");

		feet->x = x;
		feet->dir = dir;
		feet->y = y;

		level().entities.gore.emplace_back(std::move(feet));

		y -= 19;
		dy = 0;
		anim_step = 0;
		anim_tick = 0;
		status = STATE_DYING_2;
		onDieTop();
	} else {
		pic = facing(assets().pics.enemies, "sky_die")[anim_step];
	}
}

void EnemyBisouSky::onShoot() {
	anim_tick += 1;

	if (anim_tick >= 60) {
		anim_tick = 0;
		status = STATE_WALK;
		wait_for_pee = 150;
	}
}

void EnemyBisouSky::onDieTop() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("sky_die_top_right")[anim_step];
		x += 1;
	} else {
		pic = assets().pics.enemies.anim("sky_die_top_left")[anim_step];
		x -= 1;
	}

	fall();

	if (level().terrain.platform_y(x, y) != 0 && anim_step >= 3) {
		anim_step = anim_tick = 0;
		status = STATE_DYING_3;
		onDieTopEnd();
	}
}

void EnemyBisouSky::onDieTopEnd() {
	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0 && anim_step < 3) anim_step += 1;

	pic = facing(assets().pics.enemies, "sky_die_top_end")[anim_step];

	if (anim_step >= 3) {
		engrave(x, y, pic);
		destroyed = true;
	}
}

void EnemyBisouSky::onWalk() {
	static const int SPEED = 1;

	fall();

	step(4, 8);

	if (x - SPEED < xmin || level().terrain.solid_wall(x - SPEED, y))
		dir = FACING_RIGHT;
	else if (x + SPEED > level().camera.offset + 640 || level().terrain.solid_wall(x + SPEED, y))
		dir = FACING_LEFT;

	if (dir == FACING_RIGHT) {
		walk(SPEED);
		pic = assets().pics.enemies.anim("sky_walk_right")[anim_step];
	} else {
		walk(-SPEED);
		pic = assets().pics.enemies.anim("sky_walk_left")[anim_step];
	}

	if (wait_for_pee == 0 && anim_step == 4) {												  // Frame 4 looks best
		if (session().targeted_player != nullptr && level().terrain.platform_y(x, y) != 0) {  // No shooting while falling
			int ddx = x - session().targeted_player->x;
			int ddy = y - session().targeted_player->y;

			if (ddy > -10 && ddy < 20) {
				if (dir == FACING_LEFT) {
					if (ddx > 0 && ddx < 90) {
						assets().sounds.level.play(9);
						auto s = std::make_unique<ShotRainbow>();
						s->x = x - 15;
						s->y = y - 25;
						s->dir = FACING_LEFT;

						level().entities.enemy_shots.emplace_back(std::move(s));

						anim_tick = 0;
						status = STATE_SHOOT;
					}
				} else {
					if (ddx < 0 && ddx > -90) {
						assets().sounds.level.play(9);
						auto s = std::make_unique<ShotRainbow>();
						s->x = x + 15;
						s->y = y - 25;
						s->dir = FACING_RIGHT;

						level().entities.enemy_shots.emplace_back(std::move(s));

						anim_tick = 0;
						status = STATE_SHOOT;
					}
				}
			}
		}
	}

	colFromPic();
}

void EnemyBisouSky::onCharred() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 6) {
			destroyed = true;
			return;
		}
	}

	pic = facing(assets().pics.enemies, "sky_star_charred")[anim_step];
}

void EnemyBisouSky::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	if (status == STATE_DYING) assets().sounds.level.play((rnd() % 9));
}
