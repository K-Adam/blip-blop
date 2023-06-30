/******************************************************************
*
*
*		---------------------------
*		    FondEtoile.h
*		---------------------------
*
*
*
*		Mephisto / LOADED -   V 0.2 - 12 Janvier 2001
*
*
*
******************************************************************/

#include "fond_etoile.h"

const int anim_etoile [] = {37, 37, 38, 39, 39, 40, 40, 40, 39, 39, 38, 37, 37, 36, 36, 36};


void FondEtoile::update()
{
	pic = pbk_niveau[anime(anim_etoile, 16, 3)];
}
