#include "entities/enemies/enemy_sonic.h"
#include "entities/shots/shot_thorn.h"
#include "entities/shots/shot_sonic_lightning.h"
#include "entities/piece_sonic_ring.h"
#include "core/debug.h"
#include "level/world.h"
#include "core/assets.h"

const int sonic_anim_ball_right[] = {0, 1, 4, 2, 3, 4};
const int sonic_anim_ball_left[] = {0, 1, 4, 2, 3, 4};

const int sonic_anim_super_saiyan[] = {0, 1, 2, 1, 0, 1, 2, 3, 2, 1, 2, 3, 4, 3, 2, 3, 4, 5, 4, 3, 4, 5,
									   6, 5, 4, 5, 6, 7, 6, 5, 6, 7, 8, 7, 6, 7, 8, 9, 8, 7, 8, 9, 10};

EnemySonic::EnemySonic()
	: speed(1), speed_tick(0), attack_delay(50 + rand() % 250), wait_for_attack(0), hp_subcritical(3750), hp_critical(2500), hp_supercritical(1250), ring(90) {
	hp = 10000;
	xmin = 1280;
	attack_frame_tick = 0;
	attack_frame = 0;
	attack_phase = 0;
	nb_shots = 0;
	wait_blood_fall = 0;
}

void EnemySonic::update() {
	if (level().rules.flags[0] == 7) destroyed = true;

	if (wait_blood_fall < 400) {
		wait_blood_fall++;
		return;
	} else if (level().rules.flags[0] < 4) {
		level().rules.flags[0] = 4;
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

			switch (attack_type) {
				case 0:
					onAttackhorizontal();
					break;

				case 1:
					onAttackvertical();
					break;

				case 2:
					onAttackThorns();
					break;

				case 3:
					onAttackLightning();
					break;
			}
			break;
	}
}

void EnemySonic::onWalk() {
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
	if ((wait_for_attack >= attack_delay) && (ring == 0)) {
		if (session().targeted_player != NULL) {
			if ((session().targeted_player->y > 0) && (((dir == FACING_RIGHT) && (((session().targeted_player->x) - x) > 0)) ||
													   ((dir == FACING_LEFT) && (((session().targeted_player->x) - x) < 0)))) {
				int rnd = rand() % 3;
				switch (rnd) {
					case 0:
						attack_type = 0;
						speed = 12;
						status = STATE_SHOOT;
						x_target = session().targeted_player->x;
						y_target = session().targeted_player->y;
						nb_shots = 0;
						attack_phase = -1;
						anim_tick = 0;
						anim_step = 0;
						onAttackhorizontal();
						return;
						break;

					case 1:
						attack_phase = -1;
						attack_type = 1;
						speed = 12;
						status = STATE_SHOOT;
						x_target = session().targeted_player->x;
						y_target = session().targeted_player->y;
						nb_shots = 0;
						anim_tick = 0;
						anim_step = 0;
						onAttackvertical();
						return;
						break;
					case 2:
						attack_type = 3;
						anim_tick = 0;
						anim_step = 0;
						nb_shots = 0;
						status = STATE_SHOOT;
						onAttackLightning();
						return;
						break;
				}
			}
		}
	}

	if ((ring > 0) && (hp < 9000)) {
		if (ring % 30 == 0) {
			onThrowRings();
		}
		ring--;
	}

	anim_tick += 1;
	anim_tick %= (10 - speed);
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}
	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = assets().pics.enemies.anim("sonic_walk_right")[anim_step];
	} else {
		walk(-speed);
		pic = assets().pics.enemies.anim("sonic_walk_left")[anim_step];
	}
	if (speed < SONIC_SPEED) {
		speed_tick++;
		if (speed_tick >= 15) {
			speed_tick = 0;
			speed++;
		}
	}
	colFromPic();
}

void EnemySonic::onDie() {
	if (level().rules.flags[2] == 0) {
		fall();
		if (x - speed < xmin || level().terrain.solid_wall(x - speed, y)) {
			dir = FACING_RIGHT;
			speed = 1;
		} else if (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y)) {
			dir = FACING_LEFT;
			speed = 1;
		}
		anim_tick += 1;
		anim_tick %= (10 - speed);
		if (anim_tick == 0) {
			anim_step += 1;
			anim_step %= 8;
		}
		if (dir == FACING_RIGHT) {
			walk(speed);
			pic = assets().pics.enemies.anim("sonic_walk_right")[anim_step];
		} else {
			walk(-speed);
			pic = assets().pics.enemies.anim("sonic_walk_left")[anim_step];
		}
		if (speed < SONIC_SPEED) {
			speed_tick++;
			if (speed_tick >= 15) {
				speed_tick = 0;
				speed++;
			}
		}

		if ((level().terrain.platform_y2(x, y) == 0) && (x > 1600) && (x < 1630)) {
			level().rules.flags[2] = 1;
			anim_step = 0;
			anim_tick = 0;
			dir = FACING_LEFT;
		}
	} else if (level().rules.flags[2] == 1) {
		y -= 1;
		anim_tick++;
		anim_tick %= 5;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step >= 43) {
				level().rules.flags[2] = 2;
				destroyed = true;
			} else {
				pic = assets().pics.enemies.anim("sonic_power_up_left")[sonic_anim_super_saiyan[anim_step]];
			}
		}
	}
}

