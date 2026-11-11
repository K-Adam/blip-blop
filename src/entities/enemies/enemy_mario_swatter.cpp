#include "entities/enemies/enemy_mario_swatter.h"
#include "entities/enemies/enemy_luigi.h"
#include "entities/gore_splatter.h"
#include "level/events/event_hold_fire.h"

const int anim_mario_walk_right[] = {0, 1, 2, 1};
const int anim_mario_walk_left[] = {0, 1, 2, 1};
const int anim_shrunk_right[] = {0, 1};
const int anim_shrunk_left[] = {0, 1};
const int anim_transformation_right[] = {0, 1};
const int anim_transformation_left[] = {0, 1};

EnemyMarioSwatter::EnemyMarioSwatter()
	: helping_luigi(0), speed(1), speed_tick(0), attack_delay(50 + rnd() % 250), wait_for_attack(0), shrink_timer(0), shrunk(0), knockback(0) {
	hp = 10000;
	xmin = 1280;
	pic = assets().pics.enemies.anim("mario_walk_left")[0];
}

void EnemyMarioSwatter::update() {
	if (level().rules.flags[0] < 1) {
		return;
	}

	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_CHARRED:
		case STATE_DYING:
			onDie();
			break;

		case STATE_JUMP:
		case STATE_FALL:
			onJump();
			break;

		case STATE_SHOOT:
			onCharge();
			break;
	}
}

void EnemyMarioSwatter::onWalk() {
	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
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

	wait_for_attack++;

	if ((wait_for_attack >= attack_delay) && (session().targeted_player != NULL)) {
		if ((level().terrain.platform_y2(session().targeted_player->x, session().targeted_player->y) == level().terrain.platform_y2(x, y)) &&
			(((dir == FACING_RIGHT) && (x < session().targeted_player->x)) || ((dir == FACING_LEFT) && (x > session().targeted_player->x)))) {
			anim_step = 0;
			anim_tick = 0;
			attack_step = 0;
			speed = 0;
			dy = 0;
			status = STATE_SHOOT;
			onCharge();
			return;
		} else {
			int dif_y = session().targeted_player->y - y;
			int dif_x = session().targeted_player->x - x;

			if ((dir == FACING_RIGHT) && (dif_y < 0) && (dif_x > 0) && (dif_y + dif_x < 0)) {
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				attack_delay = 50 + rnd() % 250;
				speed = ((dif_x * -9) / dif_y);
				dy = (int)(-9 + speed * 0.25);
				status = STATE_JUMP;
				grav_tick = 0;	// Otherwise jump heights vary
				onJump();
				return;
			} else if ((dir == FACING_LEFT) && (dif_y < 0) && (dif_x < 0) && (dif_y - dif_x < 0)) {
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				attack_delay = 50 + rnd() % 250;
				speed = ((dif_x * 9) / dif_y);
				dy = (int)(-9 + speed * 0.25);
				status = STATE_JUMP;
				grav_tick = 0;	// Otherwise jump heights vary
				onJump();
				return;
			}
		}
	}

	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = assets().pics.enemies.anim("mario_walk_right")[animate(anim_mario_walk_right, 4, 16 - (3 * speed))];

		if (knockback < 0) {
			if (speed > -knockback / GROUND_SPEED_DIVISOR) {
				speed += knockback / GROUND_SPEED_DIVISOR;
			} else {
				speed = 0;
			}
		} else {
			if (speed + knockback / GROUND_SPEED_DIVISOR < MARIO_SPEED) {
				speed += knockback / GROUND_SPEED_DIVISOR;
			} else {
				speed = MARIO_SPEED;
			}
		}
	} else {
		walk(-speed);
		pic = assets().pics.enemies.anim("mario_walk_left")[animate(anim_mario_walk_left, 4, 16 - (3 * speed))];

		if (knockback > 0) {
			if (speed > knockback / GROUND_SPEED_DIVISOR) {
				speed -= knockback / GROUND_SPEED_DIVISOR;
			} else {
				speed = 0;
			}
		} else {
			if (speed - knockback / GROUND_SPEED_DIVISOR < MARIO_SPEED) {
				speed -= knockback / GROUND_SPEED_DIVISOR;
			} else {
				speed = MARIO_SPEED;
			}
		}
	}

	if (knockback >= GROUND_RECOIL_DIVISOR) {
		if (x + knockback / GROUND_RECOIL_DIVISOR < level().camera.offset + 640) {
			x += knockback / GROUND_RECOIL_DIVISOR;
		}
	} else if (knockback <= -GROUND_RECOIL_DIVISOR) {
		if (x + knockback / GROUND_RECOIL_DIVISOR > xmin) {
			x += knockback / GROUND_RECOIL_DIVISOR;
		}
	}

	knockback = 0;

	if (speed < MARIO_SPEED) {
		speed_tick++;
		if (speed_tick >= 20) {
			speed_tick = 0;
			speed++;
		}
	}

	colFromPic();
}

