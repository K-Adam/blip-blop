#pragma once

#include "entities/sprite.h"

#define PBK_3 142
#define PBK_L 136
#define PBK_E 133
#define PBK_V 139

class BackgroundPokeLevel3 : public Sprite {
   private:
	int anim_step_E1;
	int anim_step_V;
	int anim_step_E2;
	int anim_step_L2;

   public:
	BackgroundPokeLevel3();

	void update() override;
	void render() override;
};
