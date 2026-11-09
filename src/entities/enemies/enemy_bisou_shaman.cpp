#include "entities/enemies/enemy_bisou_shaman.h"
#include "entities/shots/shot_shaman_ball.h"
#include "entities/shots/shot_shaman_column.h"
#include <cmath>

static const int xteleport[] = {8144,	// Top right
								8082,	// Right
								7653};	// Left

static const int yteleport[] = {296, 130, 188};

#define STATE_RAISE_STAFF 30
#define STATE_LOWER_STAFF 31
#define STATE_TELEPORT 32

EnemyBisouShaman::EnemyBisouShaman() : shoot_step(0), nb_touch(0), n_pos(1) {
	hp = 11000;
	wait_shoot = 250 + rand() % 50;
	treasure = 0;
}

void EnemyBisouShaman::update() {
	game_flag[2] = 1;

	if (game_flag[0] == 1) {
		mbk_level.play(1);
		game_flag[0] = 2;
		game_flag[FLAG_TIMER] = 50;
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
			pic = pbk_enemies.anim("shaman_die_right")[anim_step];
		else
			pic = pbk_enemies.anim("shaman_die_left")[anim_step];
	}
}

void EnemyBisouShaman::onTeleport() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step == 8) {
			int new_pos;

			if (targeted_player->x > offset + 320) {
				new_pos = 2;
			} else {
				if (targeted_player->y < 280)
					new_pos = 0;
				else
					new_pos = 1;
			}

			if (new_pos != n_pos) {
				n_pos = new_pos;
			} else {
				n_pos += rand() % 2 + 1;
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
		if (x < offset + 320) {
			pic = pbk_enemies.anim("shaman_teleport_right")[anim_step];
			dir = FACING_RIGHT;
		} else {
			pic = pbk_enemies.anim("shaman_teleport_left")[anim_step];
			dir = FACING_LEFT;
		}
	} else if (anim_step <= 10) {
		if (anim_tick % 2 == 0) {
			if (x < offset + 320) {
				pic = pbk_enemies.anim("shaman_teleport_right")[7];
				dir = FACING_RIGHT;
			} else {
				pic = pbk_enemies.anim("shaman_teleport_left")[7];
				dir = FACING_LEFT;
			}
		} else {
			pic = NULL;
		}
	} else {
		if (x < offset + 320) {
			pic = pbk_enemies.anim("shaman_teleport_right")[7 - (anim_step - 11)];
			dir = FACING_RIGHT;
		} else {
			pic = pbk_enemies.anim("shaman_teleport_left")[7 - (anim_step - 11)];
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
		pic = pbk_enemies.anim("shaman_staff_right")[anim_step];
	else
		pic = pbk_enemies.anim("shaman_staff_left")[anim_step];

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
			wait_shoot = 100 + rand() % 50;
			shoot_step = 0;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies.anim("shaman_staff_right")[2 - anim_step];
	else
		pic = pbk_enemies.anim("shaman_staff_left")[2 - anim_step];

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
				int ddx;
				int ddy;

				if (targeted_player != NULL) {
					xc = targeted_player->x;
					yc = targeted_player->y;

					ddx = xc - x;
					ddy = yc - (y - 72);

					int dist = int(sqrt(((ddx * ddx) + (ddy * ddy))));

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
					ShotShamanBall* s = new ShotShamanBall();

					s->y = y - 72;

					if (dir == FACING_RIGHT)
						s->x = x + 18;
					else
						s->x = x - 18;

					s->dx = (ddx * 4) / 3;
					s->dy = (ddy * 4) / 3;

					list_enemy_shots.emplace_back(s);
				} else {
					ShotShamanColumn* s = new ShotShamanColumn();

					s->y = y - 72;

					if (dir == FACING_RIGHT)
						s->x = x + 18;
					else
						s->x = x - 18;

					s->setDir(dir);

					list_enemy_shots.emplace_back(s);
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
			draw(x + 18, y - 70, pbk_enemies.anim("shaman_glow_right")[anim_step]);
		else
			draw(x - 18, y - 70, pbk_enemies.anim("shaman_glow_left")[anim_step]);
	}
}

void EnemyBisouShaman::onWalk() {
	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (x < offset + 320) {
		pic = pbk_enemies.anim("shaman_teleport_right")[0];
		dir = FACING_RIGHT;
	} else {
		pic = pbk_enemies.anim("shaman_teleport_left")[0];
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
		sbk_level.play(16);
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
		pic = pbk_enemies.anim("shaman_charred_right")[anim_step];
	else
		pic = pbk_enemies.anim("shaman_charred_left")[anim_step];
}

void EnemyBisouShaman::takeHit(Shot* shot) {
	static const int dx_splatter[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_splatter[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	splatter(shot, dx_splatter, dy_splatter);

	nb_touch++;

	if (status == STATE_DYING || status == STATE_CHARRED) {
		game_flag[0] = 7;
		game_flag[FLAG_GEN_OFF] = 1;
		sbk_level.play(19);
	}
}
