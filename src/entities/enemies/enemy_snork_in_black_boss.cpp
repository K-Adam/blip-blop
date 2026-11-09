#include "entities/enemies/enemy_snork_in_black_boss.h"
#include "entities/shots/shot_thorn.h"
#include "core/debug.h"

const int anim_snork_in_black_boss_jump_right[] = {0, 1, 2, 1};
const int anim_snork_in_black_boss_jump_left[] = {0, 1, 2, 1};

const int snork_in_black_position_x[] = {5904, 6351, 5831, 6319};
const int snork_in_black_position_y[] = {175, 189, 342, 328};

EnemySnorkInBlackBoss::EnemySnorkInBlackBoss() : position(1), type_attack(0), attack(false), x_shark(5604), y_shark(100), shark(false), jump(false) {
	hp = 15000;
	dy = 0;
	dir = FACING_LEFT;
	pic = pbk_enemies.anim("snork_boss_idle")[0];
}

void EnemySnorkInBlackBoss::update() {
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

		case STATE_CHARRED:
		case STATE_DYING:
			onDie();
			break;

		case STATE_SHOOT:
			onShoot();
			break;
	}

	if (x > offset + 1000) destroyed = true;
}

void EnemySnorkInBlackBoss::onWalk() {
	if (game_flag[0] == 1) {
		if (anim_tick == 0 && anim_step == 0) {
			sbk_level.play(0);
		}

		anim_tick++;
		anim_tick %= 8;

		if (anim_tick == 0) {
			anim_step++;
		}

		if (anim_step >= 18) {
			game_flag[0] = 2;
			anim_step = 0;
			anim_tick = 0;
			mbk_level.play(1);
			game_flag[FLAG_TIMER] = 50;
		} else {
			pic = pbk_enemies.anim("snork_boss_intro")[anim_step];
		}
	} else if (game_flag[0] == 2) {
		anim_step++;
		if (anim_step > 20) {
			if (attack) {
				attack = false;
				anim_tick = 0;

				int destination = rand() % 4;

				if (targeted_player->x > 6200 && targeted_player->y > 200) {
					destination = 2;
				} else if (targeted_player->x < 5950 && targeted_player->y > 200) {
					destination = 3;
				} else if (targeted_player->x > 6200 && targeted_player->y < 200) {
					destination = 0;
				} else if (targeted_player->x < 5950 && targeted_player->y < 200) {
					destination = 1;
				}

				if (destination == position) {
					anim_step = 15;
				} else {
					switch (position) {
						case 0:
							dir = FACING_RIGHT;
							switch (destination) {
								case 1:
									dx = 4;
									dy = -7;
									break;

								case 2:
									dx = -1;
									dy = -2;
									break;

								case 3:
									dx = 5;
									dy = -3;

									break;
							}
							break;

						case 1:
							dir = FACING_LEFT;
							switch (destination) {
								case 0:
									dx = -6;
									dy = -5;
									break;

								case 2:
									dx = -6;
									dy = -3;
									break;

								case 3:
									dx = -1;
									dy = 3;
									break;
							}
							break;

						case 2:
							dir = FACING_RIGHT;
							switch (destination) {
								case 0:
									dx = 1;
									dy = -7;
									break;

								case 1:
									dx = 5;
									dy = -9;
									break;

								case 3:
									dx = 7;
									dy = -5;
									break;
							}
							break;

						case 3:
							dir = FACING_LEFT;
							switch (destination) {
								case 0:
									dx = -6;
									dy = -7;
									break;

								case 1:
									dx = 1;
									dy = -6;
									break;

								case 2:
									dx = -5;
									dy = -6;
									break;
							}
							break;
					}
					position = destination;
					anim_step = 0;
					status = STATE_FALL;

					grav_tick = 0;
					jump = true;
					onFall();
					return;
				}
			} else {
				status = STATE_SHOOT;
				anim_step = 0;
				anim_tick = 0;

				if (targeted_player != NULL) {
					switch (position) {
						case 0:
							if (targeted_player->x > x && (y < targeted_player->y) && (y > targeted_player->y - 50)) {
								type_attack = 1;
							} else {
								type_attack = 0;
							}
							break;

						case 1:
							if (targeted_player->x < x && (y < targeted_player->y) && (y > targeted_player->y - 50)) {
								type_attack = 1;
							} else {
								type_attack = 0;
							}
							break;

						case 2:
							if (targeted_player->x > x && (y < targeted_player->y) && (y > targeted_player->y - 50)) {
								type_attack = 1;
							} else {
								type_attack = 0;
							}
							break;

						case 3:
							if (targeted_player->x < x && (y < targeted_player->y) && (y > targeted_player->y - 50)) {
								type_attack = 1;
							} else {
								type_attack = 0;
							}
							break;
					}
				}

				if (type_attack == 0) {
					sbk_level.play(2);
				} else {
					sbk_level.play(3);
				}

				attack = true;
				onShoot();
				return;
			}
		}

		if (position == 0 || position == 2) {
			dir = FACING_RIGHT;
			pic = pbk_enemies.pic("snork_boss_stand_right");
		} else {
			dir = FACING_LEFT;
			pic = pbk_enemies.anim("snork_boss_intro")[17];
		}
	}

	colFromPic();
}

