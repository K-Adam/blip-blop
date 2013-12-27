
#include <stdlib.h>
#include "fondlumiere.h"

#define ETAT_CLIGNOTE	100

FondLumiere::FondLumiere() :  allume(false), wait_for_cligno(0) {
	delai_cligno = 50 + rand() % 200;
	pic = pbk_niveau[0];
}

void FondLumiere::update() {
	static const int anim_cligno [] = {2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0};

	wait_for_cligno += 1;

	if (allume) {
		if (wait_for_cligno >= delai_cligno) {
			allume = false;
			ss_etape = 0;
			etape = 0;
			//pic=pbk_niveau[0];
		}
	} else {
		pic = pbk_niveau[anime(anim_cligno, 58, 3)];

		if (etape == 57) {
			allume = true;
			wait_for_cligno = 0;
			delai_cligno = 50 + rand() % 200;
		}
	}

	if (x < offset - 300)
		a_detruire = true;
}