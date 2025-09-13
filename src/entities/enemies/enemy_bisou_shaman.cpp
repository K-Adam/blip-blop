#include "entities/enemies/enemy_bisou_shaman.h"
#include "entities/shots/shot_shaman_ball.h"
#include "entities/shots/shot_shaman_column.h"
#include <cmath>

static const int xtel[] = {8144,   // Top right
						   8082,   // Right
						   7653};  // Left

static const int ytel[] = {296, 130, 188};

#define ETAT_LEVEBATON 30
#define ETAT_BAISSEBATON 31
#define ETAT_TELE 32

EnemyBisouShaman::EnemyBisouShaman() : etape_shoot(0), nb_touch(0), n_pos(1) {
	hp = 11000;
	wait_shoot = 250 + rand() % 50;
	tresor = 0;
}

void EnemyBisouShaman::update() {
	game_flag[2] = 1;

	if (game_flag[0] == 1) {
		mbk_level.play(1);
		game_flag[0] = 2;
		game_flag[FLAG_TIMER] = 50;
	}

	if (blood > 0) blood -= 1;

	etape_shoot += 1;

	switch (status) {
		case STATE_NORMAL:
		case ETAT_AVANCE:
			onAvance();
			break;

		case ETAT_LEVEBATON:
			onLeveBaton();
			break;

		case ETAT_TELE:
			onTele();
			break;

		case ETAT_BAISSEBATON:
			onBaisseBaton();
			break;

		case ETAT_TIRE:
			onTire();
			break;

		case ETAT_MEURE:
		case ETAT_CARBONISE:
			onCarbonise();
			break;
	}

	updateDestroyed();
}

void EnemyBisouShaman::onMeure() {
	fall();

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step > 12) {
		engrave(x, y, pic);
		destroyed = true;
	} else {
		if (dir == FACING_RIGHT)
			pic = pbk_enemies[332 + anim_step];
		else
			pic = pbk_enemies[345 + anim_step];
	}
}

void EnemyBisouShaman::onTele() {
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

			x = xtel[n_pos];
			y = ytel[n_pos];
		} else if (anim_step == 19) {
			status = STATE_NORMAL;
			nb_touch = 0;
			return;
		}
	}

	if (anim_step <= 7) {
		if (x < offset + 320) {
			pic = pbk_enemies[316 + anim_step];
			dir = FACING_RIGHT;
		} else {
			pic = pbk_enemies[324 + anim_step];
			dir = FACING_LEFT;
		}
	} else if (anim_step <= 10) {
		if (anim_tick % 2 == 0) {
			if (x < offset + 320) {
				pic = pbk_enemies[323];
				dir = FACING_RIGHT;
			} else {
				pic = pbk_enemies[331];
				dir = FACING_LEFT;
			}
		} else {
			pic = NULL;
		}
	} else {
		if (x < offset + 320) {
			pic = pbk_enemies[323 - (anim_step - 11)];
			dir = FACING_RIGHT;
		} else {
			pic = pbk_enemies[331 - (anim_step - 11)];
			dir = FACING_LEFT;
		}
	}

	noCol();
}

void EnemyBisouShaman::onLeveBaton() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 2) {
			anim_step = anim_tick = 0;
			status = ETAT_TIRE;
			nb_lueurs = 0;
			onTire();
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[358 + anim_step];
	else
		pic = pbk_enemies[361 + anim_step];

	colFromPic();
}

void EnemyBisouShaman::onBaisseBaton() {
	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) {
		anim_step += 1;

		if (anim_step > 2) {
			anim_step = anim_tick = 0;
			status = ETAT_AVANCE;
			wait_shoot = 100 + rand() % 50;
			etape_shoot = 0;
			return;
		}
	}

	if (dir == FACING_RIGHT)
		pic = pbk_enemies[360 - anim_step];
	else
		pic = pbk_enemies[363 - anim_step];

	colFromPic();
}

void EnemyBisouShaman::onTire() {
	anim_tick += 1;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 11;

		if (anim_step == 0) {
			nb_lueurs += 1;

			if (nb_lueurs >= 3) {
				bool boule_feu = false;
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

					int pente;
					if (ddy == 0)
						pente = 10;
					else
						pente = ddx / ddy;

					if (pente < -1 || pente > 1) boule_feu = true;
				}

				if (boule_feu) {
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
				status = ETAT_BAISSEBATON;
			}
		}
	}

	colFromPic();
}

void EnemyBisouShaman::render() {
	Sprite::render();

	if (status == ETAT_TIRE) {
		if (dir == FACING_RIGHT)
			draw(x + 18, y - 70, pbk_enemies[405 + anim_step]);
		else
			draw(x - 18, y - 70, pbk_enemies[416 + anim_step]);
	}
}

void EnemyBisouShaman::onAvance() {
	static const int SPEED = 1;

	fall();

	anim_tick += 1;
	anim_tick %= 6;

	if (anim_tick == 0) {
		anim_step += 1;
		anim_step %= 8;
	}

	if (x < offset + 320) {
		pic = pbk_enemies[316];
		dir = FACING_RIGHT;
	} else {
		pic = pbk_enemies[324];
		dir = FACING_LEFT;
	}

	if (etape_shoot > wait_shoot && anim_step == 0) {
		status = ETAT_LEVEBATON;
		anim_tick = 0;
		onLeveBaton();
	}

	if (nb_touch >= 10) {
		anim_step = anim_tick = 0;
		status = ETAT_TELE;
		sbk_level.play(16);
	}

	colFromPic();
}

void EnemyBisouShaman::onCarbonise() {
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
		pic = pbk_enemies[573 + anim_step];
	else
		pic = pbk_enemies[581 + anim_step];
}

void EnemyBisouShaman::takeHit(Shot* shot) {
	static const int dx_giclure[] = {0, 7, 10, 7, 0, -7, -10, -7};
	static const int dy_giclure[] = {-15, -25, -25, -25, -35, -25, -25, -25};

	Enemy::takeHit(shot);
	gicle(shot, dx_giclure, dy_giclure);

	nb_touch++;

	if (status == ETAT_MEURE || status == ETAT_CARBONISE) {
		game_flag[0] = 7;
		game_flag[FLAG_GEN_OFF] = 1;
		sbk_level.play(19);
	}
}
