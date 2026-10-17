#include "entities/enemies/enemy_mario.h"
#include "entities/shots/shot_mario_fireball.h"
#include "entities/shots/shot_mario_fireball_vertical.h"
#include "entities/enemies/enemy_mario_hologram.h"

const int anim_mario_walk_right[] = {0, 1, 2, 1};
const int anim_mario_walk_left[] = {0, 1, 2, 1};
const int mario_recoil[] = {4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

EnemyMario::EnemyMario() : speed(1), speed_tick(0), attack_delay(50 + rand() % 50), wait_for_attack(0), fireball(0), hologram(1), nb_rain(0) {
	hp = 50000;
	pic = pbk_enemies.anim("mario_walk_left")[0];
}

void EnemyMario::update() {
	xmin = offset + 40;

	if (game_flag[0] == 0) {
		fall();
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
			switch (attack_type) {
				case 0:
					onShootHorizontal();
					break;

				case 1:
					onShootVertical();
					break;

				case 2:
					onBurstHorizontal();
					break;

				case 3:
					onBurstVertical();
					break;

				case 4:
					onBurstVerticalReverse();
					break;

				case 5:
					onBurstHorizontalDouble();
					break;

				case 6:
					onBurstVerticalDouble();
					break;

				case 7:
					onShootCross();
					break;

				case 8:
					onBurstHorizontalSteady();
					break;

				case 9:
					onBurstVerticalSteady();
					break;

				case 10:
					onBurstCross();
					break;

				case 11:
					onFireRain();
					break;

				case 12:
					onBurstVerticalHoming();
					break;

				case 13:
					onBurstHorizontalHoming();
					break;

				case 14:
					onBurstCrossHoming();
					break;

				case 15:
					onBurstBerserker();
					break;
			}
			break;
	}
}

void EnemyMario::onWalk() {
	if (platform_y(x, y) == 0) {
		status = STATE_FALL;
		dy = 0;
		grav_tick = 0;
		onJump();
	}

	if (x - speed < xmin || solid_wall(x - speed, y)) {
		dir = FACING_RIGHT;
		speed = 1;
	} else if (x + speed > offset + 600 || solid_wall(x + speed, y)) {
		dir = FACING_LEFT;
		speed = 1;
	}
	wait_for_attack++;
	if ((wait_for_attack >= attack_delay) && (targeted_player != NULL)) {
		if (((dir == FACING_RIGHT) && (targeted_player->x > x)) || ((dir == FACING_LEFT) && (targeted_player->x < x))) {
			anim_step = 0;
			anim_tick = 0;
			speed = 0;
			dy = -6;
			wait_for_attack = 0;
			status = STATE_SHOOT;
			y_target = targeted_player->y;
			x_target = targeted_player->x;
			attack_step = 0;
			attack_tick = 0;
			attack_phase = 0;

			if (hp < MARIO_HP_SUPERCRITICAL) {
				attack_type = rand() % 5 + 11;
				attack_delay = 30 + rand() % 30;
			} else if (hp < MARIO_HP_CRITICAL) {
				attack_type = rand() % 6 + 5;
				attack_delay = 50 + rand() % 50;
			} else if (hp < MARIO_HP_SUBCRITICAL) {
				attack_type = rand() % 5 + 2;
				attack_delay = 100 + rand() % 50;
			} else {
				attack_type = rand() % 5;
				attack_delay = 150 + rand() % 50;
			}
			switch (attack_type) {
				case 0:
					fireball = true;
					onShootHorizontal();
					break;

				case 1:
					fireball = true;
					onShootVertical();
					break;

				case 2:
					fireball = true;
					onBurstHorizontal();
					break;

				case 3:
					fireball = true;
					onBurstVertical();
					break;

				case 4:
					if (dir == FACING_RIGHT) {
						attack_step = 1 + (offset + 640 - x) / 100;
					} else {
						attack_step = 1 + (x - offset) / 100;
					}
					fireball = true;
					onBurstVerticalReverse();
					break;

				case 5:
					fireball = true;
					onBurstHorizontalDouble();
					break;

				case 6:
					fireball = true;
					onBurstVerticalDouble();
					break;

				case 7:
					fireball = true;
					onShootCross();
					break;

				case 8:
					nb_rain = 4 + rand() % 7;
					fireball = true;
					onBurstHorizontalSteady();
					break;

				case 9:
					fireball = true;
					nb_rain = 3 + rand() % 5;
					onBurstVerticalSteady();
					break;

				case 10:
					fireball = true;
					nb_rain = 4 + rand() % 5;
					onBurstCross();
					break;

				case 11:
					speed = 5;
					nb_rain = 3 + rand() % 4;
					onFireRain();
					break;

				case 12:
					fireball = true;
					nb_rain = 3 + rand() % 3;
					onBurstVerticalHoming();
					break;

				case 13:
					fireball = true;
					nb_rain = 3 + rand() % 3;
					onBurstHorizontalHoming();
					break;

				case 14:
					fireball = true;
					nb_rain = 3 + rand() % 4;
					onBurstCrossHoming();
					break;

				case 15:
					speed = 5;
					fireball = false;
					onBurstBerserker();
					break;
			}
			return;
		}
	}
	if (dir == FACING_RIGHT) {
		walk(speed);
		pic = pbk_enemies.anim("mario_walk_right")[animate(anim_mario_walk_right, 4, 16 - (3 * speed))];
	} else {
		walk(-speed);
		pic = pbk_enemies.anim("mario_walk_left")[animate(anim_mario_walk_left, 4, 16 - (3 * speed))];
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

void EnemyMario::onDie() { game_flag[0] = 7; }

void EnemyMario::onCharge() {}

void EnemyMario::onJump() {
	int yp;

	if (dy < 0) {
		fall_mario();
	} else {
		fall();
	}

	// A boss off screen could not be hit
	if ((dir == FACING_LEFT) && (x - speed < xmin || solid_wall(x - speed, y))) {
		dir = FACING_RIGHT;
		speed = 1;
	}

	else if ((dir == FACING_RIGHT) && (x + speed > offset + 640 || solid_wall(x + speed, y))) {
		dir = FACING_LEFT;
		speed = 1;
	}

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		onWalk();
		return;
	}

	if (dir == FACING_RIGHT) {
		x += speed;
		if (dy < 0) {
			pic = pbk_enemies.anim("mario_jump_right")[0];
		} else {
			pic = pbk_enemies.anim("mario_jump_right")[1];
		}
	} else {
		x -= speed;
		if (dy < 0) {
			pic = pbk_enemies.anim("mario_jump_left")[0];
		} else {
			pic = pbk_enemies.anim("mario_jump_left")[1];
		}
	}

	colFromPic();
}

void EnemyMario::onCharred() { destroyed = true; }

void EnemyMario::render() {
	Sprite::render();
	if ((fireball) && (anim_step < 3)) {
		if (dir == FACING_RIGHT) {
			draw(x + 29, y - 33, pbk_enemies.anim("mario_muzzle_right")[anim_step]);
		} else {
			draw(x - 29, y - 33, pbk_enemies.anim("mario_muzzle_left")[anim_step]);
		}
	}
}

void EnemyMario::onShootHorizontal() {
	if (dir == FACING_RIGHT)
		pic = pbk_enemies.pic("mario_shoot_right");

	else
		pic = pbk_enemies.pic("mario_shoot_left");

	if ((y - 25 < y_target) && (y < 444)) {
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
		if (y + dy > 444) {
			y = 444;
			dy = 0;
		} else {
			y += dy;
		}
	} else if ((y - 30 > y_target) && (y <= 444)) {
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
		} else if (attack_step >= 40) {
			if (attack_step == 40) {
				fireball = 0;
				shoot_fireball(4);
			}
			if ((dir == FACING_RIGHT) && (x > offset + 10)) {
				x -= mario_recoil[attack_step - 40];
			} else if ((dir == FACING_LEFT) && (x < offset + 630)) {
				x += mario_recoil[attack_step - 40];
			}
		}
	}
	colFromPic();
}

void EnemyMario::onShootVertical() {
	if (dir == FACING_RIGHT)
		pic = pbk_enemies.pic("mario_shoot_right");

	else
		pic = pbk_enemies.pic("mario_shoot_left");

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

		list_enemy_shots.emplace_back(shot);
	} else if (attack_step == 60) {
		status = STATE_WALK;
		anim_step = 0;
		anim_tick = 0;
		speed = 1;
		onWalk();
		return;
	}
	colFromPic();
}

