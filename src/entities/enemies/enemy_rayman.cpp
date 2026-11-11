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

EnemyRayman::EnemyRayman() : attack_delay(100 + rnd() % 250), wait_for_attack(0), fist1(1), fist2(1), feet(1), dorkemon(4), fist_shot(nullptr) {
	hp = 15000;
	xmin = 10428;
	treasure = 0;
}

void EnemyRayman::update() {
	if (blood > 0) blood -= 1;

	if (x < 10930 && level().rules.flags[3] == 0) {
		level().rules.flags[3] = 1;
	}

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			if (level().rules.flags[0] > 0) {
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
	if ((level().rules.flags[1] <= dorkemon) && (level().rules.flags[0] == 2) && (level().rules.flags[1] > 0)) {
		if (level().rules.flags[1] == 4) {
			auto bulbasaur = std::make_unique<EnemyPokemonHero>(BULBASAUR);

			bulbasaur->x = x - 10;
			bulbasaur->y = y - 25;
			bulbasaur->dir = FACING_LEFT;
			level().entities.enemies.emplace_back(std::move(bulbasaur));

			session().enemies_created++;

			assets().music.level.play(1);
			level().rules.flags[FLAG_TIMER] = 70;

			dorkemon--;
		} else if (level().rules.flags[1] == 3) {
			auto ember = std::make_unique<EnemyPokemonHero>(EMBER);

			ember->x = x - 10;
			ember->y = y - 25;
			ember->dir = FACING_LEFT;
			level().entities.enemies.emplace_back(std::move(ember));

			session().enemies_created++;

			auto Com = std::make_unique<EnemyCOMHero>();

			Com->x = x - 10;
			Com->y = y - 25;
			Com->dir = FACING_LEFT;
			level().entities.enemies.emplace_back(std::move(Com));

			dorkemon -= 2;

		} else if (level().rules.flags[1] == 1) {
			auto pika = std::make_unique<EnemyPokemonHero>(PIKACHU);

			session().enemies_created++;

			pika->x = x - 10;
			pika->y = y - 25;
			pika->dir = FACING_LEFT;
			level().entities.enemies.emplace_back(std::move(pika));
			dorkemon--;
		}
	} else if ((level().rules.flags[0] <= 2) && (level().rules.flags[1] == 0)) {
		if (level().rules.flags[0] == 2) {
			if ((hp > HP_SUBCRITICAL) || ((hp > HP_CRITICAL) && (fist1) && ((x - level().camera.offset < 80) || (x - level().camera.offset > 600)))) {
				if (level().terrain.platform_y(x, y) == 0) {
					status = STATE_FALL;
					dy = 0;
					grav_tick = 0;
					onFall();
				}

				if (x - RAYMAN_SPEED < xmin || level().terrain.solid_wall(x - RAYMAN_SPEED, y)) {
					dir = FACING_RIGHT;
				} else if (x + RAYMAN_SPEED > level().camera.offset + 600 || level().terrain.solid_wall(x + RAYMAN_SPEED, y)) {
					dir = FACING_LEFT;
				}
				wait_for_attack++;
				if ((wait_for_attack >= attack_delay) && (session().targeted_player != nullptr)) {
					if ((level().terrain.platform_y2(session().targeted_player->x, session().targeted_player->y) == level().terrain.platform_y2(x, y)) &&
						(((dir == FACING_RIGHT) && (x < session().targeted_player->x)) || ((dir == FACING_LEFT) && (x > session().targeted_player->x)))) {
						x_target = session().targeted_player->x;
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
					pic = assets().pics.enemies.anim("rayman_walk_right")[anim_step];
				} else {
					walk(-RAYMAN_SPEED);
					pic = assets().pics.enemies.anim("rayman_walk_left")[anim_step];
				}
			} else if ((hp > HP_CRITICAL) && (x - level().camera.offset >= 80) && (x - level().camera.offset <= 600)) {
				if (fist1) {
					std::unique_ptr<Sprite> s;
					if (x - level().camera.offset > 320) {
						s = std::make_unique<PieceRayman>(assets().pics.enemies.pic("rayman_fist_right"), 1, -1 - rnd() % 3, -3 - rnd() % 3);
					} else {
						s = std::make_unique<PieceRayman>(assets().pics.enemies.pic("rayman_fist_right"), 1, 1 + rnd() % 3, -3 - rnd() % 3);
					}

					s->dir = dir;
					s->y = y - 22;
					s->x = x;

					level().entities.splatters.emplace_back(std::move(s));

					level().rules.flags[0] = 1;
					fist1 = 0;
				}
				wait_for_attack++;
				if ((wait_for_attack >= attack_delay) && (session().targeted_player != nullptr)) {
					if (level().terrain.platform_y2(session().targeted_player->x, session().targeted_player->y) == level().terrain.platform_y2(x, y))

					{
						if (x < session().targeted_player->x) {
							dir = FACING_RIGHT;

						} else {
							dir = FACING_LEFT;
						}
						x_target = session().targeted_player->x;
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
				pic = facing(assets().pics.enemies, "rayman_stand")[0];
			} else if ((hp > HP_SUPERCRITICAL)) {
				if (fist2) {
					std::unique_ptr<Sprite> s;
					if (x - level().camera.offset > 320) {
						s = std::make_unique<PieceRayman>(assets().pics.enemies.pic("rayman_fist_left"), 2, -1 - rnd() % 3, -3 - rnd() % 3);
					} else {
						s = std::make_unique<PieceRayman>(assets().pics.enemies.pic("rayman_fist_left"), 2, 1 + rnd() % 3, -3 - rnd() % 3);
					}

					s->dir = dir;
					s->y = y - 22;
					s->x = x;

					level().entities.splatters.emplace_back(std::move(s));
					level().rules.flags[0] = 1;
					fist2 = 0;
				}

				if (level().terrain.platform_y(x, y) == 0) {
					status = STATE_FALL;
					dy = 0;
					grav_tick = 0;
					onFall();
				}

				if (x - RAYMAN_SPEED < xmin || level().terrain.solid_wall(x - RAYMAN_SPEED, y)) {
					dir = FACING_RIGHT;
				} else if (x + RAYMAN_SPEED > level().camera.offset + 600 || level().terrain.solid_wall(x + RAYMAN_SPEED, y)) {
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
					pic = assets().pics.enemies.anim("rayman_run_right")[anim_step];
				} else {
					walk(-RAYMAN_CRITICAL_SPEED);
					pic = assets().pics.enemies.anim("rayman_run_left")[anim_step];
				}

			} else if ((x - level().camera.offset >= 80) && (x - level().camera.offset <= 600)) {
				if (feet) {
					int vy = -3 - rnd() % 3;

					auto s = std::make_unique<PieceRayman>(assets().pics.enemies.pic("rayman_foot_right"), 3, 1 + rnd() % 3, vy);

					s->dir = dir;
					s->y = y - 22;
					s->x = x + 10;

					level().entities.splatters.emplace_back(std::move(s));

					s = std::make_unique<PieceRayman>(assets().pics.enemies.pic("rayman_foot_left"), 3, -1 - rnd() % 3, vy);

					s->dir = dir;
					s->y = y - 22;
					s->x = x - 10;

					level().entities.splatters.emplace_back(std::move(s));

					level().rules.flags[0] = 1;
					feet = 0;
				}

				pic = facing(assets().pics.enemies, "rayman_stand_footless")[0];
			}
		}
		colFromPic();
	} else if (x - level().camera.offset > 445) {
		anim_tick++;
		anim_tick %= 3;
		if (anim_tick == 0) {
			anim_step++;
			anim_step %= 21;
		}
		if (dir == FACING_RIGHT) {
			walk(RAYMAN_SPEED);
			pic = assets().pics.enemies.anim("rayman_walk_right")[anim_step];
		} else {
			walk(-RAYMAN_SPEED);
			pic = assets().pics.enemies.anim("rayman_walk_left")[anim_step];
		}

		if (level().rules.flags[3] == 1) {
			col_on = false;
		}
	}
}

void EnemyRayman::onDie() {
	anim_tick++;
	anim_tick %= 6;

	if ((anim_step == 0) && (anim_tick == 1)) {
		std::unique_ptr<Sprite> head;
		if (x - level().camera.offset > 320) {
			head = std::make_unique<PieceRaymanHead>(-1 - rnd() % 3, -3 - rnd() % 3);
		} else {
			head = std::make_unique<PieceRaymanHead>(1 + rnd() % 3, -3 - rnd() % 3);
		}
		head->x = x;
		head->y = y - 60;
		level().entities.splatters.emplace_back(std::move(head));
		level().rules.flags[3] = 2;
	}

	if (anim_tick == 0) {
		anim_step++;
		if (anim_step >= 6) {
			destroyed = true;
		} else {
			if ((anim_step < 2) && (dir == FACING_RIGHT)) {
				pic = assets().pics.enemies.anim("rayman_die")[6 + anim_step];
			} else {
				pic = assets().pics.enemies.anim("rayman_die")[anim_step];
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
					auto fist = std::make_unique<ShotRaymanFist>(RAYMAN_FIST1_SPEED, x_target);
					fist_shot = fist.get();
					fist_shot->setDir(dir);
					fist_shot->x = x + 13;
					fist_shot->y = y - 13;
					level().entities.enemy_shots.emplace_back(std::move(fist));

					attack_step = 1;
					anim_tick = 0;
					anim_step = 6;
				} else if ((dir == FACING_LEFT) && (x - x_target < fist_range)) {
					auto fist = std::make_unique<ShotRaymanFist>(-RAYMAN_FIST1_SPEED, x_target);
					fist_shot = fist.get();
					fist_shot->setDir(dir);
					fist_shot->x = x - 13;
					fist_shot->y = y - 13;
					level().entities.enemy_shots.emplace_back(std::move(fist));

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
				attack_delay = 50 + rnd() % 150;
				onWalk();
				return;
			} else if ((dir == FACING_LEFT) && (fist_shot->x > x)) {
				status = STATE_WALK;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				fist_shot->destroyed = true;
				attack_delay = 50 + rnd() % 150;
				onWalk();
				return;
			}
		}

		pic = facing(assets().pics.enemies, "rayman_punch")[anim_step];
	} else {
		if (attack_step == 0) {
			anim_tick++;
			anim_tick %= 3;
			if (anim_tick == 0) {
				anim_step++;
				anim_step %= 7;
				fist_range += 30;
				if ((dir == FACING_RIGHT) && (x_target - x < fist_range)) {
					auto fist = std::make_unique<ShotRaymanFist>(RAYMAN_FIST2_SPEED, x_target);
					fist_shot = fist.get();
					fist_shot->setDir(dir);
					fist_shot->x = x + 13;
					fist_shot->y = y - 13;
					level().entities.enemy_shots.emplace_back(std::move(fist));

					attack_step = 1;
					anim_tick = 0;
					anim_step = 7;
				} else if ((dir == FACING_LEFT) && (x - x_target < fist_range)) {
					auto fist = std::make_unique<ShotRaymanFist>(-RAYMAN_FIST2_SPEED, x_target);
					fist_shot = fist.get();
					fist_shot->setDir(dir);
					fist_shot->x = x - 13;
					fist_shot->y = y - 13;
					level().entities.enemy_shots.emplace_back(std::move(fist));

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
				attack_delay = 10 + rnd() % 50;
				onWalk();
				return;
			} else if ((dir == FACING_LEFT) && (fist_shot->x > x)) {
				status = STATE_WALK;
				anim_step = 0;
				anim_tick = 0;
				wait_for_attack = 0;
				fist_shot->destroyed = true;
				attack_delay = 10 + rnd() % 50;
				onWalk();
				return;
			}
		}

		pic = facing(assets().pics.enemies, "rayman_punch2")[anim_step];
	}

	colFromPic();
}

void EnemyRayman::onFall() {
	int yp;

	fall();

	if (dy > 0 && (yp = level().terrain.platform_y(x, y + dy)) != 0) {
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

	if (level().rules.flags[1] == 0) {
		Enemy::takeHit(shot);
		splatter(shot, dx_splatter, dy_splatter);
	}
}
