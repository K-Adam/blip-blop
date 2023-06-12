/******************************************************************
*
*
*		---------------------------
*		    FondPokeCaisseTombante.h
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

#define UNLOCK_DELAY   32


class FondPokeCaisseTombante : public Sprite
{
public:

	FondPokeCaisseTombante();
	virtual void update();
	virtual void affiche();
};
