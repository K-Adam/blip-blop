#include "graphics.h"
#include "picture.h"
#include "dd_gfx.h"
#include "ben_debug.h"

Picture::Picture() : surf(NULL), xspot(0), yspot(0), xsize(0), ysize(0) {}

void Picture::FindSize() {
	xsize = surf->Get()->w;
	ysize = surf->Get()->h;
}

void Picture::SetColorKey(Pixel rgb) {
	if (surf == NULL) {
		debug << "Picture::SetColorKey() - surface à NULL\n";
		return;
	}

	DDSetColorKey(surf, rgb);
}

void Picture::SetSpot(int x, int y) {
	xspot = x;
	yspot = y;
}

void Picture::SetSurface(SDL::Surface* s) {
	surf = s;
	FindSize();
}

void Picture::LoadBMP(char* file) {
	surf = DDLoadBMP(file);
	FindSize();
	xspot = 0;
	yspot = 0;
}

void Picture::LoadBMP(char* file, int flags) {
	surf = DDLoadBMP(file, flags);
	FindSize();
	xspot = 0;
	yspot = 0;
}

void Picture::LoadBMP(char* file, int xs, int ys) {
	surf = DDLoadBMP(file);
	FindSize();
	xspot = xs;
	yspot = ys;
}

void Picture::LoadBMP(char* file, int xs, int ys, int flags) {
	surf = DDLoadBMP(file, flags);
	FindSize();
	xspot = xs;
	yspot = ys;
}

void Picture::BlitTo(SDL::Surface* s, int x, int y) const {
	x -= xspot;
	y -= yspot;

	int x2 = x + xsize;
	int y2 = y + ysize;

	// S'il n'y a rien à afficher, on se casse tout de suite!
	if (x > XPIC_MAX || x2 < 0 || y > YPIC_MAX || y2 < 0) return;

	Rect r;

	r.left = (x < 0) ? -x : 0;
	r.top = (y < 0) ? -y : 0;
	r.right = (x2 > XPIC_MAX) ? XPIC_MAX - x : xsize;
	r.bottom = (y2 > YPIC_MAX) ? YPIC_MAX - y : ysize;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	s->BltFast(x, y, surf, &r, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
}

void Picture::PasteTo(SDL::Surface* s, int x, int y) const {
	x -= xspot;
	y -= yspot;

	int x2 = x + xsize;
	int y2 = y + ysize;

	// S'il n'y a rien à afficher, on se casse tout de suite!
	if (x > XPIC_MAX || x2 < 0 || y > YPIC_MAX || y2 < 0) return;

	Rect r;

	r.left = (x < 0) ? -x : 0;
	r.top = (y < 0) ? -y : 0;
	r.right = (x2 > XPIC_MAX) ? XPIC_MAX - x : xsize;
	r.bottom = (y2 > YPIC_MAX) ? YPIC_MAX - y : ysize;

	if (x < 0) x = 0;
	if (y < 0) y = 0;

	s->BltFast(x, y, surf, &r, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
}

void Picture::Close() {
	if (surf != NULL) surf->Release();
	surf = NULL;
}

Picture::~Picture() { Close(); }

int loadPBK(char* fic, Picture*& p, int rgb) {
	std::ifstream f;
	int nbpic;

	if (p != NULL) {
		debug << "loadPBK/Picture.cpp->" << fic << ":tableau déjà initialisé\n";
		return 0;
	}

	f.open(fic);

	if (f.is_open() == 0) {
		debug << "LoadPBK/Picture.cpp->Ne peut pas ouvrir " << fic << "\n";
		return 0;
	}

	f >> nbpic;

	if (nbpic < 1) {
		debug << "LoadPBK(Picture.cpp)->Fichier " << fic << " corrompu!\n";
		nbpic = 0;
		return 0;
	}

	p = new Picture[nbpic];

	char txt[256];
	int x;
	int y;
	int n;

	for (int i = 0; i < nbpic; i++) {
		f >> n;
		f >> txt;
		f >> x;
		f >> y;

		p[n].LoadBMP(txt, x, y);
		p[n].SetColorKey(rgb);
	}

	f.close();

	return nbpic;
}

void closePBK(Picture*& p, int t) {
	if (p != NULL) {
		for (int i = 0; i < t; i++) p[i].Close();
		delete[] p;
	}
}
