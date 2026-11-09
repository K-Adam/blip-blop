#pragma once

#include "engine/graphics.h"
#include "menus/options_menu.h"
#include "menus/pause_menu.h"

class MenuGame {
	PauseMenu pause_menu_;
	OptionsMenu options_menu_;
	AbstractMenu* active_;

   public:
	MenuGame() : active_(&pause_menu_) {}
	int Update();  // 0 stays in the menu, 1 resumes the game, 2 quits
	void Draw(SDL::Surface* surf) { active_->Draw(surf); }
};
