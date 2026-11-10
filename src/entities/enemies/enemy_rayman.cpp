#include "entities/enemies/enemy_rayman.h"
#include "entities/shots/shot_rayman_fist.h"
#include "entities/piece_rayman.h"
#include "entities/enemies/enemy_pokemon_hero.h"
#include "entities/enemies/enemy_com_hero.h"
#include "entities/piece_rayman_head.h"

static const PokemonHeroSpec PIKACHU{
	.name = "pikachu", .hp = 1500, .throw_dx = -2, .throw_dy = -8, .treasure = 100, .land_sound = 15, .speed = 3, .die_frames = 10, .leaves_body = true};
static const PokemonHeroSpec BULBASAUR{
	.name = "bulbasaur", .hp = 1500, .throw_dx = -2, .throw_dy = -8, .treasure = 5, .land_sound = -1, .speed = 2, .die_frames = 8, .leaves_body = true};
static const PokemonHeroSpec EMBER{
	.name = "ember", .hp = 1000, .throw_dx = -1, .throw_dy = -7, .treasure = 5, .land_sound = -1, .speed = 2, .die_frames = 14, .leaves_body = false};

EnemyRayman::EnemyRayman() : attack_delay(100 + rand() % 250), wait_for_attack(0), fist1(1), fist2(1), feet(1), dorkemon(4), fist_shot(NULL) {
	hp = 15000;
	xmin = 10428;
	treasure = 0;
}

void EnemyRayman::update() {
	if (blood > 0) blood -= 1;

	if (x < 10930 && game_flag[3] == 0) {
		game_flag[3] = 1;
	}

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			if (game_flag[0] > 0) {
				onWalk();
			}
			break;

		case STATE_CHARRED:
		case STATE_DYING:
			onDie();
			break;

		case STATE_JUMP:
		case STATE_FALL:
			onFall();
			break;

		case STATE_SHOOT:
			onShoot();
			break;
	}

	updateDestroyed();
}

