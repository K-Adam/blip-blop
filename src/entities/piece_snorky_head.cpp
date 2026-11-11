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
		std::unique_ptr<Sprite> s;
		if (dir == FACING_RIGHT) {
			s = std::make_unique<PieceSnorkyBaseTrunk>(1 + rnd() % 2, -1 + rnd() % 2);
			s->x = x + 19;
		} else {
			s = std::make_unique<PieceSnorkyBaseTrunk>(-1 - rnd() % 2, -1 - rnd() % 2);
			s->x = x - 19;
		}

		s->dir = dir;
		s->y = y - 26;

		level().entities.splatters.emplace_back(std::move(s));
	}

	if (anim_step == 3 && anim_tick == 0) {
		std::unique_ptr<Sprite> s;
		if (dir == FACING_RIGHT) {
			s = std::make_unique<PieceSnorkyBaseHeadBackPart>(1, 0);
			s->x = x + 0;
		} else {
			s = std::make_unique<PieceSnorkyBaseHeadBackPart>(-1, 0);
			s->x = x - 0;
		}

		s->dir = dir;
		s->y = y + 10;

		level().entities.splatters.emplace_back(std::move(s));
	}

	if (anim_step == 6) {
		engrave(x, y, pic);
		destroyed = true;
	} else {
		fall();
		if (dir == FACING_RIGHT) {
			x += 1;
			pic = assets().pics.enemies.anim("snorky_head_right")[anim_step];
			if (anim_step >= 3) {
				fall();
				x += 1;
			}
			if ((level().terrain.platform_y(x, y) != 0) && (x > 0)) {
				x -= 6 - anim_step;
			}
		} else {
			x -= 1;
			pic = assets().pics.enemies.anim("snorky_head_left")[anim_step];
			if (anim_step >= 3) {
				fall();
				x -= 1;
			}
			if ((level().terrain.platform_y(x, y) != 0) && (x < 0)) {
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
		pic = facing(assets().pics.enemies, "hyporider_rider_fall")[anim_step];
	} else if (level().terrain.platform_y(x, y) != 0) {
		if (anim_tick == 0) {
			anim_step++;
		}
		if (anim_step < 5) {
			pic = facing(assets().pics.enemies, "hyporider_rider_fall")[anim_step];
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

	pic = facing(assets().pics.enemies, "snorky_head_back")[anim_step];

	if (anim_step == 3 && (level().terrain.platform_y(x, y) != 0)) {
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

	pic = assets().pics.enemies.anim("snorky_trunk")[anim_step];

	if (level().terrain.platform_y(x, y) != 0) {
		engrave(x, y, pic);
		destroyed = true;
	}
}
