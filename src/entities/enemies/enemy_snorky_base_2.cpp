#include "entities/enemies/enemy_snorky_base_2.h"
#include "entities/shots/shot_thorn.h"

const int anim_snorky_base_walk_right[] = {0, 1, 2, 3, 4, 3, 2, 1};
const int anim_snorky_base_walk_left[] = {0, 1, 2, 3, 4, 3, 2, 1};

EnemySnorkyBase2::EnemySnorkyBase2() : wait_for_shoot(0), shoot_delay(50 + rnd() % 200) {
	hp = 150;
	pic = assets().pics.enemies.anim("snorky_walk_right")[0];
	dy = 0;
}

void EnemySnorkyBase2::update() {
	if (blood > 0) blood -= 1;

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

		case STATE_SHOOT:
			onShoot();
			break;
	}

	updateDestroyed();
}

void EnemySnorkyBase2::onWalk() {
	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onFall();
		return;
	}

	wait_for_shoot++;
	if ((session().targeted_player != nullptr) && (wait_for_shoot > shoot_delay) &&
		((dir == FACING_RIGHT && x > level().camera.offset + 80) || (dir == FACING_LEFT && x < level().camera.offset + 560))) {
		wait_for_shoot = 0;
		shoot_delay = 50 + rnd() % 200;
		anim_step = 0;
		anim_tick = 0;
		status = STATE_SHOOT;
		x_target = session().targeted_player->x;
		y_target = session().targeted_player->y;
		onShoot();
		return;
	}

	if (dir == FACING_LEFT) {
		walk(-SNORKY_BASE_SPEED);

		pic = assets().pics.enemies.anim("snorky2_walk_left")[animate(anim_snorky_base_walk_left, 8, 4)];

		if (level().terrain.solid_wall(x - SNORKY_BASE_SPEED, y) || (x - SNORKY_BASE_SPEED < xmin)) dir = FACING_RIGHT;
	} else {
		walk(SNORKY_BASE_SPEED);

		pic = assets().pics.enemies.anim("snorky2_walk_right")[animate(anim_snorky_base_walk_right, 8, 4)];

		if (level().terrain.solid_wall(x + SNORKY_BASE_SPEED, y) || (x + SNORKY_BASE_SPEED > level().camera.offset + 640)) dir = FACING_LEFT;
	}

	colFromPic();
}

void EnemySnorkyBase2::onFall() {
	fall();

	if (level().terrain.platform_y(x, y) != 0) status = STATE_WALK;

	colFromPic();
}

void EnemySnorkyBase2::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if ((anim_step == 5) && (anim_tick == 0)) {
		dy = 0;
		y -= 17;

		if (dir == FACING_RIGHT)
			x += 10;

		else
			x -= 10;
	}
	if (anim_step >= 5) {
		fall();

		if (dir == FACING_RIGHT)
			x += 1;

		else
			x -= 1;
	}

	if (anim_step == 10) {
		if (level().terrain.platform_y(x, y) != 0) engrave(x, y, pic);

		destroyed = true;
		return;
	}

	if (dir == FACING_LEFT) {
		pic = assets().pics.enemies.anim("snorky2_die_left")[anim_step];
	} else {
		pic = assets().pics.enemies.anim("snorky2_die_right")[anim_step];
	}
}

void EnemySnorkyBase2::onShoot() {
	anim_tick++;
	anim_tick %= 6;
	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step == 12) {
		anim_step = 0;
		anim_tick = 0;
		status = STATE_WALK;
		onWalk();
		return;
	}

	if (anim_step == 10 && anim_tick == 0) {
		if (dir == FACING_RIGHT) {
			if ((x_target > x) && (x_target - x != 0)) {
				int vy = ((y_target - y + 50) * 6) / (x_target - x);
				if (vy > 3) vy = 3;

				if (vy < -3) vy = -3;

				auto shot = std::make_unique<ShotThorn>(assets().pics.enemies.pic("snorky2_shot"), 6, vy);
				shot->setDir(dir);
				shot->x = x + 35;
				shot->y = y - 42;

				level().entities.enemy_shots.emplace_back(std::move(shot));

			} else {
				auto shot = std::make_unique<ShotThorn>(assets().pics.enemies.pic("snorky2_shot"), 6, 1);
				shot->setDir(dir);
				shot->x = x + 35;
				shot->y = y - 42;

				level().entities.enemy_shots.emplace_back(std::move(shot));
			}
			pic = assets().pics.enemies.anim("snorky2_shoot_right")[0];
		} else {
			if ((x_target < x) && (x_target - x != 0)) {
				int vy = ((y_target - y + 50) * 6) / (x_target - x);
				if (vy > 3) vy = 3;

				if (vy < -3) vy = -3;

				auto shot = std::make_unique<ShotThorn>(assets().pics.enemies.pic("snorky2_shot"), -6, -vy);
				shot->setDir(dir);
				shot->x = x - 35;
				shot->y = y - 42;

				level().entities.enemy_shots.emplace_back(std::move(shot));

			} else {
				auto shot = std::make_unique<ShotThorn>(assets().pics.enemies.pic("snorky2_shot"), -6, 1);
				shot->setDir(dir);
				shot->x = x - 35;
				shot->y = y - 42;

				level().entities.enemy_shots.emplace_back(std::move(shot));
			}
			pic = assets().pics.enemies.anim("snorky2_shoot_left")[0];
		}
	}

	if (anim_step < 10) {
		if (dir == FACING_RIGHT) {
			pic = assets().pics.enemies.anim("snorky2_shoot_right")[anim_step];
		} else {
			pic = assets().pics.enemies.anim("snorky2_shoot_left")[anim_step];
		}
	}
	colFromPic();
}

void EnemySnorkyBase2::onCharred() {
	anim_tick++;
	anim_tick %= 6;

	if (anim_tick == 0) anim_step++;

	if (anim_step == 9) {
		destroyed = true;
	} else {
		if (dir == FACING_LEFT)
			pic = assets().pics.enemies.anim("snorky2_burn_left")[anim_step];
		else
			pic = assets().pics.enemies.anim("snorky2_burn_right")[anim_step];
	}
}

void EnemySnorkyBase2::takeHit(Shot* shot) {
	static const int dx_splatter_snorkbase2[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_snorkbase2[] = {-15, -15, -15, -15, -35, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_snorkbase2, dy_splatter_snorkbase2);

	if (status == STATE_DYING) {
		assets().sounds.level.play(18);
	}
}
