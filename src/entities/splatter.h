#pragma once

#include "entities/character.h"

class Splatter : public Sprite {
   public:
	Character *pere;
	int deltax;
	int deltay;

	virtual void update();
};
