/******************************************************************
*
*
*		-----------------------
*		    TirCruiser.h
*		-----------------------
*
*
*
*		Mephisto / LOADED -   V 0.3 - 15 Decembre 2000
*
*
*
******************************************************************/

#pragma once

#include "sprite.h"

class TirCruiser : public Sprite
{
public:
	int dx;

	TirCruiser(int vx , int vy, int nb_pic);
	virtual void update();
};