void EnemySnorkInBlackBoss::onFall() {
	grav_tick += 1;
	grav_tick %= GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

	y += dy;

	if ((dx > 0 && (x + dx) >= snork_in_black_position_x[position]) || (dx < 0 && (x + dx) <= snork_in_black_position_x[position])) {
		dx = 0;
		x = snork_in_black_position_x[position];
	}
	x += dx;

	if ((x > (snork_in_black_position_x[position] - tolerance)) && (x < (snork_in_black_position_x[position] + tolerance)) &&
		(y > (snork_in_black_position_y[position] - tolerance)) && (y < (snork_in_black_position_y[position] + tolerance))) {
		status = STATE_WALK;
		anim_step = 0;
		anim_tick = 0;
		x = snork_in_black_position_x[position];
		y = snork_in_black_position_y[position];
		jump = false;
		onWalk();
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies.anim("snork_boss_jump_right")[animate(anim_snork_in_black_boss_jump_right, 4, 6)];
	} else {
		pic = pbk_enemies.anim("snork_boss_jump_left")[animate(anim_snork_in_black_boss_jump_left, 4, 6)];
	}

	colFromPic();
}

void EnemySnorkInBlackBoss::onDie() {
	col_on = false;
	if (game_flag[0] == 2 && !jump) {
		int destination = 0;

		if (destination == position) {
			game_flag[0] = 3;

			if (position == 0 || position == 2) {
				dir = FACING_RIGHT;
				pic = pbk_enemies.pic("snork_boss_stand_right");
			} else {
				dir = FACING_LEFT;
				pic = pbk_enemies.anim("snork_boss_intro")[17];
			}
		} else {
			switch (position) {
				case 0:
					dir = FACING_RIGHT;
					switch (destination) {
						case 1:
							dx = 4;
							dy = -7;
							break;

						case 2:
							dx = -1;
							dy = -2;
							break;

						case 3:
							dx = 5;
							dy = -3;

							break;
					}
					break;

				case 1:
					dir = FACING_LEFT;
					switch (destination) {
						case 0:
							dx = -6;
							dy = -5;
							break;

						case 2:
							dx = -6;
							dy = -3;
							break;

						case 3:
							dx = -1;
							dy = 3;
							break;
					}
					break;

				case 2:
					dir = FACING_RIGHT;
					switch (destination) {
						case 0:
							dx = 1;
							dy = -7;
							break;

						case 1:
							dx = 5;
							dy = -9;
							break;

						case 3:
							dx = 7;
							dy = -5;
							break;
					}
					break;

				case 3:
					dir = FACING_LEFT;
					switch (destination) {
						case 0:
							dx = -6;
							dy = -7;
							break;

						case 1:
							dx = 1;
							dy = -6;
							break;

						case 2:
							dx = -5;
							dy = -6;
							break;
					}
					break;
			}
			position = destination;
			anim_step = 0;
			jump = true;

			grav_tick = 0;
			return;
		}
	} else if (game_flag[0] == 2 && jump) {
		grav_tick += 1;
		grav_tick %= GRAVITY_DELAY;

		if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

		y += dy;

		if ((dx > 0 && (x + dx) >= snork_in_black_position_x[position]) || (dx < 0 && (x + dx) <= snork_in_black_position_x[position])) {
			dx = 0;
			x = snork_in_black_position_x[position];
		}

		x += dx;

		if ((x > (snork_in_black_position_x[position] - tolerance)) && (x < (snork_in_black_position_x[position] + tolerance)) &&
			(y > (snork_in_black_position_y[position] - tolerance)) && (y < (snork_in_black_position_y[position] + tolerance))) {
			anim_step = 0;
			anim_tick = 0;
			x = snork_in_black_position_x[position];
			y = snork_in_black_position_y[position];
			jump = false;
			return;
		}

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies.anim("snork_boss_jump_right")[animate(anim_snork_in_black_boss_jump_right, 4, 6)];
		} else {
			pic = pbk_enemies.anim("snork_boss_jump_left")[animate(anim_snork_in_black_boss_jump_left, 4, 6)];
		}
	} else if (game_flag[0] == 3) {
		shark = true;
		x_shark += 5;

		if (x_shark < 5800) {
			y_shark++;
		} else if (x_shark > 6004) {
			y_shark--;
		}

		if (x_shark > 6800) {
			game_flag[0] = 4;
		}
	} else if (game_flag[0] == 4) {
		x_shark += 5;
	}
}

