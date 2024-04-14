#include "entities/piece_snorky_head.h"
#include "entities/enemies/enemy.h"

PieceSnorkyBaseHeadWhole::PieceSnorkyBaseHeadWhole() { dy = 0; }

void PieceSnorkyBaseHeadWhole::update() {
	ss_etape++;
	ss_etape %= 5;
	if (ss_etape == 0) {
		etape++;
	}

	if (etape == 2 && ss_etape == 0) {
		Sprite* s;
		if (dir == SENS_DROITE) {
			s = new PieceSnorkyBaseTrunk(1 + rand() % 2, -1 + rand() % 2);
			s->x = x + 19;
		} else {
			s = new PieceSnorkyBaseTrunk(-1 - rand() % 2, -1 - rand() % 2);
			s->x = x - 19;
		}

		s->dir = dir;
		s->y = y - 26;

		list_giclures.emplace_back(s);
	}

	if (etape == 3 && ss_etape == 0) {
		Sprite* s;
		if (dir == SENS_DROITE) {
			s = new PieceSnorkyBaseHeadBackPart(1, 0);
			s->x = x + 0;
		} else {
			s = new PieceSnorkyBaseHeadBackPart(-1, 0);
			s->x = x - 0;
		}

		s->dir = dir;
		s->y = y + 10;

		list_giclures.emplace_back(s);
	}

	if (etape == 6) {
		grave(x, y, pic);
		a_detruire = true;
	} else {
		tombe();
		if (dir == SENS_DROITE) {
			x += 1;
			pic = pbk_ennemis[52 + etape];
			if (etape >= 3) {
				tombe();
				x += 1;
			}
			if ((plat(x, y) != 0) && (x > 0)) {
				x -= 6 - etape;
			}
		} else {
			x -= 1;
			pic = pbk_ennemis[85 + etape];
			if (etape >= 3) {
				tombe();
				x -= 1;
			}
			if ((plat(x, y) != 0) && (x < 0)) {
				x += 6 - etape;
			}
		}
	}
}

PieceSnorkyHyporiderBody::PieceSnorkyHyporiderBody(int vx) {
	dx = vx;
	dy = -1;
}

void PieceSnorkyHyporiderBody::update() {
	tombe();
	x += dx;
	ss_etape++;
	ss_etape %= 6;
	if ((ss_etape == 0) && (etape < 3)) {
		etape++;
	}

	if (etape < 3) {
		if (dir == SENS_DROITE) {
			pic = pbk_ennemis[307 + etape];
		} else {
			pic = pbk_ennemis[320 + etape];
		}
	} else if (plat(x, y) != 0) {
		if (ss_etape == 0) {
			etape++;
		}
		if (etape < 5) {
			if (dir == SENS_DROITE) {
				pic = pbk_ennemis[307 + etape];
			} else {
				pic = pbk_ennemis[320 + etape];
			}
		} else {
			grave(x, y, pic);
			a_detruire = true;
		}
	}
}

PieceSnorkyBaseHeadBackPart::PieceSnorkyBaseHeadBackPart(int vx, int vy) {
	dx = vx;
	dy = vy;
}

void PieceSnorkyBaseHeadBackPart::update() {
	tombe();
	x += dx;
	ss_etape++;
	ss_etape %= 6;
	if ((ss_etape == 0) && (etape < 3)) {
		etape++;
	}

	if (dir == SENS_DROITE) {
		pic = pbk_ennemis[59 + etape];
	} else {
		pic = pbk_ennemis[92 + etape];
	}

	if (etape == 3 && (plat(x, y) != 0)) {
		grave(x, y, pic);
		a_detruire = true;
	}
}

PieceSnorkyBaseTrunk::PieceSnorkyBaseTrunk(int vx, int vy) {
	dx = vx;
	dy = vy;
}

void PieceSnorkyBaseTrunk::update() {
	tombe();
	x += dx;
	ss_etape++;
	ss_etape %= 3;
	if (ss_etape == 0) {
		etape++;
		etape %= 8;
	}

	pic = pbk_ennemis[63 + etape];

	if (plat(x, y) != 0) {
		grave(x, y, pic);
		a_detruire = true;
	}
}
