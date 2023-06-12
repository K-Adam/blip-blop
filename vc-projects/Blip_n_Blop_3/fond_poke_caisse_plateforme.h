/******************************************************************
*
*
*		---------------------------
*		    FondPokeCaisseplateforme.h
*		---------------------------
*
*
*
*		Mephisto / LOADED -   V 0.1 - 21 Janvier 2001
*
*
*
******************************************************************/

#pragma once

#include "sprite.h"

class FondPokeCaissePlateforme : public Sprite
{
private:
	int y_cible;

public:
	FondPokeCaissePlateforme(int niveau_y);
	virtual void update();

};
