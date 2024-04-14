#include "core/globals.h"
#include "level/backgrounds/background_electronics.h"

void BackgroundElectronic1::update() {
	ss_etape += 1;
	ss_etape %= 2;
	if (ss_etape == 0) {
		etape += 1;
		etape %= 15;
	}
	pic = pbk_niveau[27 + etape];
}

void BackgroundElectronic1::affiche() {
	Sprite::affiche();
	draw(x, y, pbk_niveau[42]);
}

void BackgroundElectronic2::update() {
	ss_etape += 1;
	ss_etape %= 2;
	if (ss_etape == 0) {
		etape += 1;
		etape %= 15;
	}
	pic = pbk_niveau[27 + etape];
}

void BackgroundElectronic2::affiche() {
	Sprite::affiche();
	draw(x, y, pbk_niveau[44]);
}

void BackgroundElectronic3::update() {
	ss_etape += 1;
	ss_etape %= 2;
	if (ss_etape == 0) {
		etape += 1;
		etape %= 15;
	}
	pic = pbk_niveau[27 + etape];
}

void BackgroundElectronic3::affiche() {
	Sprite::affiche();
	draw(x, y, pbk_niveau[43]);
}

void BackgroundElectronic4::update() {
	ss_etape += 1;
	ss_etape %= 2;
	if (ss_etape == 0) {
		etape += 1;
		etape %= 15;
	}
	pic = pbk_niveau[27 + etape];
}
