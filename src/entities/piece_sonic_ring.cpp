#include "entities/piece_sonic_ring.h"

PieceSonicRing::PieceSonicRing(int vx, int vy) : dx(vx) { dy = vy; }

void PieceSonicRing::update() {
	x += dx;
	fall2();

	if (level().terrain.platform_y(x + dx, y + dy) != 0) {
		if (dy > 2) {
			dy = -dy + 1;
		} else if ((dy == 2) || (dy == 1)) {
			dy = -2;
		}
	}

	anim_tick++;
	anim_tick %= 4;

	if (anim_tick == 0) {
		anim_step++;
		anim_step %= 4;
	}

	pic = assets().pics.enemies.anim("sonic_ring")[anim_step];
	updateDestroyed();
}
