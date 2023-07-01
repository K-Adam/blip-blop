/******************************************************************
 *
 *
 *		---------------------------
 *		    FondAssembleur.h
 *		---------------------------
 *
 *
 *
 *		Mephisto / LOADED -   V 0.2 - 12 Janvier 2001
 *
 *
 *
 ******************************************************************/

#pragma once

#include "sprite.h"

class FondAssembleur : public Sprite {
   public:
	FondAssembleur();
	virtual void update();
	virtual void affiche();
};
