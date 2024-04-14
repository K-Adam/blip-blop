#include "menus/menu_game.h"
#include "core/config.h"
#include "engine/control_alias.h"
#include "core/globals.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "menus/txt_defines.h"
#include "core/text_data.h"

int MenuGame::Update() {
	in.update();
	MenuType next = MenuType(active_->ProcessEvent());
	in.waitClean();

	switch (next) {
		case MenuType::Game_1:
			return 1;
		case MenuType::Exit:
			return 2;
		case MenuType::Main:
			active_ = &pause_menu_;
			break;
		case MenuType::Options:
			active_ = &options_menu_;
	}
	return 0;
}