void EnemyMarioSwatter::onDie() {
	if (!shrunk) {
		fall();
		shrink_timer++;
		if (shrink_timer >= SHRINK_DELAY) {
			speed = 1;
			shrunk = true;
		} else {
			if (dir == FACING_RIGHT) {
				pic = assets().pics.enemies.anim("mario_shrink_right")[animate(anim_transformation_right, 2, 6)];
			} else {
				pic = assets().pics.enemies.anim("mario_shrink_left")[animate(anim_transformation_left, 2, 6)];
			}
		}
	} else {
		fall();

		if (level().rules.flags[0] >= 3) {
			if (!helping_luigi) {
				if (x - speed < xmin || level().terrain.solid_wall(x - speed, y)) {
					dir = FACING_RIGHT;
					speed = 2;
				} else if (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y)) {
					dir = FACING_LEFT;
					speed = 2;
				}
			}

			if (dir == FACING_RIGHT) {
				walk(speed);
				pic = assets().pics.enemies.anim("mario_shrunk_right")[animate(anim_shrunk_right, 2, 16 - (3 * speed))];
			} else {
				walk(-speed);
				pic = assets().pics.enemies.anim("mario_shrunk_left")[animate(anim_shrunk_left, 2, 16 - (3 * speed))];
			}
		}
	}

	if ((!helping_luigi) && (shrunk)) {
		level().rules.flags[3] = 1;
		helping_luigi = 1;
		dir = FACING_RIGHT;
		Enemy* luigi = new EnemyLuigi();

		session().enemies_created++;

		luigi->x = 1960;
		luigi->y = y;
		luigi->dir = FACING_LEFT;
		level().entities.enemies.emplace_back(luigi);

		EventHoldFire e;

		e.x_activation = 0;
		e.flag = 0;
		e.val = 3;
		e.doEvent();
	}
}

void EnemyMarioSwatter::onCharge() {
	speed = MARIO_CHARGE_SPEED;
	if (x - speed < xmin || level().terrain.solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
		anim_step = 0;
		anim_tick = 0;
		status = STATE_WALK;
		wait_for_attack = 0;
		attack_delay = 50 + rnd() % 250;
		knockback = 0;
		onWalk();
		return;
	} else if (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
		anim_step = 0;
		anim_tick = 0;
		status = STATE_WALK;
		wait_for_attack = 0;
		attack_delay = 50 + rnd() % 250;
		knockback = 0;
		onWalk();
		return;
	}
	attack_step += 1;
	if (attack_step < 40) {
		if (dir == FACING_RIGHT) {
			pic = assets().pics.enemies.anim("mario_walk_right")[animate(anim_mario_walk_right, 4, 3)];
		} else {
			pic = assets().pics.enemies.anim("mario_walk_left")[animate(anim_mario_walk_left, 4, 3)];
		}

		if (knockback > 1000) {
			speed = 1;
			anim_step = 0;
			anim_tick = 0;
			status = STATE_WALK;
			wait_for_attack = 0;
			attack_delay = 50 + rnd() % 250;
			knockback = 0;
			onWalk();
			return;
		}
	} else {
		if (dir == FACING_RIGHT) {
			walk(speed);
			pic = assets().pics.enemies.anim("mario_walk_right")[animate(anim_mario_walk_right, 4, 2)];
		} else {
			walk(-speed);
			pic = assets().pics.enemies.anim("mario_walk_left")[animate(anim_mario_walk_left, 4, 2)];
		}
	}
	colFromPic();
}

