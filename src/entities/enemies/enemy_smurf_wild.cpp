const int shot_start_x[] = {-15, -8, 6, 1, 8, 17};
const int shot_start_y[] = {55, 62, 65, 65, 62, 55};

const int shot_dx[] = {-4, -1, 0, 0, 1, 4};
const int shot_dy[] = {4, 5, 6, 6, 5, 4};

#include "entities/enemies/enemy_smurf_wild.h"
#include "entities/shots/shot_smurf_wild.h"

int EnemySmurfWild::wait_shoot;
int EnemySmurfWild::wait_step;

EnemySmurfWild::EnemySmurfWild() : shoot_step(0), position(false) {
	first_loop = true;
	hp = 150;
	anim_step = 1;
	nb_shoot = 0;
	dead = false;
	target = targeted_player;
	angle_tick = angle = 0;
	angleIncreasing = true;

	wait_shoot = 100 + rand() % 100;
	wait_step = 0;
	status = STATE_WAITING;

	switch (dir) {
		case FACING_RIGHT:
			hang = "smurf_wild_hang_right";
			pic = pbk_enemies.anim("smurf_wild_hang_right")[0];
			shoot_direction = DOWN_RIGHT;
			break;
		case FACING_LEFT:
			hang = "smurf_wild_hang_left";
			pic = pbk_enemies.anim("smurf_wild_hang_left")[0];
			shoot_direction = DOWN_LEFT;
			break;
	}
}

void EnemySmurfWild::update() {
	if (x > 5850) x = 5850;

	shooting = false;

	if (first_loop) {
		y = -100;
		first_loop = false;
	}

	if (target == NULL || target->isDestroyed()) {
		target = targeted_player;
	}

	if (status != STATE_DYING) {
		if (target != NULL && x != target->x)
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

	if (status != STATE_DYING && anim_step > 0) pic = pbk_enemies.anim(hang)[anim_step - 1];

	if (blood != 0) blood -= 1;

	if (!dead && hp < 0) {
		dead = true;
		anim_step = 0;
		status = 0;
	}

	switch (status) {
		case STATE_WAITING:
			if (++wait_step >= wait_shoot) {
				status = STATE_FALL;

				sbk_level.play(17);

				if (rand() % 2 == 0)
					x = offset + 320 - rand() % 150;
				else
					x = offset + 320 + rand() % 150;

				int j = 100 + rand() % 140;

				while (solid_wall(x, j)) j = 100 + rand() % 140;

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
				wait_shoot = 100 + rand() % 100;
				status = STATE_WAITING;
				destroyed = true;
				target = NULL;
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

	wait_step = 0;
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
		ShotSmurfWild* shot = new ShotSmurfWild();
		shot->x = x + shot_start_x[shoot_direction];
		shot->y = y + shot_start_y[shoot_direction];
		shot->dx = shot_dx[shoot_direction];
		shot->dy = shot_dy[shoot_direction];
		list_enemy_shots.emplace_back(shot);

		if (nb_shoot == 0) sbk_level.play(18);
		nb_shoot += 1;
	}

	if (nb_shoot >= 5) status = STATE_CLIMB;

	colFromPic();
}

void EnemySmurfWild::takeHit(Shot* shot) {
	static const int dx_splatter_smurf_wild[] = {0, 0, 6, 0, 0, 0, -6, 0};
	static const int dy_splatter_smurf_wild[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	Enemy::takeHit(shot);
	if (status == STATE_DYING) sbk_level.play(4 + rand() % 10);
}

void EnemySmurfWild::onDie() {
	if (anim_step >= 5) {
		bool ok = true;

		colFromPic();
		y1 = 0;

		for (auto& s : list_static_bgs) {
			ok = ok && !collision(s.get());
		}

		col_on = false;

		if (ok) {
			engrave(x, y - 50, pbk_enemies.pic("smurf_wild_rope"));
			engrave(x, y - 100, pbk_enemies.pic("smurf_wild_rope"));
			engrave(x, y - 150, pbk_enemies.pic("smurf_wild_rope"));
			engrave(x, y - 200, pbk_enemies.pic("smurf_wild_rope"));
			engrave(x, y - 250, pbk_enemies.pic("smurf_wild_rope"));
			engrave(x, y, pic);
			destroyed = true;
		}
	} else {
		anim_tick += 1;
		anim_tick %= 4;

		if (anim_tick == 0) {
			anim_step += 1;
			if (dir == FACING_RIGHT)
				pic = pbk_enemies.anim("smurf_wild_die_right")[anim_step - 1];
			else
				pic = pbk_enemies.anim("smurf_wild_die_left")[anim_step - 1];
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
			pic = pbk_enemies.anim("smurf_burn_left")[anim_step];
		else
			pic = pbk_enemies.anim("smurf_burn_right")[anim_step];
	}
}
