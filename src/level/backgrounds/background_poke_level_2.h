#pragma once

#include "entities/sprite.h"

class BackgroundPokeLevel2 : public Sprite {
   private:
	int anim_step_L1;
	int anim_step_E1;
	int anim_step_V;
	int anim_step_E2;
	int anim_step_L2;

   public:
	BackgroundPokeLevel2();

	void update() override;
	void render() override;
};
