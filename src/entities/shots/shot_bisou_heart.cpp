#include "entities/shots/shot_bisou_heart.h"
#include <cmath>
#include <numbers>

ShotBisouHeart::ShotBisouHeart() : ok(false) { phi = rand() % 360; }

void ShotBisouHeart::update() {
	y += 1;

	phi += 5;
	phi %= 360;

	x = xbase + int(10 * std::cos(phi * std::numbers::pi / 180));

	if (!ok) {
		ss_etape += 1;
		ss_etape %= 6;

		if (ss_etape == 0) {
			etape += 1;

			if (etape >= 2) ok = true;
		}

		pic = pbk_ennemis[461 + etape];
	}

	if (mur_opaque(x, y)) a_detruire = true;

	colFromPic();
	updateADetruire();
}
