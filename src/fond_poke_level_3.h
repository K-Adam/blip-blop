#pragma once

#include "sprite.h"

#define PBK_3 142
#define PBK_L 136
#define PBK_E 133
#define PBK_V 139

class FondPokeLevel3 : public Sprite {
   private:
	int etape_E1;
	int etape_V;
	int etape_E2;
	int etape_L2;

   public:
	FondPokeLevel3();

	virtual void update();
	virtual void affiche();
};