void EnemySonic::onCharge() {}

void EnemySonic::onJump() {
	int yp;

	fall();

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		anim_tick = 0;
		anim_step = 0;
		onWalk();
		return;
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

	anim_tick += 1;
	anim_tick %= (10 - speed);
	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}
	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = assets().pics.enemies.anim("sonic_walk_right")[anim_step];
	} else {
		walk(-speed);
		pic = assets().pics.enemies.anim("sonic_walk_left")[anim_step];
	}
	colFromPic();
}

void EnemySonic::onAttackhorizontal() {
	if (attack_phase == -1) {
		if (hp < hp_subcritical) {
			attack_frame_tick += 1;
			attack_frame_tick %= 2;
			if (attack_frame_tick == 0) {
				attack_frame += 1;
				attack_frame %= 16;
			}
		}
		nb_shots += 1;
		if (nb_shots == 50) {
			attack_phase = 0;
		} else {
			step(2, 5);
			if (dir == FACING_RIGHT) {
				pic = assets().pics.enemies.anim("sonic_dash_right")[anim_step];
			} else {
				pic = assets().pics.enemies.anim("sonic_dash_left")[anim_step];
			}
		}

	} else {
		if (attack_phase == 0) {
			if (hp < hp_critical) {
				attack_frame_tick += 1;
				attack_frame_tick %= 2;
				if (attack_frame_tick == 0) {
					attack_frame += 1;
					attack_frame %= 16;
				}
			}
			fall();
			if ((dir == FACING_RIGHT) && (x + speed > 1900)) {
				speed = 0;
				dy = -12;
				attack_phase = 1;
			}
			if ((dir == FACING_LEFT) && (x - speed < 1300)) {
				speed = 0;
				dy = -12;
				attack_phase = 1;
			}
		} else if (attack_phase == 1) {
			fall();
			if (hp < hp_supercritical) {
				attack_frame_tick += 1;
				attack_frame_tick %= 2;
				if (attack_frame_tick == 0) {
					attack_frame += 1;
					attack_frame %= 16;
				}
			}
			if (y + dy < y_target) {
				if (dir == FACING_RIGHT) {
					attack_phase = 2;
					dir = FACING_LEFT;
					dy = 0;
					speed = 12;
				} else {
					attack_phase = 2;
					dir = FACING_RIGHT;
					dy = 0;
					speed = 12;
				}
			}
		} else if (attack_phase == 2) {
			if (x - speed < xmin || level().terrain.solid_wall(x - speed, y)) {
				dir = FACING_RIGHT;
				speed = 1;
				status = STATE_WALK;
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				anim_tick = 0;
				anim_step = 0;
				onWalk();
				return;
			} else if (x + speed > level().camera.offset + 640 || level().terrain.solid_wall(x + speed, y)) {
				dir = FACING_LEFT;
				speed = 1;
				status = STATE_WALK;
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				anim_tick = 0;
				anim_step = 0;
				onWalk();
				return;
			}
		}
		if (dir == FACING_RIGHT) {
			x += speed;
			pic = assets().pics.enemies.anim("sonic_ball_right")[animate(sonic_anim_ball_right, 6, 3)];
		} else {
			x -= speed;
			pic = assets().pics.enemies.anim("sonic_ball_left")[animate(sonic_anim_ball_left, 6, 3)];
		}
	}
	colFromPic();
}