void EnemyMario::onShootCross() {
	switch (attack_phase) {
		case 0:
			onShootHorizontal();
			if (attack_step > 0) {
				attack_phase = 1;
				attack_step = 20;
				attack_tick = 0;
				fireball = true;
			}
			break;

		case 1:
			onShootVertical();
			if (attack_step > 40) {
				attack_phase = 2;
				attack_step = 0;
				attack_tick = 0;
				fireball = true;
			}
			break;

		case 2:
			onShootHorizontal();
			break;
	}
}

void EnemyMario::onBurstHorizontal() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.pic("mario_shoot_right");
	} else {
		pic = pbk_enemies.pic("mario_shoot_left");
	}

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	if ((y >= 140) && (y <= 145)) {
		shoot_fireball(1);
	} else if ((y >= 240) && (y <= 245)) {
		shoot_fireball(1);
	} else if ((y >= 340) && (y <= 345)) {
		shoot_fireball(1);
	} else if ((y >= 440) && (y <= 455)) {
		shoot_fireball(1);
	}

	if (((dy < 0) && (y < 100)) || ((dy > 0) && (y > 435))) {
		anim_step = 0;
		anim_tick = 0;
		status = STATE_FALL;
		fireball = false;
		onJump();

		if (dy > 3) {
			dy = 3;
		}
		return;
	}
	y += dy;
	colFromPic();
}

