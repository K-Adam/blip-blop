const int shot_start_x[] = {-15, -8, 6, 1, 8, 17};
const int shot_start_y[] = {55, 62, 65, 65, 62, 55};

const int shot_dx[] = {-4, -1, 0, 0, 1, 4};
const int shot_dy[] = {4, 5, 6, 6, 5, 4};

#include "entities/enemies/enemy_smurf_wild.h"
#include "entities/shots/shot_smurf_wild.h"

namespace {
// One countdown for every wild smurf, so they drop in one at a time
struct SmurfAmbush {
	int wait_shoot = 0;
	int wait_step = 0;
};
}  // namespace

EnemySmurfWild::EnemySmurfWild() : shoot_step(0), position(false) {
	first_loop = true;
	hp = 150;
	anim_step = 1;
	nb_shoot = 0;
	dead = false;
	target = session().targeted_player;
	angle_tick = angle = 0;
	angleIncreasing = true;

	SmurfAmbush& ambush = level().shared<SmurfAmbush>();
	ambush.wait_shoot = 100 + rnd() % 100;
	ambush.wait_step = 0;
	status = STATE_WAITING;

	switch (dir) {
		case FACING_RIGHT:
			hang = "smurf_wild_hang_right";
			pic = assets().pics.enemies.anim("smurf_wild_hang_right")[0];
			shoot_direction = DOWN_RIGHT;
			break;
		case FACING_LEFT:
			hang = "smurf_wild_hang_left";
			pic = assets().pics.enemies.anim("smurf_wild_hang_left")[0];
			shoot_direction = DOWN_LEFT;
			break;
	}
}

void EnemySmurfWild::update() {
	auto& [wait_shoot, wait_step] = level().shared<SmurfAmbush>();
	if (x > 5850) x = 5850;

	shooting = false;

	if (first_loop) {
		y = -100;
		first_loop = false;
	}

	if (target == nullptr || target->isDestroyed()) {
		target = session().targeted_player;
	}

	if (status != STATE_DYING) {
		if (target != nullptr && x != target->x)
			slope = (abs(target->y - y) * 10) / (x - target->x);
		else
			slope = 3;
	}

	if (slope > 0) {
		dir = FACING_LEFT;
		hang = "smurf_wild_hang_left";

		if (slope <= 15)
			shoot_direction = DIAGONAL_LEFT;
		else if (slope <= 25)
			shoot_direction = DIAGONAL_DOWN_LEFT;
		else
			shoot_direction = DOWN_LEFT;
	} else if (slope < 0) {
		dir = FACING_RIGHT;
		hang = "smurf_wild_hang_right";

		if (slope >= -15)
			shoot_direction = DIAGONAL_RIGHT;
		else if (slope >= -25)
			shoot_direction = DIAGONAL_DOWN_RIGHT;
		else
			shoot_direction = DOWN_RIGHT;
	}

	if (status != STATE_DYING && anim_step > 0) pic = assets().pics.enemies.anim(hang)[anim_step - 1];

	if (blood != 0) blood -= 1;

	if (!dead && hp < 0) {
		dead = true;
		anim_step = 0;
		status = STATE_WALK;
	}

	switch (status) {
		case STATE_WAITING:
			if (++wait_step >= wait_shoot) {
				status = STATE_FALL;

				assets().sounds.level.play(17);

				if (rnd() % 2 == 0)
					x = level().camera.offset + 320 - rnd() % 150;
				else
					x = level().camera.offset + 320 + rnd() % 150;

				int j = 100 + rnd() % 140;

				while (level().terrain.solid_wall(x, j)) j = 100 + rnd() % 140;

				nb_shoot = 0;
				height_px = j;
			}
			break;
		case STATE_FALL:
			onFall();
			break;

		case STATE_SHOOT:
			onShoot();
			break;

		case STATE_CLIMB:
			wait_step = 0;

			if (y <= -100) {
				wait_shoot = 100 + rnd() % 100;
				status = STATE_WAITING;
				destroyed = true;
				target = nullptr;
			} else {
				y -= 2;
			}
			colFromPic();

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

void EnemySmurfWild::onFall() {
	if (y < height_px) {
		y += 3;
	} else {
		status = STATE_SHOOT;
	}

	level().shared<SmurfAmbush>().wait_step = 0;
	colFromPic();
}

void EnemySmurfWild::onShoot() {
	shooting = true;

	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step > 2) anim_step = 1;

	shoot_step += 1;
	shoot_step %= 10;

	if (shoot_step == 0) {
		auto shot = std::make_unique<ShotSmurfWild>();
		shot->x = x + shot_start_x[shoot_direction];
		shot->y = y + shot_start_y[shoot_direction];
		shot->dx = shot_dx[shoot_direction];
		shot->dy = shot_dy[shoot_direction];
		level().entities.enemy_shots.emplace_back(std::move(shot));

		if (nb_shoot == 0) assets().sounds.level.play(18);
		nb_shoot += 1;
	}

	if (nb_shoot >= 5) status = STATE_CLIMB;

	colFromPic();
}

void EnemySmurfWild::takeHit(Shot* shot) {
	Enemy::takeHit(shot);
	if (status == STATE_DYING) assets().sounds.level.play(4 + rnd() % 10);
}

void EnemySmurfWild::onDie() {
	if (anim_step >= 5) {
		bool ok = true;

		colFromPic();
		y1 = 0;

		for (auto& s : level().entities.static_bgs) {
			ok = ok && !collision(s.get());
		}

		col_on = false;

		if (ok) {
			engrave(x, y - 50, assets().pics.enemies.pic("smurf_wild_rope"));
			engrave(x, y - 100, assets().pics.enemies.pic("smurf_wild_rope"));
			engrave(x, y - 150, assets().pics.enemies.pic("smurf_wild_rope"));
			engrave(x, y - 200, assets().pics.enemies.pic("smurf_wild_rope"));
			engrave(x, y - 250, assets().pics.enemies.pic("smurf_wild_rope"));
			engrave(x, y, pic);
			destroyed = true;
		}
	} else {
		anim_tick += 1;
		anim_tick %= 4;

		if (anim_tick == 0) {
			anim_step += 1;
			if (dir == FACING_RIGHT)
				pic = assets().pics.enemies.anim("smurf_wild_die_right")[anim_step - 1];
			else
				pic = assets().pics.enemies.anim("smurf_wild_die_left")[anim_step - 1];
		}
	}
}

void EnemySmurfWild::onCharred() {
	y = height_px + 50 - anim_step;
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