void EnemySonic::onAttackvertical() {
	if (attack_phase == -1) {
		if (hp < hp_subcritical) {
			attack_frame_tick += 1;
			attack_frame_tick %= 2;
			if (attack_frame_tick == 0) {
				attack_frame += 1;
				attack_frame %= 16;
			}
		}
		nb_shots += 1;
		if (nb_shots == 50) {
			attack_phase = 0;
		} else {
			step(2, 5);
			if (dir == FACING_RIGHT) {
				pic = assets().pics.enemies.anim("sonic_dash_right")[anim_step];
			} else {
				pic = assets().pics.enemies.anim("sonic_dash_left")[anim_step];
			}
		}

	} else {
		if (attack_phase == 0) {
			fall();
			if (hp < hp_critical) {
				attack_frame_tick += 1;
				attack_frame_tick %= 2;
				if (attack_frame_tick == 0) {
					attack_frame += 1;
					attack_frame %= 16;
				}
			}
			if ((dir == FACING_RIGHT) && (x + speed > 1900)) {
				speed = 0;
				dy = -12;
				attack_phase = 1;
			} else if ((dir == FACING_LEFT) && (x - speed < 1300)) {
				speed = 0;
				dy = -12;
				attack_phase = 1;
			}
		} else if (attack_phase == 1) {
			fall();
			if (hp < hp_supercritical) {
				attack_frame_tick += 1;
				attack_frame_tick %= 2;
				if (attack_frame_tick == 0) {
					attack_frame += 1;
					attack_frame %= 16;
				}
			}
			if (y + dy < 40) {
				if (dir == FACING_RIGHT) {
					attack_phase = 2;
					dir = FACING_LEFT;
					speed = -dy;
					dy = 0;
				} else {
					attack_phase = 2;
					dir = FACING_RIGHT;
					speed = -dy;
					dy = 0;
				}
			}
		} else if (attack_phase == 2) {
			if ((dir == FACING_RIGHT) && (x > x_target)) {
				attack_phase = 3;
				dy = 12;
				speed = 0;
			} else if ((dir == FACING_LEFT) && (x < x_target)) {
				attack_phase = 3;
				dy = 12;
				speed = 0;
			}
		} else if (attack_phase == 3) {
			if (y + dy > level().terrain.platforms[0][x]) {
				fall();
				y = level().terrain.platforms[0][x];
				dy = 0;
				anim_tick = 0;
				anim_step = 0;
				speed = 1;
				status = STATE_WALK;
				wait_for_attack = 0;
				attack_delay = 50 + rand() % 250;
				onWalk();
				return;
			} else {
				y += dy;
			}
		}
		if (dir == FACING_RIGHT) {
			x += speed;
			pic = assets().pics.enemies.anim("sonic_ball_right")[animate(sonic_anim_ball_right, 6, 3)];
		} else {
			x -= speed;
			pic = assets().pics.enemies.anim("sonic_ball_left")[animate(sonic_anim_ball_left, 6, 3)];
		}
	}
	colFromPic();
}

void EnemySonic::onAttackThorns() {
	nb_shots += 1;
	if (nb_shots > 200) {
		speed = 1;
		status = STATE_WALK;
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		anim_tick = 0;
		anim_step = 0;
		onWalk();
		return;
	}

	anim_tick += 1;
	if (hp > hp_subcritical) {
		anim_tick %= 5;
	} else if (hp > hp_critical) {
		anim_tick %= 4;
	} else if (hp > hp_supercritical) {
		anim_tick %= 3;
	} else {
		anim_tick %= 2;
	}

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 5;
	}
	if (dir == FACING_RIGHT) {
		pic = assets().pics.enemies.anim("sonic_dash_right")[anim_step];
	} else {
		pic = assets().pics.enemies.anim("sonic_dash_left")[anim_step];
	}
	attack_frame_tick += 1;
	attack_frame_tick %= 1;
	if (attack_frame_tick == 0) {
		attack_frame += 1;
		attack_frame %= 16;
	}
	colFromPic();
}

void EnemySonic::onAttackLightning() {
	anim_tick++;
	anim_tick %= 5;
	if (anim_tick == 0) {
		anim_step++;
	}
	if (anim_step == 23) {
		speed = 1;
		status = STATE_WALK;
		wait_for_attack = 0;
		attack_delay = 50 + rand() % 250;
		anim_tick = 0;
		anim_step = 0;
		onWalk();
		return;
	}

	if ((anim_step == 5) && (anim_tick == 0)) {
		if ((nb_shots <= 1) && (hp < hp_subcritical)) {
			anim_step = 0;
			anim_tick = 1;
		} else if ((nb_shots <= 2) && (hp < hp_critical)) {
			anim_step = 0;
			anim_tick = 1;
		} else if ((nb_shots <= 3) && (hp < hp_supercritical)) {
			anim_step = 0;
			anim_tick = 1;
		}
	}

	if ((anim_step == 0) && (anim_tick == 1) && (session().targeted_player != NULL)) {
		ShotSonicLightning* shot;
		shot = new ShotSonicLightning();
		shot->setDir(dir);
		shot->x = session().targeted_player->x;
		shot->y = 0;
		level().entities.enemy_shots.emplace_back(shot);
		nb_shots += 1;
	}

	if (dir == FACING_RIGHT) {
		y -= 1;
		pic = assets().pics.enemies.anim("sonic_power_up_right")[anim_tick];
	} else {
		y -= 1;
		pic = assets().pics.enemies.anim("sonic_power_up_left")[anim_tick];
	}

	colFromPic();
}

