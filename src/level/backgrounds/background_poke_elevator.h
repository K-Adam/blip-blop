#pragma once

#include "entities/sprite.h"

class BackgroundPokeElevator : public Sprite {
   public:
	// Starts moving once game_flag[2] passes active_after, a hidden one waits off screen until then
	BackgroundPokeElevator(int active_after, bool hidden);
	void update() override;

   private:
	int active_after_;
	bool hidden_;
};
