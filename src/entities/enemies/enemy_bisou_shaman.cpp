#include "entities/enemies/enemy_bisou_shaman.h"
#include "entities/shots/shot_shaman_ball.h"
#include "entities/shots/shot_shaman_column.h"
#include <cmath>

static const int xteleport[] = {8144,	// Top right
								8082,	// Right
								7653};	// Left

static const int yteleport[] = {296, 130, 188};

constexpr int STATE_RAISE_STAFF = 30;
constexpr int STATE_LOWER_STAFF = 31;
constexpr int STATE_TELEPORT = 32;

EnemyBisouShaman::EnemyBisouShaman() : shoot_step(0), nb_touch(0), n_pos(1) {
	hp = 11000;
	wait_shoot = 250 + rnd() % 50;
	treasure = 0;
}

void EnemyBisouShaman::update() {
	level().rules.flags[2] = 1;

	if (level().rules.flags[0] == 1) {
		assets().music.level.play(1);
		level().rules.flags[0] = 2;
		level().rules.flags[FLAG_TIMER] = 50;
	}

	if (blood > 0) blood -= 1;

	shoot_step += 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
			onWalk();
			break;

		case STATE_RAISE_STAFF:
			onRaiseStaff();
			break;

		case STATE_TELEPORT:
			onTeleport();
			break;

		case STATE_LOWER_STAFF:
			onLowerStaff();
			break;

		case STATE_SHOOT:
			onShoot();
			break;

		case STATE_DYING:
		case STATE_CHARRED:
			onCharred();
			break;
	}

	updateDestroyed();
}

void EnemyBisouShaman::onDie() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step > 12) {
		engrave(x, y, pic);
		destroyed = true;
	} else {
		if (dir == FACING_RIGHT)
			pic = assets().pics.enemies.anim("shaman_die_right")[anim_step];
		else
			pic = assets().pics.enemies.anim("shaman_die_left")[anim_step];
	}
}

void EnemyBisouShaman::onTeleport() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step == 8) {
			int new_pos;

			if (session().targeted_player->x > level().camera.offset + 320) {
				new_pos = 2;
			} else {
				if (session().targeted_player->y < 280)
					new_pos = 0;
				else
					new_pos = 1;
			}

			if (new_pos != n_pos) {
				n_pos = new_pos;
			} else {
				n_pos += rnd() % 2 + 1;
				n_pos %= 3;
			}

			x = xteleport[n_pos];
			y = yteleport[n_pos];
		} else if (anim_step == 19) {
			status = STATE_NORMAL;
			nb_touch = 0;
			return;
		}
	}

	if (anim_step <= 7) {
		if (x < level().camera.offset + 320) {
			pic = assets().pics.enemies.anim("shaman_teleport_right")[anim_step];
			dir = FACING_RIGHT;
		} else {
			pic = assets().pics.enemies.anim("shaman_teleport_left")[anim_step];
			dir = FACING_LEFT;
		}
	} else if (anim_step <= 10) {
		if (anim_tick % 2 == 0) {
			if (x < level().camera.offset + 320) {
				pic = assets().pics.enemies.anim("shaman_teleport_right")[7];
				dir = FACING_RIGHT;
			} else {
				pic = assets().pics.enemies.anim("shaman_teleport_left")[7];
				dir = FACING_LEFT;
			}
		} else {
			pic = nullptr;
		}
	} else {
		if (x < level().camera.offset + 320) {
			pic = assets().pics.enemies.anim("shaman_teleport_right")[7 - (anim_step - 11)];
			dir = FACING_RIGHT;
		} else {
			pic = assets().pics.enemies.anim("shaman_teleport_left")[7 - (anim_step - 11)];
			dir = FACING_LEFT;
		}
	}

	noCol();
}

