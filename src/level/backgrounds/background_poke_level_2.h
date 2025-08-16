#pragma once

#include "entities/sprite.h"

#define PBK_2 130
#define PBK_L 136
#define PBK_E 133
#define PBK_V 139

class BackgroundPokeLevel2 : public Sprite {
   private:
	int etape_L1;
	int etape_E1;
	int etape_V;
	int etape_E2;
	int etape_L2;

   public:
	BackgroundPokeLevel2();

	void update() override;
	void affiche() override;
};
