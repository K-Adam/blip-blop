#include "entities/enemies/enemy_spikes.h"

EnemySpikes::EnemySpikes() : y_origine(0) {
	pv = 1;
	pic = pbk_ennemis[168];
}

void EnemySpikes::update() {
	if ((game_flag[0] == 3) || (game_flag[0] == 2)) {
		if (-104 > y_origine) {
			// Fully out, nothing left to raise
		} else if (-96 > y_origine) {
			y -= 1;
			y_origine -= 1;
		} else if (-86 > y_origine) {
			y -= 2;
			y_origine -= 2;
		} else if (-66 > y_origine) {
			y -= 4;
			y_origine -= 4;
		} else if (-46 > y_origine) {
			y -= 6;
			y_origine -= 6;
		} else if (-26 > y_origine) {
			y -= 8;
			y_origine -= 8;
		} else if (0 >= y_origine) {
			y -= 10;
			y_origine -= 10;
		}
		colFromPic();
	} else if (game_flag[0] >= 5) {
		if (-104 > y_origine) {
			y += 10;
			y_origine += 10;
		} else if (-96 > y_origine) {
			y += 8;
			y_origine += 8;
		} else if (-86 > y_origine) {
			y += 6;
			y_origine += 6;
		} else if (-66 > y_origine) {
			y += 4;
			y_origine += 4;
		} else if (-46 > y_origine) {
			y += 3;
			y_origine += 3;
		} else if (-26 > y_origine) {
			y += 2;
			y_origine += 2;
		} else if (0 >= y_origine) {
			y += 1;
			y_origine += 1;
		} else {
			// Fully retracted, safe to remove
			for (int i = 28; i <= 55; i++) murs_opaques[i][108] = false;

			a_detruire = true;
		}
		colFromPic();
	}
}

void EnemySpikes::affiche() {
	SDL::Surface* surf;
	int xs;
	int ys;
	Rect r;

	surf = pbk_ennemis[168]->Surf();
	xs = pbk_ennemis[168]->xSize();
	ys = pbk_ennemis[168]->ySize();

	r.top = 0;
	r.left = 0;

	r.right = 39;
	r.bottom = -y_origine;

	backSurface->BltFast(x - offset, y, surf, &r);
}

void EnemySpikes::colFromPic() {
	Sprite::colFromPic();

	x2 += 25;
	x1 += 25;
}

void EnemySpikes::estTouche(Shot* tir) {}