void EnemyRayman::onWalk() {
	if ((game_flag[1] <= dorkemon) && (game_flag[0] == 2) && (game_flag[1] > 0)) {
		if (game_flag[1] == 4) {
			EnemyPokemonHero* bulbasaur = new EnemyPokemonHero(BULBASAUR);

			bulbasaur->x = x - 10;
			bulbasaur->y = y - 25;
			bulbasaur->dir = FACING_LEFT;
			list_enemies.emplace_back(bulbasaur);

			nb_enemies_created++;

			mbk_level.play(1);
			game_flag[FLAG_TIMER] = 70;

			dorkemon--;
		} else if (game_flag[1] == 3) {
			EnemyPokemonHero* ember = new EnemyPokemonHero(EMBER);

			ember->x = x - 10;
			ember->y = y - 25;
			ember->dir = FACING_LEFT;
			list_enemies.emplace_back(ember);

			nb_enemies_created++;

			EnemyCOMHero* Com = new EnemyCOMHero();

			Com->x = x - 10;
			Com->y = y - 25;
			Com->dir = FACING_LEFT;
			list_enemies.emplace_back(Com);

			dorkemon -= 2;

		} else if (game_flag[1] == 1) {
			EnemyPokemonHero* pika = new EnemyPokemonHero(PIKACHU);

			nb_enemies_created++;

			pika->x = x - 10;
			pika->y = y - 25;
			pika->dir = FACING_LEFT;
			list_enemies.emplace_back(pika);
			dorkemon--;
		}
	} else if ((game_flag[0] <= 2) && (game_flag[1] == 0)) {
		if (game_flag[0] == 2) {
			if ((hp > HP_SUBCRITICAL) || ((hp > HP_CRITICAL) && (fist1) && ((x - offset < 80) || (x - offset > 600)))) {
				if (platform_y(x, y) == 0) {
					status = STATE_FALL;
					dy = 0;
					grav_tick = 0;
					onFall();
				}

				if (x - RAYMAN_SPEED < xmin || solid_wall(x - RAYMAN_SPEED, y)) {
					dir = FACING_RIGHT;
				} else if (x + RAYMAN_SPEED > offset + 600 || solid_wall(x + RAYMAN_SPEED, y)) {
					dir = FACING_LEFT;
				}
				wait_for_attack++;
				if ((wait_for_attack >= attack_delay) && (targeted_player != NULL)) {
					if ((platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y)) &&
						(((dir == FACING_RIGHT) && (x < targeted_player->x)) || ((dir == FACING_LEFT) && (x > targeted_player->x)))) {
						x_target = targeted_player->x;
						dy = 0;
						status = STATE_SHOOT;
						fist_range = 0;
						anim_step = 0;
						anim_tick = 0;
						attack_step = 0;
						onShoot();
						return;
					}
				}
				anim_tick++;
				anim_tick %= 3;
				if (anim_tick == 0) {
					anim_step++;
					anim_step %= 21;
				}
				if (dir == FACING_RIGHT) {
					walk(RAYMAN_SPEED);
					pic = pbk_enemies.anim("rayman_walk_right")[anim_step];
				} else {
					walk(-RAYMAN_SPEED);
					pic = pbk_enemies.anim("rayman_walk_left")[anim_step];
				}
			} else if ((hp > HP_CRITICAL) && (x - offset >= 80) && (x - offset <= 600)) {
				if (fist1) {
					Sprite* s;
					if (x - offset > 320) {
						s = new PieceRayman(pbk_enemies.pic("rayman_fist_right"), 1, -1 - rand() % 3, -3 - rand() % 3);
					} else {
						s = new PieceRayman(pbk_enemies.pic("rayman_fist_right"), 1, 1 + rand() % 3, -3 - rand() % 3);
					}

					s->dir = dir;
					s->y = y - 22;
					s->x = x;

					list_splatters.emplace_back(s);

					game_flag[0] = 1;
					fist1 = 0;
				}
				wait_for_attack++;
				if ((wait_for_attack >= attack_delay) && (targeted_player != NULL)) {
					if (platform_y2(targeted_player->x, targeted_player->y) == platform_y2(x, y))

					{
						if (x < targeted_player->x) {
							dir = FACING_RIGHT;

						} else {
							dir = FACING_LEFT;
						}
						x_target = targeted_player->x;
						dy = 0;
						status = STATE_SHOOT;
						fist_range = 0;
						anim_step = 0;
						anim_tick = 0;
						attack_step = 0;
						onShoot();
						return;
					}
				}
				if (dir == FACING_RIGHT) {
					pic = pbk_enemies.pic("rayman_stand_right");
				} else {
					pic = pbk_enemies.pic("rayman_stand_left");
				}
			} else if ((hp > HP_SUPERCRITICAL)) {
				if (fist2) {
					Sprite* s;
					if (x - offset > 320) {
						s = new PieceRayman(pbk_enemies.pic("rayman_fist_left"), 2, -1 - rand() % 3, -3 - rand() % 3);
					} else {
						s = new PieceRayman(pbk_enemies.pic("rayman_fist_left"), 2, 1 + rand() % 3, -3 - rand() % 3);
					}

					s->dir = dir;
					s->y = y - 22;
					s->x = x;

					list_splatters.emplace_back(s);
					game_flag[0] = 1;
					fist2 = 0;
				}

				if (platform_y(x, y) == 0) {
					status = STATE_FALL;
					dy = 0;
					grav_tick = 0;
					onFall();
				}

				if (x - RAYMAN_SPEED < xmin || solid_wall(x - RAYMAN_SPEED, y)) {
					dir = FACING_RIGHT;
				} else if (x + RAYMAN_SPEED > offset + 600 || solid_wall(x + RAYMAN_SPEED, y)) {
					dir = FACING_LEFT;
				}

				anim_tick++;
				anim_tick %= 3;
				if (anim_tick == 0) {
					anim_step++;
					anim_step %= 8;
				}
				if (dir == FACING_RIGHT) {
					walk(RAYMAN_CRITICAL_SPEED);
					pic = pbk_enemies.anim("rayman_run_right")[anim_step];
				} else {
					walk(-RAYMAN_CRITICAL_SPEED);
					pic = pbk_enemies.anim("rayman_run_left")[anim_step];
				}

			} else if ((x - offset >= 80) && (x - offset <= 600)) {
				if (feet) {
					int vy = -3 - rand() % 3;

					Sprite* s = new PieceRayman(pbk_enemies.pic("rayman_foot_right"), 3, 1 + rand() % 3, vy);

					s->dir = dir;
					s->y = y - 22;
					s->x = x + 10;

					list_splatters.emplace_back(s);

					s = new PieceRayman(pbk_enemies.pic("rayman_foot_left"), 3, -1 - rand() % 3, vy);

					s->dir = dir;
					s->y = y - 22;
					s->x = x - 10;

					list_splatters.emplace_back(s);

					game_flag[0] = 1;
					feet = 0;
				}

				if (dir == FACING_RIGHT) {
					pic = pbk_enemies.pic("rayman_stand_footless_right");
				} else {
					pic = pbk_enemies.pic("rayman_stand_footless_left");
				}
			}
		}
		colFromPic();
	} else if (x - offset > 445) {
		anim_tick++;
		anim_tick %= 3;
		if (anim_tick == 0) {
			anim_step++;
			anim_step %= 21;
		}
		if (dir == FACING_RIGHT) {
			walk(RAYMAN_SPEED);
			pic = pbk_enemies.anim("rayman_walk_right")[anim_step];
		} else {
			walk(-RAYMAN_SPEED);
			pic = pbk_enemies.anim("rayman_walk_left")[anim_step];
		}

		if (game_flag[3] == 1) {
			col_on = false;
		}
	}
}