void EnemyBisouShaman::onRaiseStaff() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 2) {
			anim_step = anim_tick = 0;
			status = STATE_SHOOT;
			nb_glows = 0;
			onShoot();
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.anim("shaman_staff_right")[anim_step];
	else
		pic = assets().pics.enemies.anim("shaman_staff_left")[anim_step];

	colFromPic();
}

void EnemyBisouShaman::onLowerStaff() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 2) {
			anim_step = anim_tick = 0;
			status = STATE_WALK;
			wait_shoot = 100 + rnd() % 50;
			shoot_step = 0;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.anim("shaman_staff_right")[2 - anim_step];
	else
		pic = assets().pics.enemies.anim("shaman_staff_left")[2 - anim_step];

	colFromPic();
}

void EnemyBisouShaman::onShoot() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 11;

		if (anim_step == 0) {
			nb_glows += 1;

			if (nb_glows >= 3) {
				bool use_fireball = false;
				int xc;
				int yc;
				int ddx = 0;
				int ddy = 0;

				if (session().targeted_player != nullptr) {
					xc = session().targeted_player->x;
					yc = session().targeted_player->y;

					ddx = xc - x;
					ddy = yc - (y - 72);

					int dist = static_cast<int>(sqrt(((ddx * ddx) + (ddy * ddy))));

					ddx = (ddx * 6) / dist;
					ddy = (ddy * 6) / dist;

					int slope;
					if (ddy == 0)
						slope = 10;
					else
						slope = ddx / ddy;

					if (slope < -1 || slope > 1) use_fireball = true;
				}

				if (use_fireball) {
					auto s = std::make_unique<ShotShamanBall>();

					s->y = y - 72;

					if (dir == FACING_RIGHT)
						s->x = x + 18;
					else
						s->x = x - 18;

					s->dx = (ddx * 4) / 3;
					s->dy = (ddy * 4) / 3;

					level().entities.enemy_shots.emplace_back(std::move(s));
				} else {
					auto s = std::make_unique<ShotShamanColumn>();

					s->y = y - 72;

					if (dir == FACING_RIGHT)
						s->x = x + 18;
					else
						s->x = x - 18;

					s->setDir(dir);

					level().entities.enemy_shots.emplace_back(std::move(s));
				}

				anim_step = anim_tick = 0;
				status = STATE_LOWER_STAFF;
			}
		}
	}

	colFromPic();
}

void EnemyBisouShaman::render() {
	Sprite::render();

	if (status == STATE_SHOOT) {
		if (dir == FACING_RIGHT)
			draw(x + 18, y - 70, assets().pics.enemies.anim("shaman_glow_right")[anim_step]);
		else
			draw(x - 18, y - 70, assets().pics.enemies.anim("shaman_glow_left")[anim_step]);
	}
}

void EnemyBisouShaman::onWalk() {
	fall();

	step(6, 8);

	if (x < level().camera.offset + 320) {
		pic = assets().pics.enemies.anim("shaman_teleport_right")[0];
		dir = FACING_RIGHT;
	} else {
		pic = assets().pics.enemies.anim("shaman_teleport_left")[0];
		dir = FACING_LEFT;
	}

	if (shoot_step > wait_shoot && anim_step == 0) {
		status = STATE_RAISE_STAFF;
		anim_tick = 0;
		onRaiseStaff();
	}

	if (nb_touch >= 10) {
		anim_step = anim_tick = 0;
		status = STATE_TELEPORT;
		assets().sounds.level.play(16);
	}

	colFromPic();
}

void EnemyBisouShaman::onCharred() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 7) {
			destroyed = true;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = assets().pics.enemies.anim("shaman_charred_right")[anim_step];
	else
		pic = assets().pics.enemies.anim("shaman_charred_left")[anim_step];
}

void EnemyBisouShaman::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	nb_touch++;

	if (status == STATE_DYING || status == STATE_CHARRED) {
		level().rules.flags[0] = 7;
		level().rules.flags[FLAG_GEN_OFF] = 1;
		assets().sounds.level.play(19);
	}
}
