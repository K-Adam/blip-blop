#pragma once

#include "sprite.h"

class MorceauTeteRayman : public Sprite {
   private:
	int dx;

   public:
	// methodes "habituelles"
	MorceauTeteRayman(int vx, int vy);
	virtual void update();
};
