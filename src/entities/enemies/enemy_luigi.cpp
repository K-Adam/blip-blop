#include "entities/enemies/enemy_luigi.h"
#include "entities/shots/shot_thorn.h"
#include "entities/gore_splatter.h"
#include "entities/enemies/enemy_sonic.h"
#include "entities/enemies/enemy_pacman_car.h"
#include "level/events/event_hold_fire.h"

const int anim_luigi_walk_right[] = {0, 1, 2, 1};
const int anim_luigi_walk_left[] = {0, 1, 2, 1};
const int anim_luigi_attack_right[] = {0, 1, 2, 1};
const int anim_luigi_attack_left[] = {0, 1, 2, 1};
const int anim_luigi_walk_dead_right[] = {0, 1, 2, 1};
const int anim_luigi_walk_dead_left[] = {0, 1, 2, 1};
const int anim_luigi_death[] = {0, 1, 2, 3, 4, 3, 2, 1};

EnemyLuigi::EnemyLuigi() : speed(1), speed_tick(0), attack_delay(50 + rnd() % 250), wait_for_attack(0), knockback(0) {
	hp = 10000;
	xmin = level().camera.offset + 20;
}

void EnemyLuigi::update() {
	if (level().rules.flags[0] == 7) destroyed = true;

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
			switch (attack_type) {
				case 0:
					break;
				case 1:
					onAttack();
					break;
			}
			break;
	}
}

