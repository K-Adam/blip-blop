#pragma once

#ifndef TEXTE_COOL_CPP
extern bool txt_cool_free[];
#endif

class TexteCool {
   public:
	int y;
	int nn;
	int ntxt;
	int x;
	int t;

	TexteCool();

	void update();
	void affiche();

	bool aDetruire() { return x <= -320; };
};

void clearTexteCool();
