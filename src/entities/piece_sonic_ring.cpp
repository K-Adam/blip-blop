#include "entities/piece_sonic_ring.h"

PieceSonicRing::PieceSonicRing(int vx, int vy) : dx(vx) { dy = vy; }

void PieceSonicRing::update() {
	x += dx;
	tombe2();

	if (plat(x + dx, y + dy) != 0) {
		if (dy > 2) {
			dy = -dy + 1;
		} else if ((dy == 2) || (dy == 1)) {
			dy = -2;
		}
	}

	ss_etape++;
	ss_etape %= 4;

	if (ss_etape == 0) {
		etape++;
		etape %= 4;
	}

	pic = pbk_ennemis[119 + etape];
	updateADetruire();
}
