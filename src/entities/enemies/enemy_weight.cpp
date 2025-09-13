#include "entities/enemies/enemy_weight.h"
#include "entities/gore_splatter.h"

EnemyWeight::EnemyWeight() : nb_collisions(0) {
	hp = 1000;
	pic = pbk_enemies[169];
}

void EnemyWeight::update() {
	if (status == STATE_DYING) {
		onDie();
	} else {
		pic = pbk_enemies[169];
		if (game_flag[0] < 3) {
			y = -100;
			x = 565;
		}
		if ((game_flag[0] == 3) || (game_flag[0] == 4)) {
			if (y < 100) {
				fall2();
			} else {
				dy = 0;
			}
			if (x > 950) {
				status = STATE_DYING;
			}
		}

		if (nb_collisions >= HITS_PER_MOVE) {
			x += 3;
			nb_collisions -= HITS_PER_MOVE;
		}
		colFromPic();
	}
}

void EnemyWeight::onDie() {
	if ((solid_wall(x, y + dy))) {
		game_flag[0] = 7;
		anim_tick++;
		anim_tick %= 15;
		if (anim_tick == 0) {
			anim_step++;
			if (anim_step >= 3) {
				engrave(x, y, pic);
				destroyed = true;
			}
		}
		if (anim_step < 3) {
			pic = pbk_enemies[202 + anim_step];
		}
	} else {
		fall2();
		if (solid_wall(x, y + dy * 10)) {
			pic = pbk_enemies[201];
			int i;
			Sprite* s;
			for (i = 0; i < 8; i++) {
				s = new GoreSplatter(rand() % 15 - 7, -1 - rand() % 10);
				s->x = x + rand() % 80 - 40;
				s->y = y - rand() % 30 - 10;
				list_splatters.emplace_back(s);
			}
		} else if (solid_wall(x, y + dy * 15)) {
			pic = pbk_enemies[200];
			int i;
			Sprite* s;
			for (i = 0; i < 6; i++) {
				s = new GoreSplatter(rand() % 15 - 7, -1 - rand() % 10);
				s->x = x + rand() % 80 - 40;
				s->y = y - rand() % 30 - 10;
				list_splatters.emplace_back(s);
			}
		} else if (solid_wall(x, y + dy * 20)) {
			pic = pbk_enemies[200];
			int i;
			Sprite* s;
			for (i = 0; i < 5; i++) {
				s = new GoreSplatter(rand() % 15 - 7, -1 - rand() % 10);
				s->x = x + rand() % 80 - 40;
				s->y = y - rand() % 30 - 10;
				list_splatters.emplace_back(s);
			}
		} else if (solid_wall(x, y + dy * 25)) {
			game_flag[0] = 6;
			pic = pbk_enemies[199];
			int i;
			Sprite* s;
			for (i = 0; i < 4; i++) {
				s = new GoreSplatter(rand() % 15 - 7, -1 - rand() % 10);
				s->x = x + rand() % 80 - 40;
				s->y = y - rand() % 30 - 10;
				list_splatters.emplace_back(s);
			}
		} else if (solid_wall(x, y + dy * 30)) {
			pic = pbk_enemies[199];
			int i;
			Sprite* s;
			for (i = 0; i < 3; i++) {
				s = new GoreSplatter(rand() % 15 - 7, -1 - rand() % 10);
				s->x = x + rand() % 80 - 40;
				s->y = y - rand() % 30 - 10;
				list_splatters.emplace_back(s);
			}
		} else {
			pic = pbk_enemies[170];
		}
	}
}

int EnemyWeight::damage() { return 0; };

void EnemyWeight::takeHit(Shot* shot) {
	if ((shot->dir >= 2) && (shot->dir <= 6)) {
		nb_collisions++;
	}

	shot->onHit(0);
}
