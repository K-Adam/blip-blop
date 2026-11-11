#include "entities/enemies/enemy_mario_hologram.h"
#include "entities/shots/shot_mario_fireball.h"
#include "entities/shots/shot_mario_fireball_vertical.h"

const int anim_mario_walk_right[] = {0, 1, 2, 1};
const int anim_mario_walk_left[] = {0, 1, 2, 1};
const int mario_recoil[] = {4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

EnemyMarioHologram::EnemyMarioHologram()
	: speed(1), speed_tick(0), fireball(0), wait_for_blink(0), blink_delay(10 + rand() % 10), attack_delay(50 + rand() % 50), wait_for_attack(0) {
	hp = 1;
	pic = assets().pics.enemies.anim("mario_walk_right")[0];
}

void EnemyMarioHologram::update() {
	wait_for_blink++;
	if (wait_for_blink > blink_delay + NB_BLINKS) {
		look_ = "hologram_";
		wait_for_blink = 0;
		blink_delay = 50 + rand() % 50;
	} else if (wait_for_blink > blink_delay) {
		look_ = "hologram_";
	} else {
		look_ = "mario_";
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

		case STATE_SHOOT:
			switch (attack_type) {
				case 0:
					onShoot();
					break;

				case 1:
					onShootVertical();
					break;
			}
			break;
	}
}

void EnemyMarioHologram::onWalk() {
	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onJump();
	}

	if (x - speed < xmin || level().terrain.solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > level().camera.offset + 600 || level().terrain.solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}
	wait_for_attack++;
	if ((wait_for_attack >= attack_delay) && (session().targeted_player != NULL)) {
		if (((dir == FACING_RIGHT) && (session().targeted_player->x > x)) || ((dir == FACING_LEFT) && (session().targeted_player->x < x))) {
			anim_step = 0;
			anim_tick = 0;
			speed = 0;
			dy = -6;
			wait_for_attack = 0;
			attack_delay = 50 + rand() % 50;
			status = STATE_SHOOT;
			y_target = session().targeted_player->y;
			x_target = session().targeted_player->x;
			attack_step = 0;
			fireball = 1;
			attack_type = rand() % 2;
			switch (attack_type) {
				case 0:
					onShoot();
					break;

				case 1:
					onShootVertical();
					break;
			}
			return;
		}
	}
	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = assets().pics.enemies.anim(look_ + "walk_right")[animate(anim_mario_walk_right, 4, 16 - (3 * speed))];
	} else {
		walk(-speed);
		pic = assets().pics.enemies.anim(look_ + "walk_left")[animate(anim_mario_walk_left, 4, 16 - (3 * speed))];
	}
	if (speed < MARIO_SPEED) {
		speed_tick++;
		if (speed_tick >= 20) {
			speed_tick = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyMarioHologram::onDie() { destroyed = true; }

void EnemyMarioHologram::onCharge() {}

void EnemyMarioHologram::onJump() {
	int yp;

	if (dy < 0) {
		fall_mario();
	} else {
		fall();
	}

	// A boss off screen could not be hit
	if ((dir == FACING_LEFT) && (x - speed < xmin || level().terrain.solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
	}

	else if ((dir == FACING_RIGHT) && (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y))) {
		dir = FACING_LEFT;
		speed = 1;
	}

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		onWalk();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		if (dy < 0) {
			pic = assets().pics.enemies.anim(look_ + "jump_right")[0];
		} else {
			pic = assets().pics.enemies.anim(look_ + "jump_right")[1];
		}
	} else {
		x -= speed;
		if (dy < 0) {
			pic = assets().pics.enemies.anim(look_ + "jump_left")[0];
		} else {
			pic = assets().pics.enemies.anim(look_ + "jump_left")[1];
		}
	}

	colFromPic();
}

void EnemyMarioHologram::onCharred() { destroyed = true; }

void EnemyMarioHologram::render() {
	Sprite::render();
	if (fireball) {
		if (dir == FACING_RIGHT) {
			draw(x + 29, y - 33, assets().pics.enemies.anim("mario_muzzle_right")[anim_step]);
		} else {
			draw(x - 29, y - 33, assets().pics.enemies.anim("mario_muzzle_left")[anim_step]);
		}
	}
}

void EnemyMarioHologram::onShoot() {
	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.anim(look_ + "shoot_right")[0];

	else
		pic = assets().pics.enemies.anim(look_ + "shoot_left")[0];

	if ((y - 25 < y_target) && (y_target < 400)) {
		if (y > y_target - 60) {
			dy = 2;
		} else if (y > y_target - 120) {
			dy = 3;
		} else if (y > y_target - 180) {
			dy = 4;
		} else if (y > y_target - 240) {
			dy = 5;
		} else {
			dy = 6;
		}
		y += dy;
	} else if ((y - 30 > y_target) && (y_target < 400)) {
		if (y < y_target + 60) {
			dy = -2;
		} else if (y < y_target + 120) {
			dy = -3;
		} else if (y < y_target + 180) {
			dy = -4;
		} else if (y < y_target + 240) {
			dy = -5;
		} else {
			dy = -6;
		}
		y += dy;
	} else {
		anim_tick++;
		anim_tick %= 4;
		if (anim_tick == 0) {
			anim_step++;
			anim_step %= 3;
		}

		attack_step++;
		if (attack_step == 60) {
			status = STATE_WALK;
			anim_step = 0;
			anim_tick = 0;
			speed = 1;
			onWalk();
			return;
			fireball = 0;
			shoot_fireball();
		} else if (attack_step >= 40) {
			if (attack_step == 40) {
				fireball = 0;
				shoot_fireball();
			}
			if ((dir == FACING_RIGHT) && (x > 10)) {
				x -= mario_recoil[attack_step - 40];
			} else if ((dir == FACING_LEFT) && (x < 630)) {
				x += mario_recoil[attack_step - 40];
			}
		}
	}
	colFromPic();
}

void EnemyMarioHologram::onShootVertical() {
	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.pic("mario_shoot_right");

	else
		pic = assets().pics.enemies.pic("mario_shoot_left");

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	attack_step++;
	if (attack_step == 40) {
		fireball = 0;
		ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(0, x_target);

		shot->setDir(dir);
		if (dir == FACING_RIGHT) {
			shot->x = x + 38;
		} else {
			shot->x = x - 38;
		}
		shot->y = y - 38;

		level().entities.enemy_shots.emplace_back(shot);
	} else if (attack_step == 80) {
		status = STATE_WALK;
		anim_step = 0;
		anim_tick = 0;
		speed = 1;
		onWalk();
		return;
	}
	colFromPic();
}

inline void EnemyMarioHologram::shoot_fireball() {
	if (dir == FACING_RIGHT) {
		ShotMarioFireball* shot = new ShotMarioFireball(1);

		shot->setDir(dir);
		shot->x = x + 38;
		shot->y = y - 31;

		level().entities.enemy_shots.emplace_back(shot);
	} else {
		ShotMarioFireball* shot = new ShotMarioFireball(-1);

		shot->setDir(dir);
		shot->x = x - 38;
		shot->y = y - 31;

		level().entities.enemy_shots.emplace_back(shot);
	}
}

void EnemyMarioHologram::fall_mario() {
	grav_tick += 1;
	grav_tick %= MARIO_GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

	if (dy < 0 && level().terrain.solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = level().terrain.platform_y(x, y + dy);

	if (ny != 0 && dy > 0)
		y = ny;
	else
		y += dy;
}

void EnemyMarioHologram::takeHit(Shot*) {}