void EnemyLuigi::onWalk() {
	if (level().terrain.platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onJump();
	}

	if (x - speed < (xmin + 20) || level().terrain.solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > level().camera.offset + 600 || level().terrain.solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}

	if (level().rules.flags[0] >= 3) {
		wait_for_attack++;
	} else if (x < level().camera.offset + 600) {
		level().rules.flags[0] = 2;
	}

	if ((wait_for_attack >= attack_delay) && (session().targeted_player != NULL)) {
		int dif_y = session().targeted_player->y - y;
		int dif_x = session().targeted_player->x - x;

		if (dif_y == 0) {
			dif_y = 1;
		}
		int slope_ratio = dif_x / dif_y;
		if ((dir == FACING_RIGHT) && (dif_y < 0) && (dif_x > 0) && (dif_y + dif_x < 0)) {
			wait_for_attack = 0;
			attack_delay = 10 + rnd() % 50;
			speed = (int)((dif_x * -9) / dif_y);
			dy = (int)(-9 + speed * 0.25);
			anim_step = 0;
			anim_tick = 0;
			status = STATE_JUMP;
			grav_tick = 0;	// Otherwise jump heights vary
			onJump();
			return;

		} else if ((dir == FACING_LEFT) && (dif_y < 0) && (dif_x < 0) && (dif_y - dif_x < 0)) {
			wait_for_attack = 0;
			attack_delay = 10 + rnd() % 50;
			speed = (int)((dif_x * 9) / dif_y);
			dy = (int)(-9 + speed * 0.25);
			anim_step = 0;
			anim_tick = 0;
			status = STATE_JUMP;
			grav_tick = 0;	// Otherwise jump heights vary
			onJump();
			return;
		} else if (hp >= 1000) {
			if ((dir == FACING_RIGHT) && (dif_x > 0) && ((slope_ratio <= -2) || (slope_ratio >= 2))) {
				wait_for_attack = 0;
				attack_delay = 50 + rnd() % 250;
				attack_type = 1;
				anim_tick = 0;
				anim_step = 0;
				speed = 0;
				attack_frame_tick = 0;
				attack_frame = 0;
				status = STATE_SHOOT;
				nb_shots = 0;
				assets().sounds.level.play(9);
				onAttack();
				return;
			} else if ((dir == FACING_LEFT) && (dif_x < 0) && ((slope_ratio <= -2) || (slope_ratio >= 2))) {
				wait_for_attack = 0;
				attack_delay = 50 + rnd() % 250;
				attack_type = 1;
				anim_tick = 0;
				anim_step = 0;
				speed = 0;
				attack_frame_tick = 0;
				attack_frame = 0;
				status = STATE_SHOOT;
				nb_shots = 0;
				assets().sounds.level.play(9);
				onAttack();
				return;
			}
		}
	}
	if (dir == FACING_RIGHT) {
		if (speed > 6)
			speed = 6;
		else if (speed < -6)
			speed = -6;

		walk(speed);
		pic = assets().pics.enemies.anim("luigi_walk_right")[animate(anim_luigi_walk_right, 4, 14 - (2 * speed))];

		if (knockback < 0) {
			if (speed > -knockback / GROUND_SPEED_DIVISOR) {
				speed += knockback / GROUND_SPEED_DIVISOR;
			} else {
				speed = 0;
			}
		} else {
			if (speed + knockback / GROUND_SPEED_DIVISOR < LUIGI_SPEED) {
				speed += knockback / GROUND_SPEED_DIVISOR;
			} else {
				speed = LUIGI_SPEED;
			}
		}
	} else {
		if (speed > 6)
			speed = 6;
		else if (speed < -6)
			speed = -6;

		walk(-speed);
		pic = assets().pics.enemies.anim("luigi_walk_left")[animate(anim_luigi_walk_left, 4, 14 - (2 * speed))];

		if (knockback > 0) {
			if (speed > knockback / GROUND_SPEED_DIVISOR) {
				speed -= knockback / GROUND_SPEED_DIVISOR;
			} else {
				speed = 0;
			}
		} else {
			if (speed - knockback / GROUND_SPEED_DIVISOR < LUIGI_SPEED) {
				speed -= knockback / GROUND_SPEED_DIVISOR;
			} else {
				speed = LUIGI_SPEED;
			}
		}
	}

	if (knockback >= GROUND_RECOIL_DIVISOR) {
		if (x + knockback / GROUND_RECOIL_DIVISOR < level().camera.offset + 620) {
			x += knockback / GROUND_RECOIL_DIVISOR;
		}
	} else if (knockback <= -GROUND_RECOIL_DIVISOR) {
		if (x + knockback / GROUND_RECOIL_DIVISOR > xmin) {
			x += knockback / GROUND_RECOIL_DIVISOR;
		}
	}

	knockback = 0;

	if (speed < LUIGI_SPEED) {
		speed_tick++;
		if (speed_tick >= 20) {
			speed_tick = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemyLuigi::onDie() {
	if (level().rules.flags[1] == 0) {
		fall();
		if (x - 3 < xmin || level().terrain.solid_wall(x - 3, y)) {
			dir = FACING_RIGHT;
		} else if (x + 3 > level().camera.offset + 640 || level().terrain.solid_wall(x + 3, y)) {
			dir = FACING_LEFT;
		}
		if (dir == FACING_RIGHT) {
			walk(3);
			pic = assets().pics.enemies.anim("luigi_dazed_right")[animate(anim_luigi_walk_dead_right, 4, 8)];
		} else {
			walk(-3);
			pic = assets().pics.enemies.anim("luigi_dazed_left")[animate(anim_luigi_walk_dead_left, 4, 8)];
		}
		if ((level().terrain.platform_y2(x, y) == 0) && (x > 1500) && (x < 1550)) {
			dir = FACING_LEFT;
			level().rules.flags[1] = 1;
			attack_delay = 0;
		}
	} else if (level().rules.flags[1] == 1) {
		attack_delay++;
		if (attack_delay >= EXPLODE_DELAY) {
			level().rules.flags[1] = 2;
			dir = FACING_RIGHT;
		} else {
			pic = assets().pics.enemies.anim("luigi_death")[animate(anim_luigi_death, 8, 6)];
		}
	} else if (level().rules.flags[1] == 2) {
		walk(3);
		pic = assets().pics.enemies.anim("luigi_dazed_right")[animate(anim_luigi_walk_dead_right, 4, 8)];
		if (x > level().camera.offset + 680) {
			level().rules.flags[1] = 3;
			attack_delay = 0;
		}
	} else if (level().rules.flags[1] == 3) {
		attack_delay++;
		if (attack_delay >= NB_SPLATTERS) {
			Enemy* sonic = new EnemyPacmanCar();

			sonic->x = 1960;
			sonic->y = y;
			sonic->dir = FACING_LEFT;
			level().entities.enemies.emplace_back(sonic);

			session().enemies_created++;

			sonic = new EnemySonic();

			sonic->x = 1960;
			sonic->y = y;
			sonic->dir = FACING_LEFT;
			level().entities.enemies.emplace_back(sonic);
			destroyed = true;

		} else {
			int i;
			for (i = 0; i < 16; i++) {
				Sprite* s = new GoreSplatter(-rnd() % 10 - 2, -1 - rnd() % 10);
				s->x = x + rnd() % 20 - 10;
				s->y = y - rnd() % 60 - 10;
				level().entities.splatters.emplace_back(s);
			}
		}
	}
}

void EnemyLuigi::onAttack() {
	nb_shots++;
	if (nb_shots > 275) {
		anim_step = 0;
		anim_tick = 0;
		status = STATE_WALK;
		onWalk();
		return;
	}

	if (nb_shots < 50) {
		if (dir == FACING_RIGHT)
			pic = assets().pics.enemies.anim("luigi_attack_right")[0];
		else
			pic = assets().pics.enemies.anim("luigi_attack_left")[0];

		colFromPic();
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("luigi_attack_right")[animate(anim_luigi_attack_right, 4, 2)];
		attack_frame_tick += 1;
		attack_frame_tick %= 4;
		if (attack_frame_tick == 0) {
			attack_frame += 1;
			attack_frame %= 6;
			ShotThorn* shot1;
			ShotThorn* shot2;

			switch (attack_frame) {
				case 0:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[3], 12, 0, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[3], 12, 0, true);

					shot1->setDir(dir);
					shot1->x = x + 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x + 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);

					break;
				case 1:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[2], 11, 1, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[4], 11, -1, true);

					shot1->setDir(dir);
					shot1->x = x + 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x + 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
				case 2:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[1], 10, 2, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[5], 10, -2, true);

					shot1->setDir(dir);
					shot1->x = x + 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x + 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
				case 3:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[0], 9, 3, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[6], 9, -3, true);

					shot1->setDir(dir);
					shot1->x = x + 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x + 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
				case 4:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[1], 10, 2, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[5], 10, -2, true);

					shot1->setDir(dir);
					shot1->x = x + 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x + 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
				case 5:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[2], 11, 1, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_right")[4], 11, -1, true);

					shot1->setDir(dir);
					shot1->x = x + 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x + 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
			}

			if (x > level().camera.offset + 30) {
				x -= 1;
			}
		}
	} else {
		pic = assets().pics.enemies.anim("luigi_attack_left")[animate(anim_luigi_attack_left, 4, 2)];
		attack_frame_tick += 1;
		attack_frame_tick %= 4;
		if (attack_frame_tick == 0) {
			attack_frame += 1;
			attack_frame %= 6;
			ShotThorn* shot1;
			ShotThorn* shot2;

			switch (attack_frame) {
				case 0:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[3], -12, 0, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[3], -12, 0, true);

					shot1->setDir(dir);
					shot1->x = x - 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x - 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);

					break;
				case 1:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[2], -11, 1, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[4], -11, -1, true);

					shot1->setDir(dir);
					shot1->x = x - 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x - 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
				case 2:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[1], -10, 2, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[5], -10, -2, true);

					shot1->setDir(dir);
					shot1->x = x - 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x - 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
				case 3:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[0], -9, 3, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[6], -9, -3, true);

					shot1->setDir(dir);
					shot1->x = x - 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x - 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
				case 4:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[1], -10, 2, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[5], -10, -2, true);

					shot1->setDir(dir);
					shot1->x = x - 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x - 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
				case 5:
					shot1 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[2], -11, 1, true);
					shot2 = new ShotThorn(assets().pics.enemies.anim("luigi_plunger_left")[4], -11, -1, true);

					shot1->setDir(dir);
					shot1->x = x - 23;
					shot1->y = y - 13;

					shot2->setDir(dir);
					shot2->x = x - 23;
					shot2->y = y - 33;

					level().entities.enemy_shots.emplace_back(shot1);
					level().entities.enemy_shots.emplace_back(shot2);
					break;
			}

			if (x < level().camera.offset + 610) {
				x += 1;
			}
		}
	}
	colFromPic();
}

