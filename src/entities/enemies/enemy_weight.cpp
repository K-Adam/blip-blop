#include "entities/enemies/enemy_weight.h"
#include "entities/gore_splatter.h"

EnemyWeight::EnemyWeight() : nb_collisions(0) {
	hp = 1000;
	pic = assets().pics.enemies.pic("weight_hanging");
}

void EnemyWeight::update() {
	if (status == STATE_DYING) {
		onDie();
	} else {
		pic = assets().pics.enemies.pic("weight_hanging");
		if (level().rules.flags[0] < 3) {
			y = -100;
			x = 565;
		}
		if ((level().rules.flags[0] == 3) || (level().rules.flags[0] == 4)) {
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
	if ((level().terrain.solid_wall(x, y + dy))) {
		level().rules.flags[0] = 7;
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
			pic = assets().pics.enemies.anim("weight_crush")[3 + anim_step];
		}
	} else {
		fall2();
		if (level().terrain.solid_wall(x, y + dy * 10)) {
			pic = assets().pics.enemies.anim("weight_crush")[2];
			int i;
			Sprite* s;
			for (i = 0; i < 8; i++) {
				s = new GoreSplatter(rnd() % 15 - 7, -1 - rnd() % 10);
				s->x = x + rnd() % 80 - 40;
				s->y = y - rnd() % 30 - 10;
				level().entities.splatters.emplace_back(s);
			}
		} else if (level().terrain.solid_wall(x, y + dy * 15)) {
			pic = assets().pics.enemies.anim("weight_crush")[1];
			int i;
			Sprite* s;
			for (i = 0; i < 6; i++) {
				s = new GoreSplatter(rnd() % 15 - 7, -1 - rnd() % 10);
				s->x = x + rnd() % 80 - 40;
				s->y = y - rnd() % 30 - 10;
				level().entities.splatters.emplace_back(s);
			}
		} else if (level().terrain.solid_wall(x, y + dy * 20)) {
			pic = assets().pics.enemies.anim("weight_crush")[1];
			int i;
			Sprite* s;
			for (i = 0; i < 5; i++) {
				s = new GoreSplatter(rnd() % 15 - 7, -1 - rnd() % 10);
				s->x = x + rnd() % 80 - 40;
				s->y = y - rnd() % 30 - 10;
				level().entities.splatters.emplace_back(s);
			}
		} else if (level().terrain.solid_wall(x, y + dy * 25)) {
			level().rules.flags[0] = 6;
			pic = assets().pics.enemies.anim("weight_crush")[0];
			int i;
			Sprite* s;
			for (i = 0; i < 4; i++) {
				s = new GoreSplatter(rnd() % 15 - 7, -1 - rnd() % 10);
				s->x = x + rnd() % 80 - 40;
				s->y = y - rnd() % 30 - 10;
				level().entities.splatters.emplace_back(s);
			}
		} else if (level().terrain.solid_wall(x, y + dy * 30)) {
			pic = assets().pics.enemies.anim("weight_crush")[0];
			int i;
			Sprite* s;
			for (i = 0; i < 3; i++) {
				s = new GoreSplatter(rnd() % 15 - 7, -1 - rnd() % 10);
				s->x = x + rnd() % 80 - 40;
				s->y = y - rnd() % 30 - 10;
				level().entities.splatters.emplace_back(s);
			}
		} else {
			pic = assets().pics.enemies.pic("weight_dropped");
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
