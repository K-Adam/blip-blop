#include "entities/shots/shot_sonic_lightning.h"

ShotSonicLightning::ShotSonicLightning() {
	ss_etape = 0;
	etape = 0;
}

void ShotSonicLightning::update() {
	ss_etape += 1;
	ss_etape %= 3;

	if (ss_etape == 0) {
		etape += 1;
	}
	pic = pbk_ennemis[56 + ss_etape];

	if (etape > 30) {
		colFromPic();
		if (etape > 36) {
			a_detruire = true;
		}
	}
	if (etape == 0 || etape > 30) {
		tremblement(7);
	}
}

void ShotSonicLightning::affiche() {
	if (etape == 0 || etape > 30) {
		Sprite::affiche();
	}
}
