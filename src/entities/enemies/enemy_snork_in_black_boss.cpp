#include "entities/enemies/enemy_snork_in_black_boss.h"
#include "entities/shots/shot_thorn.h"
#include "core/debug.h"

const int anim_snork_in_black_boss_saute_droite[] = {414, 415, 416, 415};
const int anim_snork_in_black_boss_saute_gauche[] = {417, 418, 419, 418};

const int snork_in_black_position_x[] = {5904, 6351, 5831, 6319};
const int snork_in_black_position_y[] = {175, 189, 342, 328};

EnemySnorkInBlackBoss::EnemySnorkInBlackBoss() : position(1), type_attack(0), attack(false), x_shark(5604), y_shark(100), shark(false), jump(false) {
	hp = 15000;
	dy = 0;
	dir = FACING_LEFT;
	pic = pbk_enemies[380];
}

void EnemySnorkInBlackBoss::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case ETAT_AVANCE:
		case STATE_NORMAL:
			onAvance();
			break;

		case ETAT_TOMBE:
		case ETAT_SAUTE:
			onTombe();
			break;

		case ETAT_CARBONISE:
		case ETAT_MEURE:
			onMeure();
			break;

		case ETAT_TIRE:
			onTire();
			break;
	}

	if (x > offset + 1000) destroyed = true;
}

void EnemySnorkInBlackBoss::onAvance() {
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
			pic = pbk_enemies[383 + anim_step];
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
					status = ETAT_TOMBE;

					lat_grav = 0;
					jump = true;
					onTombe();
					return;
				}
			} else {
				status = ETAT_TIRE;
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
				onTire();
				return;
			}
		}

		if (position == 0 || position == 2) {
			dir = FACING_RIGHT;
			pic = pbk_enemies[401];
		} else {
			dir = FACING_LEFT;
			pic = pbk_enemies[400];
		}
	}

	colFromPic();
}

void EnemySnorkInBlackBoss::onTombe() {
	lat_grav += 1;
	lat_grav %= GRAVITY_DELAY;

	if (lat_grav == 0 && dy < GRAVITY_MAX) dy += 1;

	y += dy;

	if ((dx > 0 && (x + dx) >= snork_in_black_position_x[position]) || (dx < 0 && (x + dx) <= snork_in_black_position_x[position])) {
		dx = 0;
		x = snork_in_black_position_x[position];
	}
	x += dx;

	if ((x > (snork_in_black_position_x[position] - tolerance)) && (x < (snork_in_black_position_x[position] + tolerance)) &&
		(y > (snork_in_black_position_y[position] - tolerance)) && (y < (snork_in_black_position_y[position] + tolerance))) {
		status = ETAT_AVANCE;
		anim_step = 0;
		anim_tick = 0;
		x = snork_in_black_position_x[position];
		y = snork_in_black_position_y[position];
		jump = false;
		onAvance();
		return;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[animate(anim_snork_in_black_boss_saute_droite, 4, 6)];
	} else {
		pic = pbk_enemies[animate(anim_snork_in_black_boss_saute_gauche, 4, 6)];
	}

	colFromPic();
}

