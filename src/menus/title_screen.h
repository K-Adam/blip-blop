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
	// Compared with sound_on/music_on when the menu ends to reload only what changed
	bool start_sound_on;
	bool start_music_on;

	TitleScreen() : active_menu_(&first_menu_) {}
	void start();
	int update();  // 0 stays in the menu, 1 resumes the game, 2 quits
	void stop();
	void draw(SDL::Surface* surf);
};
