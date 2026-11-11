#include "entities/piece_rayman.h"

PieceRayman::PieceRayman(const Picture* p, int piece, int vx, int vy) : dx(vx), piece(piece) {
	pic = p;
	dy = vy;
}

void PieceRayman::update() {
	x += dx;
	fall();

	if (level().terrain.platform_y(x, y) != 0) {
		level().rules.flags[0] = 2;
		level().rules.flags[2] = piece;
		engrave(x, y, pic);
		destroyed = true;
	}
}
