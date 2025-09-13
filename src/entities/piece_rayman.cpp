#include "entities/piece_rayman.h"

PieceRayman::PieceRayman(int m_pic, int vx, int vy) : dx(vx) {
	pic = pbk_enemies[m_pic];
	dy = vy;
	n_pic = m_pic;
}

void PieceRayman::update() {
	x += dx;
	fall();

	if (platform_y(x, y) != 0) {
		switch (n_pic) {
			case 230:
				game_flag[0] = 2;
				game_flag[2] = 1;
				engrave(x, y, pic);
				destroyed = true;
				break;

			case 231:
				game_flag[0] = 2;
				game_flag[2] = 2;
				engrave(x, y, pic);
				destroyed = true;
				break;

			case 270:
			case 271:
				game_flag[0] = 2;
				game_flag[2] = 3;
				engrave(x, y, pic);
				destroyed = true;
				break;
		}
	}
}