void EnemySnorkInBlackBoss::onShoot() {
	if (type_attack == 0) {
		anim_tick++;
		anim_tick %= 6;
		if (anim_tick == 0) {
			anim_step++;
		}

		if (anim_step > 5) {
			status = STATE_NORMAL;
			anim_step = 0;
			anim_tick = 0;
			onWalk();
			return;
		} else if (anim_tick == 1) {
			if (dir == FACING_RIGHT) {
				ShotThorn* shot;
				switch (anim_step) {
					case 0:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[1], 4, -6);
						shot->setDir(dir);
						shot->x = x + 36;
						shot->y = y - 51;

						list_enemy_shots.emplace_back(shot);

						break;

					case 1:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[2], 5, -5);
						shot->setDir(dir);
						shot->x = x + 36;
						shot->y = y - 51;

						list_enemy_shots.emplace_back(shot);

						break;

					case 2:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[3], 6, -4);
						shot->setDir(dir);
						shot->x = x + 52;
						shot->y = y - 16;

						list_enemy_shots.emplace_back(shot);
						break;

					case 3:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[4], 7, 0);
						shot->setDir(dir);
						shot->x = x + 52;
						shot->y = y - 16;

						list_enemy_shots.emplace_back(shot);
						break;

					case 4:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[5], 6, 4);
						shot->setDir(dir);
						shot->x = x + 35;
						shot->y = y + 25;

						list_enemy_shots.emplace_back(shot);
						break;

					case 5:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[6], 5, 5);
						shot->setDir(dir);
						shot->x = x + 35;
						shot->y = y + 25;

						list_enemy_shots.emplace_back(shot);
						break;
				}
			} else {
				ShotThorn* shot;

				switch (anim_step) {
					case 0:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[15], -4, -6);
						shot->setDir(dir);
						shot->x = x - 36;
						shot->y = y - 51;

						list_enemy_shots.emplace_back(shot);

						break;

					case 1:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[14], -5, -5);
						shot->setDir(dir);
						shot->x = x - 36;
						shot->y = y - 51;

						list_enemy_shots.emplace_back(shot);

						break;

					case 2:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[13], -6, -4);
						shot->setDir(dir);
						shot->x = x - 52;
						shot->y = y - 16;

						list_enemy_shots.emplace_back(shot);
						break;

					case 3:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[12], -7, 0);
						shot->setDir(dir);
						shot->x = x - 52;
						shot->y = y - 16;

						list_enemy_shots.emplace_back(shot);
						break;

					case 4:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[11], -6, 4);
						shot->setDir(dir);
						shot->x = x - 35;
						shot->y = y + 25;

						list_enemy_shots.emplace_back(shot);
						break;

					case 5:
						shot = new ShotThorn(pbk_enemies.anim("snork_boss_thorn")[10], -5, 5);
						shot->setDir(dir);
						shot->x = x - 35;
						shot->y = y + 25;

						list_enemy_shots.emplace_back(shot);
						break;
				}
			}
		}

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies.anim("snork_boss_shoot_right")[2 * (anim_step / 2) + anim_tick / 3];
		} else {
			pic = pbk_enemies.anim("snork_boss_shoot_left")[2 * (anim_step / 2) + anim_tick / 3];
		}
	} else {
		anim_tick++;
		anim_tick %= 5;

		if (anim_tick == 0) {
			anim_step++;
		}

		if (anim_step >= 1) {
			if (dir == FACING_RIGHT) {
				ShotThorn* shot = new ShotThorn(pbk_enemies.pic("snork_boss_laser_right"), 14, 0);

				shot->setDir(dir);
				shot->x = x + 77;
				shot->y = y - 2;

				list_enemy_shots.emplace_back(shot);
			} else {
				ShotThorn* shot = new ShotThorn(pbk_enemies.pic("snork_boss_laser_left"), -14, 0);

				shot->setDir(dir);
				shot->x = x - 77;
				shot->y = y - 2;

				list_enemy_shots.emplace_back(shot);
			}

			status = STATE_NORMAL;
			anim_step = 0;
			anim_tick = 0;
			onWalk();
			return;
		} else {
			if (dir == FACING_RIGHT) {
				pic = pbk_enemies.pic("snork_boss_aim_right");
			} else {
				pic = pbk_enemies.pic("snork_boss_aim_left");
			}
		}
	}
	colFromPic();
}

void EnemySnorkInBlackBoss::takeHit(Shot* shot) {
	static const int dx_splatter_snorkbase1[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter_snorkbase1[] = {-15, -15, -15, -15, -25, -15, -15, -15};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter_snorkbase1, dy_splatter_snorkbase1);
}

void EnemySnorkInBlackBoss::render() {
	if (x_shark < 5904) {
		Sprite::render();
	}
	if (shark) {
		if (x_shark < 5904) {
			draw(x_shark, y_shark, pbk_enemies.pic("snork_boss_shark_bite"));
		} else {
			draw(x_shark, y_shark, pbk_enemies.pic("snork_boss_shark_swim"));
		}
	}
}
