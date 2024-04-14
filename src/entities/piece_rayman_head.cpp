#include "entities/piece_rayman_head.h"

PieceRaymanHead::PieceRaymanHead(int vx, int vy) : dx(vx) {
	pic = pbk_ennemis[496];
	dy = vy;
}

void PieceRaymanHead::update() {
	x += dx;
	tombe();

	ss_etape++;
	ss_etape %= 4;
	if (ss_etape == 0) {
		etape++;
		etape %= 8;
		pic = pbk_ennemis[496 + etape];
	}

	// Et si la tete touche le sol? ?
	if (dy > 0 && (plat(x, y + dy)) != 0) {
		grave(x, y, pic);
		a_detruire = true;
	}
}