void EnemyMario::onBurstHorizontalDouble() {
	switch (attack_phase) {
		case 0:
			if (y + dy < 100) {
				attack_phase = 1;
				fireball = false;
			} else {
				onBurstHorizontal();
			}
			break;

		case 1:
			fall();
			if ((dy > 0) && (y < 0)) {
				attack_phase = 2;
				fireball = true;
				dy = 6;
			}
			colFromPic();
			break;

		case 2:
			onBurstHorizontal();
			break;
	}
}

void EnemyMario::onBurstHorizontalSteady() {
	onShootHorizontal();
	if ((nb_rain > attack_phase) && (attack_step > 50)) {
		attack_phase++;
		attack_step = 30;
		attack_tick = 0;
	} else if ((nb_rain > attack_phase) && (attack_step == 40)) {
		fireball = true;
	}
}

void EnemyMario::onBurstHorizontalHoming() {
	onShootHorizontal();
	if ((nb_rain > attack_phase) && (attack_step > 50)) {
		if ((targeted_player != NULL) && ((targeted_player->x > x && dir == FACING_RIGHT) || (targeted_player->x < x && dir == FACING_LEFT))) {
			attack_phase++;
			attack_step = 30;
			attack_tick = 0;
			y_target = targeted_player->y;
		} else {
			fireball = false;
		}
	} else if ((nb_rain > attack_phase) && (attack_step == 40)) {
		fireball = true;
	}
}

void EnemyMario::onBurstVertical() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.pic("mario_shoot_right");
	} else {
		pic = pbk_enemies.pic("mario_shoot_left");
	}

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	attack_tick++;
	attack_tick %= 6;
	if (attack_tick == 0) {
		attack_step++;
		if (dir == FACING_RIGHT) {
			x_target = x + attack_step * 100 + rand() % 20;
			if (x_target > offset + 720) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = STATE_WALK;
				fireball = false;
				onWalk();
				return;
			}
			ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(attack_step, x_target);

			shot->setDir(dir);
			shot->x = x + 38;
			shot->y = y - 38;

			list_enemy_shots.emplace_back(shot);
		} else {
			int x_target = x - attack_step * 100 - rand() % 20;
			if (x_target < offset - 100) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = STATE_WALK;
				fireball = false;
				onWalk();
				return;
			}
			ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(-attack_step, x_target);

			shot->setDir(dir);
			shot->x = x - 38;
			shot->y = y - 38;

			list_enemy_shots.emplace_back(shot);
		}
	}
	colFromPic();
}

void EnemyMario::onBurstVerticalHoming() {
	onShootVertical();
	if ((nb_rain > attack_phase) && (attack_step == 40) && (targeted_player != NULL)) {
		attack_step = 10;
		attack_tick = 0;
		fireball = true;
		attack_phase++;
		x_target = targeted_player->x;
	}
}

void EnemyMario::onBurstVerticalSteady() {
	onShootVertical();
	if ((nb_rain > attack_phase) && (attack_step == 40)) {
		attack_step = 25;
		attack_tick = 0;
		fireball = true;
		attack_phase++;
	}
}