void EnemyMarioSwatter::onJump() {
	int yp;

	if (dy < 0) {
		fall_mario();
	} else {
		fall();
	}

	// A boss off screen could not be hit
	if ((x - speed < xmin || level().terrain.solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
	}

	else if ((x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y))) {
		dir = FACING_LEFT;
		speed = 1;
	}

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		if (speed > MARIO_SPEED) {
			speed = MARIO_SPEED;
		}
		onWalk();
		return;
	}

	if (!shrunk) {
		if (dir == FACING_RIGHT) {
			x += speed;
			if (dy < 0) {
				pic = assets().pics.enemies.anim("mario_jump_right")[0];
			} else {
				pic = assets().pics.enemies.anim("mario_jump_right")[1];
			}

			if (knockback != 0) {
				x += knockback / AIR_RECOIL_DIVISOR;
			}

			if (knockback < 0) {
				if (speed + knockback / AIR_SPEED_DIVISOR < 0) {
					if (speed + knockback / AIR_SPEED_DIVISOR < -2 * MARIO_SPEED) {
						speed = -2 * MARIO_SPEED;
					} else {
						speed += knockback / AIR_SPEED_DIVISOR;
					}
				} else {
					speed += knockback / AIR_SPEED_DIVISOR;
				}

			} else {
				if (speed + knockback / AIR_SPEED_DIVISOR < 2 * MARIO_SPEED) {
					speed += knockback / AIR_SPEED_DIVISOR;
				} else {
					speed = 2 * MARIO_SPEED;
				}
			}
		} else {
			x -= speed;
			if (dy < 0) {
				pic = assets().pics.enemies.anim("mario_jump_left")[0];
			} else {
				pic = assets().pics.enemies.anim("mario_jump_left")[1];
			}

			if (knockback != 0) {
				x += knockback / AIR_RECOIL_DIVISOR;
			}

			if (knockback > 0) {
				if (speed - knockback / AIR_SPEED_DIVISOR < 0) {
					if (speed - knockback / AIR_SPEED_DIVISOR < -2 * MARIO_SPEED) {
						speed = -2 * MARIO_SPEED;
					} else {
						speed -= knockback / AIR_SPEED_DIVISOR;
					}
				} else {
					speed -= knockback / AIR_SPEED_DIVISOR;
				}

			} else {
				if (speed - knockback / AIR_SPEED_DIVISOR < 2 * MARIO_SPEED) {
					speed -= knockback / AIR_SPEED_DIVISOR;
				} else {
					speed = 2 * MARIO_SPEED;
				}
			}
		}
	} else {
		if (dir == FACING_RIGHT) {
			x += speed;
			pic = assets().pics.enemies.anim("mario_shrunk_right")[animate(anim_shrunk_right, 2, 16 - (3 * speed))];
		} else {
			x -= speed;
			pic = assets().pics.enemies.anim("mario_shrunk_left")[animate(anim_shrunk_left, 2, 16 - (3 * speed))];
		}
	}

	knockback = 0;
	colFromPic();
}

void EnemyMarioSwatter::onCharred() { destroyed = true; }

void EnemyMarioSwatter::fall_mario() {
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

void EnemyMarioSwatter::takeHit(Shot* shot) {
	for (int i = 0; i < 4; i++) {
		GoreSplatter* s;
		if ((shot->dir >= 2) && (shot->dir <= 6)) {
			s = new GoreSplatter(rnd() % 6, -2 - rnd() % 6);
		} else if ((shot->dir >= 10) && (shot->dir <= 14)) {
			s = new GoreSplatter(-rnd() % 5 - 1, -2 - rnd() % 6);
		} else {
			s = new GoreSplatter(rnd() % 11 - 5, -2 - rnd() % 6);
		}
		s->x = x + rnd() % 11 - 5;
		s->y = y - rnd() % 30 - 10;

		level().entities.splatters.emplace_back(s);
	}

	if ((shot->dir >= 2) && (shot->dir <= 6)) {
		knockback += shot->damage();
	} else if ((shot->dir >= 10) && (shot->dir <= 14)) {
		knockback -= shot->damage();
	}

	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);
}
