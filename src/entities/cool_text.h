#pragma once

#ifndef TEXTE_COOL_CPP
extern bool txt_cool_free[];
#endif

class CoolText {
   public:
	int y;
	int nn;
	int ntxt;
	int x;
	int t;

	CoolText();

	void update();
	void affiche();

	bool aDetruire() { return x <= -320; };
};

void clearTexteCool();