void EnemyMario::onBurstVerticalDouble() {
	switch (attack_phase) {
		case 0:
			if ((dir == FACING_RIGHT) && (x + (attack_step + 1) * 100 + 20 > offset + 620)) {
				attack_phase = 1;
				attack_tick = 0;
				attack_step = 1 + (offset + 640 - x) / 100;
			} else if ((dir == FACING_LEFT) && (x - (attack_step + 1) * 100 - 20 < offset + 20)) {
				attack_phase = 1;
				attack_tick = 0;
				attack_step = 1 + (x - offset) / 100;
			} else {
				onBurstVertical();
			}
			break;

		case 1:
			onBurstVerticalReverse();
			break;
	}
}

void EnemyMario::onBurstVerticalReverse() {
	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.pic("mario_shoot_right");
	} else {
		pic = pbk_enemies.pic("mario_shoot_left");
	}

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 3;
	}

	attack_tick++;
	attack_tick %= 6;
	if (attack_tick == 0) {
		attack_step--;
		if (dir == FACING_RIGHT) {
			x_target = offset + attack_step * 100 + rand() % 20;
			if (x_target < x + 20) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = STATE_WALK;
				fireball = false;
				onWalk();
				return;
			}
			ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(attack_step, x_target);

			shot->setDir(dir);
			shot->x = x + 38;
			shot->y = y - 38;

			list_enemy_shots.emplace_back(shot);
		} else {
			int x_target = offset + 640 - attack_step * 100 - rand() % 20;
			if (x_target > x - 20) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = STATE_WALK;
				fireball = false;
				onWalk();
				return;
			}
			ShotMarioFireballVertical* shot = new ShotMarioFireballVertical(-attack_step, x_target);

			shot->setDir(dir);
			shot->x = x - 38;
			shot->y = y - 38;

			list_enemy_shots.emplace_back(shot);
		}
	}
	colFromPic();
}

void EnemyMario::onBurstCrossHoming() {
	if (attack_phase % 2 == 0) {
		onShootVertical();
		if ((nb_rain > attack_phase) && (attack_step == 40) && (targeted_player != NULL)) {
			attack_step = 30;
			attack_tick = 0;
			fireball = true;
			attack_phase++;
			y_target = targeted_player->y;
		}
	} else {
		onShootHorizontal();
		if ((nb_rain > attack_phase) && (attack_step > 50)) {
			if ((targeted_player != NULL) && ((targeted_player->x > x && dir == FACING_RIGHT) || (targeted_player->x < x && dir == FACING_LEFT))) {
				attack_phase++;
				attack_step = 30;
				attack_tick = 0;
				x_target = targeted_player->x;
			} else {
				fireball = false;
			}
		} else if ((nb_rain > attack_phase) && (attack_step == 40)) {
			fireball = true;
		}
	}
}

void EnemyMario::onBurstCross() {
	if (attack_phase % 2 == 0) {
		onShootVertical();
		if ((nb_rain > attack_phase) && (attack_step == 40)) {
			attack_step = 35;
			attack_tick = 0;
			fireball = true;
			attack_phase++;
		}
	} else {
		onShootHorizontal();
		if ((nb_rain > attack_phase) && (attack_step > 50)) {
			attack_phase++;
			attack_step = 35;
			attack_tick = 0;
		} else if ((nb_rain > attack_phase) && (attack_step == 40)) {
			fireball = true;
		}
	}
}

