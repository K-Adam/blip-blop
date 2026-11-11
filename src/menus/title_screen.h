#pragma once

#include <string>
#include <vector>

#include "engine/graphics.h"
#include "menus/main_menu.h"
#include "menus/options_menu.h"

#define RET_CONTINUE 0
#define RET_START_GAME 1
#define RET_HISCORES 2
#define RET_EXIT 3
#define RET_CREDITS 4

class TitleScreen {
	MainMenu first_menu_;
	OptionsMenu options_menu_;
	AbstractMenu* active_menu_;

   public:
	TitleScreen() : active_menu_(&first_menu_) {}
	int update();  // One of the RET_ codes
	void draw(SDL::Surface* surf);
};