void EnemySonic::onThrowRings() {
	Sprite* s;
	int i, mx = 1, my = 1;

	for (i = 0; i < 4; i++) {
		switch (i) {
			case 1:
				mx = -1;
				break;

			case 2:
				my = -1;
				break;

			case 3:
				mx = 1;
				break;
		}
		s = new PieceSonicRing(1 * mx, 10 * my);
		s->x = x;
		s->y = y - 40;
		level().entities.splatters.emplace_back(s);

		s = new PieceSonicRing(3 * mx, 9 * my);
		s->x = x;
		s->y = y - 40;
		level().entities.splatters.emplace_back(s);

		s = new PieceSonicRing(4 * mx, 7 * my);
		s->x = x;
		s->y = y - 40;
		level().entities.splatters.emplace_back(s);

		s = new PieceSonicRing(5 * mx, 6 * my);
		s->x = x;
		s->y = y - 40;
		level().entities.splatters.emplace_back(s);

		s = new PieceSonicRing(6 * mx, 5 * my);
		s->x = x;
		s->y = y - 40;
		level().entities.splatters.emplace_back(s);

		s = new PieceSonicRing(7 * mx, 4 * my);
		s->x = x;
		s->y = y - 40;
		level().entities.splatters.emplace_back(s);

		s = new PieceSonicRing(9 * mx, 3 * my);
		s->x = x;
		s->y = y - 40;
		level().entities.splatters.emplace_back(s);

		s = new PieceSonicRing(10 * mx, 1 * my);
		s->x = x;
		s->y = y - 40;
		level().entities.splatters.emplace_back(s);
	}
}

void EnemySonic::throw_thorns() {
	ShotThorn* shot;
	switch (attack_frame) {
		case 0:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[0], 12, 0);
			shot->setDir(dir);
			shot->x = x + 23;
			shot->y = y - 13;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 1:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[1], 9, -2);
			shot->setDir(dir);
			shot->x = x + 23;
			shot->y = y - 18;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 2:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[2], 8, -8);
			shot->setDir(dir);
			shot->x = x + 18;
			shot->y = y - 23;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 3:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[3], 2, -9);
			shot->setDir(dir);
			shot->x = x + 13;
			shot->y = y - 23;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 4:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[4], 0, -12);
			shot->setDir(dir);
			shot->x = x + 0;
			shot->y = y - 23;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 5:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[5], -2, -9);
			shot->setDir(dir);
			shot->x = x - 13;
			shot->y = y - 23;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 6:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[6], -8, -8);
			shot->setDir(dir);
			shot->x = x - 18;
			shot->y = y - 23;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 7:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[7], -9, -2);
			shot->setDir(dir);
			shot->x = x - 23;
			shot->y = y - 18;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 8:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[8], -12, 0);
			shot->setDir(dir);
			shot->x = x - 23;
			shot->y = y - 13;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 9:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[9], -9, 2);
			shot->setDir(dir);
			shot->x = x - 23;
			shot->y = y - 8;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 10:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[10], -8, 8);
			shot->setDir(dir);
			shot->x = x - 18;
			shot->y = y - 3;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 11:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[11], -2, 9);
			shot->setDir(dir);
			shot->x = x - 13;
			shot->y = y - 3;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 12:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[12], 0, 12);
			shot->setDir(dir);
			shot->x = x - 0;
			shot->y = y - 0;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 13:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[13], 2, 9);
			shot->setDir(dir);
			shot->x = x + 13;
			shot->y = y - 3;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 14:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[14], 8, 8);
			shot->setDir(dir);
			shot->x = x + 18;
			shot->y = y - 8;
			level().entities.enemy_shots.emplace_back(shot);
			break;

		case 15:
			shot = new ShotThorn(assets().pics.enemies.anim("sonic_thorn")[15], 9, 2);
			shot->setDir(dir);
			shot->x = x + 18;
			shot->y = y - 13;
			level().entities.enemy_shots.emplace_back(shot);
			break;
	}
}

void EnemySonic::onCharred() { destroyed = true; }

void EnemySonic::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);
}
