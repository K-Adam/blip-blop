#include "core/globals.h"
#include "level/backgrounds/background_assembler.h"

BackgroundAssembler::BackgroundAssembler() {}

void BackgroundAssembler::update() {
	etape += 1;
	etape %= 432;
}

void BackgroundAssembler::affiche() {
	SDL::Surface* surf;
	int xs;
	int ys;
	Rect r;
	int largeur;

	surf = pbk_niveau[53]->Surf();
	xs = pbk_niveau[53]->xSize();
	ys = pbk_niveau[53]->ySize();

	r.top = etape;
	r.left = 0;

	if (x + 75 > offset + 640)
		largeur = offset + 640 - x;
	else
		largeur = 75;

	r.right = largeur;
	r.bottom = 87 + etape;

	backSurface->BltFast(x - offset, y, surf, &r);
}
