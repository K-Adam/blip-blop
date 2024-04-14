#include "entities/shots/shot_bb_cow.h"
#include "entities/gore_splatter.h"

void ShotBBCow::update() {
	y += dy;

	ss_etape += 1;
	ss_etape %= 8;

	if (ss_etape == 0) {
		etape += 1;
		etape %= 8;
	}

	if (mur_opaque(x, y) && y > 50) {
		pic = NULL;
		if (etape < 4)
			grave(x, y, pbk_bb[211]);
		else
			grave(x, y, pbk_bb[210]);
		col_on = false;
		a_detruire = true;
		tremblement(10);

		GoreSplatter* s;

		for (int i = 0; i < 15; i++) {
			s = new GoreSplatter(rand() % 11 - 5, -2 - rand() % 6);
			s->x = x + rand() % 11 - 5;
			s->y = y - rand() % 30 - 10;
			list_gore.emplace_back(s);
		}

		sbk_misc.play(11);
	} else if (y > 550 || x < offset - 50) {
		a_detruire = true;
	} else {
		pic = pbk_bb[202 + etape];
		colFromPic();
	}
}
