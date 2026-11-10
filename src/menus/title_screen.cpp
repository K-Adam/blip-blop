#include "menus/title_screen.h"
#include "engine/control_alias.h"
#include "level/world.h"
#include "core/assets.h"
#include "engine/input.h"
#include "engine/key_translator.h"
#include "menus/txt_defines.h"

#define NB_TXT 10

int TitleScreen::update() {
	MenuType next = MenuType(active_menu_->ProcessEvent());

	switch (next) {
		case MenuType::Main:
			active_menu_ = &first_menu_;
			break;
		case MenuType::Options:
			active_menu_ = &options_menu_;
			break;
		case MenuType::Play:
			return RET_START_GAME;
		case MenuType::Scores:
			return RET_HISCORES;
		case MenuType::Exit:
			return RET_EXIT;
		default:
			break;
	}

	return RET_CONTINUE;
}

void TitleScreen::draw(SDL::Surface* surf) { active_menu_->Draw(surf); }