void EnemyLuigi::onJump() {
	int yp;

	if (dy < 0) {
		fall_luigi();
	} else {
		fall();
	}

	// A boss off screen could not be hit
	if ((x - speed < xmin || level().terrain.solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
	}

	else if ((x + speed > level().camera.offset + 600 || level().terrain.solid_wall(x + speed, y))) {
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
		if (x + speed < level().camera.offset + 620) x += speed;

		if (dy < 0) {
			pic = assets().pics.enemies.anim("luigi_jump_right")[0];
		} else {
			pic = assets().pics.enemies.anim("luigi_jump_right")[1];
		}

		if (knockback != 0) {
			x += knockback / AIR_RECOIL_DIVISOR;
		}

		if (knockback < 0) {
			if (speed + knockback / AIR_SPEED_DIVISOR < 0) {
				if (speed + knockback / AIR_SPEED_DIVISOR < -2 * LUIGI_SPEED) {
					speed = -2 * LUIGI_SPEED;
				} else {
					speed += knockback / AIR_SPEED_DIVISOR;
				}
			} else {
				speed += knockback / AIR_SPEED_DIVISOR;
			}

		} else {
			if (speed + knockback / AIR_SPEED_DIVISOR < 2 * LUIGI_SPEED) {
				speed += knockback / AIR_SPEED_DIVISOR;
			} else {
				speed = 2 * LUIGI_SPEED;
			}
		}
	} else {
		if (x - speed > xmin) x -= speed;

		if (dy < 0) {
			pic = assets().pics.enemies.anim("luigi_jump_left")[0];
		} else {
			pic = assets().pics.enemies.anim("luigi_jump_left")[1];
		}

		if (knockback != 0) {
			x += knockback / AIR_RECOIL_DIVISOR;
		}

		if (knockback > 0) {
			if (speed - knockback / AIR_SPEED_DIVISOR < 0) {
				if (speed - knockback / AIR_SPEED_DIVISOR < -2 * LUIGI_SPEED) {
					speed = -2 * LUIGI_SPEED;
				} else {
					speed -= knockback / AIR_SPEED_DIVISOR;
				}
			} else {
				speed -= knockback / AIR_SPEED_DIVISOR;
			}

		} else {
			if (speed - knockback / AIR_SPEED_DIVISOR < 2 * LUIGI_SPEED) {
				speed -= knockback / AIR_SPEED_DIVISOR;
			} else {
				speed = 2 * LUIGI_SPEED;
			}
		}
	}

	knockback = 0;

	colFromPic();
}

void EnemyLuigi::onCharred() { destroyed = true; }

void EnemyLuigi::fall_luigi() {
	grav_tick += 1;
	grav_tick %= LUIGI_GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

	if (dy < 0 && level().terrain.solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = level().terrain.platform_y(x, y + dy);

	if (ny != 0 && dy > 0)
		y = ny;
	else
		y += dy;
}

void EnemyLuigi::takeHit(Shot* shot) {
	Sprite* s;

	for (int i = 0; i < 4; i++) {
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

	if (hp <= 0) {
		EventHoldFire e;

		e.x_activation = 0;
		e.flag = 0;
		e.val = 5;
		e.doEvent();
	}
}