void EnemyRayman::onDie() {
	anim_tick++;
	anim_tick %= 6;

	if ((anim_step == 0) && (anim_tick == 1)) {
		Sprite* head;
		if (x - offset > 320) {
			head = new PieceRaymanHead(-1 - rand() % 3, -3 - rand() % 3);
		} else {
			head = new PieceRaymanHead(1 + rand() % 3, -3 - rand() % 3);
		}
		head->x = x;
		head->y = y - 60;
		list_splatters.emplace_back(head);
		game_flag[3] = 2;
	}

	if (anim_tick == 0) {
		anim_step++;
		if (anim_step >= 6) {
			destroyed = true;
		} else {
			if ((anim_step < 2) && (dir == FACING_RIGHT)) {
				pic = pbk_enemies.anim("rayman_die")[6 + anim_step];
			} else {
				pic = pbk_enemies.anim("rayman_die")[anim_step];
			}
		}
	}
}

void EnemyRayman::onShoot() {
	if (hp > HP_SUBCRITICAL) {
		if (attack_step == 0) {
			anim_tick++;
			anim_tick %= 3;
			if (anim_tick == 0) {
				anim_step++;
				anim_step %= 6;
				fist_range += 30;
				if ((dir == FACING_RIGHT) && (x_target - x < fist_range)) {
					fist_shot = new ShotRaymanFist(RAYMAN_FIST1_SPEED, x_target);
					fist_shot->setDir(dir);
					fist_shot->x = x + 13;
					fist_shot->y = y - 13;
					list_enemy_shots.emplace_back(fist_shot);

					attack_step = 1;
					anim_tick = 0;
					anim_step = 6;
				} else if ((dir == FACING_LEFT) && (x - x_target < fist_range)) {
					fist_shot = new ShotRaymanFist(-RAYMAN_FIST1_SPEED, x_target);
					fist_shot->setDir(dir);
					fist_shot->x = x - 13;
					fist_shot->y = y - 13;
					list_enemy_shots.emplace_back(fist_shot);

					attack_step = 1;
					anim_tick = 0;
					anim_step = 6;
				}
			}

		} else if (attack_step == 1) {
			anim_tick++;
			if (anim_tick == 20) {
				anim_step = 7;
			}

			if ((dir == FACING_RIGHT) && (fist_shot->x < x)) {
				status = STATE_WALK;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				fist_shot->destroyed = true;
				attack_delay = 50 + rand() % 150;
				onWalk();
				return;
			} else if ((dir == FACING_LEFT) && (fist_shot->x > x)) {
				status = STATE_WALK;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				fist_shot->destroyed = true;
				attack_delay = 50 + rand() % 150;
				onWalk();
				return;
			}
		}

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies.anim("rayman_punch_right")[anim_step];
		} else {
			pic = pbk_enemies.anim("rayman_punch_left")[anim_step];
		}
	} else {
		if (attack_step == 0) {
			anim_tick++;
			anim_tick %= 3;
			if (anim_tick == 0) {
				anim_step++;
				anim_step %= 7;
				fist_range += 30;
				if ((dir == FACING_RIGHT) && (x_target - x < fist_range)) {
					fist_shot = new ShotRaymanFist(RAYMAN_FIST2_SPEED, x_target);
					fist_shot->setDir(dir);
					fist_shot->x = x + 13;
					fist_shot->y = y - 13;
					list_enemy_shots.emplace_back(fist_shot);

					attack_step = 1;
					anim_tick = 0;
					anim_step = 7;
				} else if ((dir == FACING_LEFT) && (x - x_target < fist_range)) {
					fist_shot = new ShotRaymanFist(-RAYMAN_FIST2_SPEED, x_target);
					fist_shot->setDir(dir);
					fist_shot->x = x - 13;
					fist_shot->y = y - 13;
					list_enemy_shots.emplace_back(fist_shot);

					attack_step = 1;
					anim_tick = 0;
					anim_step = 7;
				}
			}

		} else if (attack_step == 1) {
			anim_tick++;
			if (anim_tick == 20) {
				anim_step = 8;
			}

			if ((dir == FACING_RIGHT) && (fist_shot->x < x)) {
				status = STATE_WALK;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				fist_shot->destroyed = true;
				attack_delay = 10 + rand() % 50;
				onWalk();
				return;
			} else if ((dir == FACING_LEFT) && (fist_shot->x > x)) {
				status = STATE_WALK;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				fist_shot->destroyed = true;
				attack_delay = 10 + rand() % 50;
				onWalk();
				return;
			}
		}

		if (dir == FACING_RIGHT) {
			pic = pbk_enemies.anim("rayman_punch2_right")[anim_step];
		} else {
			pic = pbk_enemies.anim("rayman_punch2_left")[anim_step];
		}
	}

	colFromPic();
}

void EnemyRayman::onFall() {
	int yp;

	fall();

	if (dy > 0 && (yp = platform_y(x, y + dy)) != 0) {
		status = STATE_NORMAL;
		dy = 0;
		y = yp;
		onWalk();
		return;
	}

	colFromPic();
}

void EnemyRayman::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 0, 4, 0, 0, 0, -4, 0};
	static const int dy_splatter[] = {-15, -15, -15, -15, -15, -15, -15, -15};

	if (game_flag[1] == 0) {
		Enemy::takeHit(shot);
		splatter(shot, dx_splatter, dy_splatter);
	}
}
