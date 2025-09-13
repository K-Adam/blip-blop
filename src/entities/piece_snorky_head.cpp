#include "entities/piece_snorky_head.h"
#include "entities/enemies/enemy.h"

PieceSnorkyBaseHeadWhole::PieceSnorkyBaseHeadWhole() { dy = 0; }

void PieceSnorkyBaseHeadWhole::update() {
	anim_tick++;
	anim_tick %= 5;
	if (anim_tick == 0) {
		anim_step++;
	}

	if (anim_step == 2 && anim_tick == 0) {
		Sprite* s;
		if (dir == FACING_RIGHT) {
			s = new PieceSnorkyBaseTrunk(1 + rand() % 2, -1 + rand() % 2);
			s->x = x + 19;
		} else {
			s = new PieceSnorkyBaseTrunk(-1 - rand() % 2, -1 - rand() % 2);
			s->x = x - 19;
		}

		s->dir = dir;
		s->y = y - 26;

		list_splatters.emplace_back(s);
	}

	if (anim_step == 3 && anim_tick == 0) {
		Sprite* s;
		if (dir == FACING_RIGHT) {
			s = new PieceSnorkyBaseHeadBackPart(1, 0);
			s->x = x + 0;
		} else {
			s = new PieceSnorkyBaseHeadBackPart(-1, 0);
			s->x = x - 0;
		}

		s->dir = dir;
		s->y = y + 10;

		list_splatters.emplace_back(s);
	}

	if (anim_step == 6) {
		engrave(x, y, pic);
		destroyed = true;
	} else {
		fall();
		if (dir == FACING_RIGHT) {
			x += 1;
			pic = pbk_enemies[52 + anim_step];
			if (anim_step >= 3) {
				fall();
				x += 1;
			}
			if ((platform_y(x, y) != 0) && (x > 0)) {
				x -= 6 - anim_step;
			}
		} else {
			x -= 1;
			pic = pbk_enemies[85 + anim_step];
			if (anim_step >= 3) {
				fall();
				x -= 1;
			}
			if ((platform_y(x, y) != 0) && (x < 0)) {
				x += 6 - anim_step;
			}
		}
	}
}

PieceSnorkyHyporiderBody::PieceSnorkyHyporiderBody(int vx) {
	dx = vx;
	dy = -1;
}

void PieceSnorkyHyporiderBody::update() {
	fall();
	x += dx;
	anim_tick++;
	anim_tick %= 6;
	if ((anim_tick == 0) && (anim_step < 3)) {
		anim_step++;
	}

	if (anim_step < 3) {
		if (dir == FACING_RIGHT) {
			pic = pbk_enemies[307 + anim_step];
		} else {
			pic = pbk_enemies[320 + anim_step];
		}
	} else if (platform_y(x, y) != 0) {
		if (anim_tick == 0) {
			anim_step++;
		}
		if (anim_step < 5) {
			if (dir == FACING_RIGHT) {
				pic = pbk_enemies[307 + anim_step];
			} else {
				pic = pbk_enemies[320 + anim_step];
			}
		} else {
			engrave(x, y, pic);
			destroyed = true;
		}
	}
}

PieceSnorkyBaseHeadBackPart::PieceSnorkyBaseHeadBackPart(int vx, int vy) {
	dx = vx;
	dy = vy;
}

void PieceSnorkyBaseHeadBackPart::update() {
	fall();
	x += dx;
	anim_tick++;
	anim_tick %= 6;
	if ((anim_tick == 0) && (anim_step < 3)) {
		anim_step++;
	}

	if (dir == FACING_RIGHT) {
		pic = pbk_enemies[59 + anim_step];
	} else {
		pic = pbk_enemies[92 + anim_step];
	}

	if (anim_step == 3 && (platform_y(x, y) != 0)) {
		engrave(x, y, pic);
		destroyed = true;
	}
}

PieceSnorkyBaseTrunk::PieceSnorkyBaseTrunk(int vx, int vy) {
	dx = vx;
	dy = vy;
}

void PieceSnorkyBaseTrunk::update() {
	fall();
	x += dx;
	anim_tick++;
	anim_tick %= 3;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 8;
	}

	pic = pbk_enemies[63 + anim_step];

	if (platform_y(x, y) != 0) {
		engrave(x, y, pic);
		destroyed = true;
	}
}
