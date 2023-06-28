/******************************************************************
*
*
*		---------------------------
*		    FondMonolite.h
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

class FondMonolite : public Sprite
{
public:
	int ss_etape_levitation;
	int etape_levitation;

	FondMonolite();
	virtual void update();
};
