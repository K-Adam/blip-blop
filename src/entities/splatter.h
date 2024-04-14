#pragma once

#include "entities/character.h"

class Giclure : public Sprite {
   public:
	Personnage *pere;
	int deltax;
	int deltay;

	virtual void update();
};