void EnemySnorkInBlackBoss::onMeure() {
	col_on = false;
	if (game_flag[0] == 2 && !jump) {
		int destination = 0;

		if (destination == position) {
			game_flag[0] = 3;

			if (position == 0 || position == 2) {
				dir = FACING_RIGHT;
				pic = pbk_enemies[401];
			} else {
				dir = FACING_LEFT;
				pic = pbk_enemies[400];
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

			lat_grav = 0;
			return;
		}
	} else if (game_flag[0] == 2 && jump) {
		lat_grav += 1;
		lat_grav %= GRAVITY_DELAY;

		if (lat_grav == 0 && dy < GRAVITY_MAX) dy += 1;

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
			pic = pbk_enemies[animate(anim_snork_in_black_boss_saute_droite, 4, 6)];
		} else {
			pic = pbk_enemies[animate(anim_snork_in_black_boss_saute_gauche, 4, 6)];
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

void EnemySnorkInBlackBoss::onTire() {
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
			onAvance();
			return;
		} else if (anim_tick == 1) {
			if (dir == FACING_RIGHT) {
				ShotThorn* shot;
				switch (anim_step) {
					case 0:
						shot = new ShotThorn(425, 4, -6);
						shot->setDir(dir);
						shot->x = x + 36;
						shot->y = y - 51;

						list_enemy_shots.emplace_back(shot);

						break;

					case 1:
						shot = new ShotThorn(426, 5, -5);
						shot->setDir(dir);
						shot->x = x + 36;
						shot->y = y - 51;

						list_enemy_shots.emplace_back(shot);

						break;

					case 2:
						shot = new ShotThorn(427, 6, -4);
						shot->setDir(dir);
						shot->x = x + 52;
						shot->y = y - 16;

						list_enemy_shots.emplace_back(shot);
						break;

					case 3:
						shot = new ShotThorn(428, 7, 0);
						shot->setDir(dir);
						shot->x = x + 52;
						shot->y = y - 16;

						list_enemy_shots.emplace_back(shot);
						break;

					case 4:
						shot = new ShotThorn(429, 6, 4);
						shot->setDir(dir);
						shot->x = x + 35;
						shot->y = y + 25;

						list_enemy_shots.emplace_back(shot);
						break;

					case 5:
						shot = new ShotThorn(430, 5, 5);
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
						shot = new ShotThorn(439, -4, -6);
						shot->setDir(dir);
						shot->x = x - 36;
						shot->y = y - 51;

						list_enemy_shots.emplace_back(shot);

						break;

					case 1:
						shot = new ShotThorn(438, -5, -5);
						shot->setDir(dir);
						shot->x = x - 36;
						shot->y = y - 51;

						list_enemy_shots.emplace_back(shot);

						break;

					case 2:
						shot = new ShotThorn(437, -6, -4);
						shot->setDir(dir);
						shot->x = x - 52;
						shot->y = y - 16;

						list_enemy_shots.emplace_back(shot);
						break;

					case 3:
						shot = new ShotThorn(436, -7, 0);
						shot->setDir(dir);
						shot->x = x - 52;
						shot->y = y - 16;

						list_enemy_shots.emplace_back(shot);
						break;

					case 4:
						shot = new ShotThorn(435, -6, 4);
						shot->setDir(dir);
						shot->x = x - 35;
						shot->y = y + 25;

						list_enemy_shots.emplace_back(shot);
						break;

					case 5:
						shot = new ShotThorn(434, -5, 5);
						shot->setDir(dir);
						shot->x = x - 35;
						shot->y = y + 25;

						list_enemy_shots.emplace_back(shot);
						break;
				}
			}
		}

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[402 + 2 * (anim_step / 2) + anim_tick / 3];
		} else {
			pic = pbk_enemies[408 + 2 * (anim_step / 2) + anim_tick / 3];
		}
	} else {
		anim_tick++;
		anim_tick %= 5;

		if (anim_tick == 0) {
			anim_step++;
		}

		if (anim_step >= 1) {
			if (dir == FACING_RIGHT) {
				ShotThorn* shot = new ShotThorn(422, 14, 0);

				shot->setDir(dir);
				shot->x = x + 77;
				shot->y = y - 2;

				list_enemy_shots.emplace_back(shot);
			} else {
				ShotThorn* shot = new ShotThorn(423, -14, 0);

				shot->setDir(dir);
				shot->x = x - 77;
				shot->y = y - 2;

				list_enemy_shots.emplace_back(shot);
			}

			status = STATE_NORMAL;
			anim_step = 0;
			anim_tick = 0;
			onAvance();
			return;
		} else {
			if (dir == FACING_RIGHT) {
				pic = pbk_enemies[421];
			} else {
				pic = pbk_enemies[420];
			}
		}
	}
	colFromPic();
}

void EnemySnorkInBlackBoss::takeHit(Shot* shot) {
	static const int dx_giclure_snorkbase1[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_giclure_snorkbase1[] = {-15, -15, -15, -15, -25, -15, -15, -15};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure_snorkbase1, dy_giclure_snorkbase1);
}

void EnemySnorkInBlackBoss::render() {
	if (x_shark < 5904) {
		Sprite::render();
	}
	if (shark) {
		if (x_shark < 5904) {
			draw(x_shark, y_shark, pbk_enemies[441]);
		} else {
			draw(x_shark, y_shark, pbk_enemies[440]);
		}
	}
}
