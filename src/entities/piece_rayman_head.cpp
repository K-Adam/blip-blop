#include "entities/piece_rayman_head.h"

PieceRaymanHead::PieceRaymanHead(int vx, int vy) : dx(vx) {
	pic = pbk_enemies[496];
	dy = vy;
}

void PieceRaymanHead::update() {
	x += dx;
	fall();

	anim_tick++;
	anim_tick %= 4;
	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 8;
		pic = pbk_enemies[496 + anim_step];
	}

	if (dy > 0 && (platform_y(x, y + dy)) != 0) {
		engrave(x, y, pic);
		destroyed = true;
	}
}
