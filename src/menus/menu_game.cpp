#include "menus/menu_game.h"
#include "engine/control_alias.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "menus/txt_defines.h"

int MenuGame::Update() {
	MenuType next = MenuType(active_->ProcessEvent());

	switch (next) {
		case MenuType::Play:
			return 1;
		case MenuType::Exit:
			return 2;
		case MenuType::Main:
			active_ = &pause_menu_;
			break;
		case MenuType::Options:
			active_ = &options_menu_;
			break;
		default:
			break;
	}
	return 0;
}