void EnemyMario::onBurstBerserker() {
	switch (attack_phase) {
		case 0:
			if (dir == FACING_RIGHT) {
				walk(speed);
				pic = pbk_enemies.anim("mario_walk_right")[animate(anim_mario_walk_right, 4, 16 - (3 * speed))];
				if (x > offset + 600) {
					fireball = true;
					attack_phase = 1;
					speed = 1;
					dir = FACING_LEFT;
				}
			} else {
				walk(-speed);
				pic = pbk_enemies.anim("mario_walk_left")[animate(anim_mario_walk_left, 4, 16 - (3 * speed))];
				if (x < offset + 40) {
					fireball = true;
					attack_phase = 1;
					speed = 1;
					dir = FACING_RIGHT;
				}
			}
			colFromPic();
			break;

		case 1:
			if ((dir == FACING_RIGHT) && (x + (attack_step + 1) * 100 + 20 > offset + 620)) {
				attack_phase = 2;
			} else if ((dir == FACING_LEFT) && (x - (attack_step + 1) * 100 - 20 < offset + 20)) {
				attack_phase = 2;
			} else {
				onBurstVertical();
			}

			break;

		case 2:
			if (y + dy < 100) {
				attack_phase = 3;
				fireball = false;
			} else {
				onBurstHorizontal();
			}
			break;

		case 3:
			fall();
			if (y < 0) {
				if (dir == FACING_RIGHT) {
					dir = FACING_LEFT;
					x = offset + 600;
				} else {
					dir = FACING_RIGHT;
					x = offset + 40;
				}
				dy = 6;
				attack_phase = 4;
				fireball = true;
			}
			colFromPic();
			break;

		case 4:
			if ((y + dy) > 435) {
				attack_phase = 5;
				if (dir == FACING_RIGHT) {
					attack_step = 1 + (640 - x) / 100;
				} else {
					attack_step = 1 + x / 100;
				}
				attack_tick = 0;
				anim_step = 0;
				anim_tick = 0;
				dy = 0;
			} else {
				onBurstHorizontal();
			}
			break;

		case 5:
			if ((dir == FACING_RIGHT) && (x + (attack_step - 1) * 100 - 20 < x)) {
				attack_phase = 6;
				attack_step = 0;
				fireball = false;
			} else if ((dir == FACING_LEFT) && (x - (attack_step - 1) * 100 + 20 > x)) {
				attack_phase = 6;
				attack_step = 0;
				fireball = false;
			} else {
				onBurstVerticalReverse();
			}
			break;

		case 6:
			attack_step++;
			if (attack_step >= 160) {
				status = STATE_WALK;
				anim_step = 0;
				anim_tick = 0;
				speed = 1;
				onWalk();
				return;
			}
			break;
	}
}

void EnemyMario::onFireRain() {
	if (attack_phase == 0) {
		if (dir == FACING_RIGHT) {
			walk(speed);
			pic = pbk_enemies.anim("mario_walk_right")[animate(anim_mario_walk_right, 4, 16 - (3 * speed))];
			if (x > offset + 600) {
				fireball = true;
				attack_phase = 1;
				speed = 1;
				dir = FACING_LEFT;
			}
		} else {
			walk(-speed);
			pic = pbk_enemies.anim("mario_walk_left")[animate(anim_mario_walk_left, 4, 16 - (3 * speed))];
			if (x < offset + 40) {
				fireball = true;
				attack_phase = 1;
				speed = 1;
				dir = FACING_RIGHT;
			}
		}
		colFromPic();
	} else {
		if (attack_phase < nb_rain) {
			if (attack_phase % 2 == 0) {
				if ((dir == FACING_RIGHT) && (x + (attack_step - 1) * 100 - 20 < x)) {
					attack_phase++;
					attack_tick = 0;
					attack_step = 0;
				} else if ((dir == FACING_LEFT) && (x - (attack_step - 1) * 100 + 20 > x)) {
					attack_phase++;
					attack_tick = 0;
					attack_step = 0;
				} else {
					onBurstVerticalReverse();
				}
			} else {
				if ((dir == FACING_RIGHT) && (x + (attack_step + 1) * 100 + 20 > 620)) {
					attack_phase++;
					attack_tick = 0;
					attack_step = 1 + (offset + 640 - x) / 100;
				} else if ((dir == FACING_LEFT) && (x - (attack_step + 1) * 100 - 20 < 20)) {
					attack_phase++;
					attack_tick = 0;
					attack_step = 1 + (x - offset) / 100;
				} else {
					onBurstVertical();
				}
			}
		} else {
			anim_step++;
			fireball = false;
			if (anim_step >= 60 + nb_rain * 30) {
				anim_step = 0;
				anim_tick = 0;
				speed = 0;
				status = STATE_WALK;
				onWalk();
				return;
			}
		}
	}
}

inline void EnemyMario::shoot_fireball(int velocity) {
	if (dir == FACING_RIGHT) {
		ShotMarioFireball* shot = new ShotMarioFireball(velocity);

		shot->setDir(dir);
		shot->x = x + 38;
		shot->y = y - 31;

		list_enemy_shots.emplace_back(shot);
	} else {
		ShotMarioFireball* shot = new ShotMarioFireball(-velocity);

		shot->setDir(dir);
		shot->x = x - 38;
		shot->y = y - 31;

		list_enemy_shots.emplace_back(shot);
	}
}

void EnemyMario::fall_mario() {
	grav_tick += 1;
	grav_tick %= MARIO_GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = platform_y(x, y + dy);

	if (ny != 0 && dy > 0)
		y = ny;
	else
		y += dy;
}

void EnemyMario::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 10, 15, 10, 0, -10, -15, -10};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);
}
