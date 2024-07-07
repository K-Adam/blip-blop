#include "menus/start_menu.h"

#include "engine/control_alias.h"
#include "engine/input.h"
#include "core/text_data.h"
#include "menus/txt_defines.h"

StartMenu::StartMenu() {
	items_.AddEntry(txt_data[TXT_START_GAME1]);
	items_.AddEntry(txt_data[TXT_START_GAME2]);
	items_.AddEntry(txt_data[TXT_RETURN]);
}

int StartMenu::ProcessEvent() {
	if (in.pressedKey(DIK_UP) || in.pressedAlias(ALIAS_P1_UP)) {
		items_.MoveUp();
	} else if (in.pressedKey(DIK_DOWN) || in.pressedAlias(ALIAS_P1_DOWN)) {
		items_.MoveDown();
	}
	if (in.pressedKey(DIK_RETURN) || in.pressedAlias(ALIAS_P1_FIRE)) {
		switch (items_.focused()) {
			case 0:
				return MenuType::Game_1;
			case 1:
				return MenuType::Game_2;
			case 2:
				return MenuType::Main;
		}
	}
	return MenuType::Start;
}
