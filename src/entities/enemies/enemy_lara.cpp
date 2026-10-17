#include "core/debug.h"
#include "entities/enemies/enemy_lara.h"
#include "entities/shots/shot_thorn.h"
#include "entities/enemies/enemy_spikes.h"
#include "entities/enemies/enemy_weight.h"
#include "entities/enemies/enemy_rayman_head.h"
#include <cmath>

const int anim_lara_button[] = {0, 0, 1, 2, 3, 3, 3, 2, 1, 0};
const int anim_lara_draw[] = {4, 4, 4, 5, 5, 6, 7, 7, 8, 8, 8};

EnemyLara::EnemyLara() : bust_pic_num(4), wait_for_attack(0), pics(0), rayman_head(0) {
	hp = 15000;
	pic = pbk_enemies.pic("lara_legs");
}

void EnemyLara::update() {
	if (blood > 0) blood -= 1;

	switch (status) {
		case STATE_NORMAL:
		case STATE_WALK:
		case STATE_JUMP:
		case STATE_FALL:
			onWalk();
			break;

		case STATE_CHARRED:
		case STATE_DYING:
			onDie();
			break;

		case STATE_SHOOT:
			onShoot();
			break;
	}
}

void EnemyLara::onWalk() {
	if (!pics) {
		Enemy* pic;
		int i, j;
		for (i = 2; i >= 0; i--) {
			for (j = 0; j < 6; j++) {
				pic = new EnemySpikes();

				if (i != 1) {
					pic->x = 515 + j * 65 - i * 20;
				} else {
					pic->x = 547 + j * 65 - i * 20;
				}

				switch (i) {
					case 0:
						pic->y = 429;
						break;
					case 1:
						pic->y = 449;
						break;

					case 2:
						pic->y = 471;
						break;
				}

				pic->dir = FACING_LEFT;
				list_enemies.emplace_back(pic);
			}
		}

		pics = true;
	}
	if (game_flag[0] == 1) {
		if (!rayman_head) {
			if (targeted_player != NULL) {
				Enemy* head = new EnemyRaymanHead();
				head->x = targeted_player->x + 10;
				head->y = targeted_player->y - 30;
				head->dy = -8;
				head->dir = FACING_LEFT;
				list_enemies.emplace_back(head);
				rayman_head = true;
			}
		}
	} else if (game_flag[0] == 2) {
		anim_tick++;
		anim_tick %= 6;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step > 9) {
				game_flag[0] = 3;
				anim_step = 0;
				game_flag[FLAG_TIMER] = 180;
			} else {
				bust_pic_num = anim_lara_button[anim_step];
			}
		}
	} else if (game_flag[0] == 3) {
		anim_tick++;
		anim_tick %= 6;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step > 10) {
				game_flag[0] = 4;
				anim_step = 0;
			} else {
				bust_pic_num = anim_lara_draw[anim_step];
			}
		}
	} else if (game_flag[0] == 4) {
		wait_for_attack++;
		if ((wait_for_attack > ATTACK_DELAY) && (targeted_player != NULL)) {
			int dif_y = y - 102 - targeted_player->y;
			int dif_x = x - targeted_player->x;

			if (dif_y != 0) {
				int slope = (dif_x * 10) / dif_y;
				if (slope > 40) {
					int dx = targeted_player->x + 20 - x + 60;
					int dy = targeted_player->y - y + 144;

					int dist = (int)(sqrt((dx * dx) + (dy * dy)));

					dx = (dx * LARA_SHOT_SPEED) / dist;
					dy = (dy * LARA_SHOT_SPEED) / dist;

					ShotThorn* shot;
					shot = new ShotThorn(pbk_enemies.pic("lara_bullet"), dx, dy + rand() % 3 - 1);
					shot->x = x - 60;
					shot->y = y - 144;
					shot->traverse = true;
					list_enemy_shots.emplace_back(shot);

					bust_pic_num = 27;
					anim_tick = 0;
					anim_step = 1;
					status = STATE_SHOOT;
					onShoot();
					return;
				} else if (slope > 30) {
					bust_pic_num = 9;
					int dx = targeted_player->x + 20 - x + 58;
					int dy = targeted_player->y - y + 154;

					int dist = (int)sqrt((dx * dx) + (dy * dy));

					dx = (dx * LARA_SHOT_SPEED) / dist;
					dy = (dy * LARA_SHOT_SPEED) / dist;

					ShotThorn* shot;

					shot = new ShotThorn(pbk_enemies.pic("lara_bullet"), dx, dy + rand() % 3 - 1);

					shot->x = x - 58;
					shot->y = y - 154;
					shot->traverse = true;
					list_enemy_shots.emplace_back(shot);
					anim_tick = 0;
					anim_step = 1;
					status = STATE_SHOOT;
					onShoot();
					return;

				} else if (slope > 20) {
					int dx = targeted_player->x + 20 - x + 57;
					int dy = targeted_player->y - y + 165;

					int dist = (int)sqrt((dx * dx) + (dy * dy));

					dx = (dx * LARA_SHOT_SPEED) / dist;
					dy = (dy * LARA_SHOT_SPEED) / dist;

					ShotThorn* shot;

					shot = new ShotThorn(pbk_enemies.pic("lara_bullet"), dx, dy + rand() % 3 - 1);

					shot->x = x - 57;
					shot->y = y - 165;
					shot->traverse = true;
					list_enemy_shots.emplace_back(shot);
					bust_pic_num = 15;
					anim_tick = 0;
					anim_step = 1;
					status = STATE_SHOOT;
					onShoot();
					return;
				} else if (slope > 5) {
					bust_pic_num = 21;
					int dx = targeted_player->x + 20 - x + 53;
					int dy = targeted_player->y - y + 175;

					int dist = (int)sqrt((dx * dx) + (dy * dy));

					dx = (dx * LARA_SHOT_SPEED) / dist;
					dy = (dy * LARA_SHOT_SPEED) / dist;

					ShotThorn* shot;

					shot = new ShotThorn(pbk_enemies.pic("lara_bullet"), dx, dy + rand() % 3 - 1);
					shot->x = x - 53;
					shot->y = y - 175;
					shot->traverse = true;
					list_enemy_shots.emplace_back(shot);
					anim_tick = 0;
					anim_step = 1;
					status = STATE_SHOOT;
					onShoot();
					return;
				} else if (slope > -15) {
					// Too close to shoot above -15 degrees

					bust_pic_num = 27;
				} else if (slope > -40) {
					int dx = targeted_player->x + 20 - x + 59;
					int dy = targeted_player->y - y + 129;

					int dist = (int)sqrt((dx * dx) + (dy * dy));

					dx = (dx * LARA_SHOT_SPEED) / dist;
					dy = (dy * LARA_SHOT_SPEED) / dist;

					ShotThorn* shot;

					shot = new ShotThorn(pbk_enemies.pic("lara_bullet"), dx, dy + rand() % 3 - 1);

					shot->x = x - 59;
					shot->y = y - 129;
					shot->traverse = true;
					list_enemy_shots.emplace_back(shot);
					// Frame aimed at -10 degrees
					bust_pic_num = 33;

					anim_tick = 0;
					anim_step = 1;
					status = STATE_SHOOT;
					onShoot();
					return;
				} else {
					int dx = targeted_player->x - x + 60;
					int dy = targeted_player->y - y + 144;

					int dist = (int)sqrt((dx * dx) + (dy * dy));

					dx = (dx * LARA_SHOT_SPEED) / dist;
					dy = (dy * LARA_SHOT_SPEED) / dist;

					ShotThorn* shot;

					shot = new ShotThorn(pbk_enemies.pic("lara_bullet"), dx, dy + rand() % 3 - 1);

					shot->x = x - 60;
					shot->y = y - 144;
					shot->traverse = true;
					list_enemy_shots.emplace_back(shot);
					bust_pic_num = 27;
					anim_tick = 0;
					anim_step = 1;
					status = STATE_SHOOT;
					onShoot();
					return;
				}
			} else {
			}
		}
	} else if (game_flag[0] == 7) {
		destroyed = true;
		game_flag[FLAG_NB_KILL]++;
	}
}

void EnemyLara::onShoot() {
	anim_tick++;
	anim_tick %= anim_step;
	if (anim_tick == 0) {
		anim_step++;
		if (anim_step >= 7) {
			status = STATE_WALK;
			anim_tick = 0;
			anim_step = 0;
			bust_pic_num -= 5;
			wait_for_attack = 0;
			onWalk();
			return;
		} else {
			bust_pic_num++;
		}
	}
}

void EnemyLara::onDie() {
	game_flag[0] = 5;
	destroyed = true;
}

void EnemyLara::render() {
	if (game_flag[0] < 6) {
		draw(x, y - 102, pbk_enemies.anim("lara_bust")[bust_pic_num]);
	}

	Sprite::render();
}

void EnemyLara::takeHit(Shot* shot) { Enemy::takeHit(shot); }
