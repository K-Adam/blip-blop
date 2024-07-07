#include "menus/main_menu.h"

#include "engine/control_alias.h"
#include "engine/input.h"
#include "core/text_data.h"
#include "menus/txt_defines.h"

MainMenu::MainMenu() {
	items_.AddEntry(txt_data[TXT_START_GAME]);
	items_.AddEntry("OPTIONS");
	items_.AddEntry(txt_data[TXT_EXIT]);
}

int MainMenu::ProcessEvent() {
	if (in.pressedKey(DIK_UP) || in.pressedAlias(ALIAS_P1_UP)) {
		items_.MoveUp();
	} else if (in.pressedKey(DIK_DOWN) || in.pressedAlias(ALIAS_P1_DOWN)) {
		items_.MoveDown();
	}
	if (in.pressedKey(DIK_RETURN) || in.pressedAlias(ALIAS_P1_FIRE)) {
		switch (items_.focused()) {
			case 0:
				return MenuType::Start;
			case 1:
				return MenuType::Options;
			case 2:
				return MenuType::Exit;
		}
	}
	return MenuType::Main;
}
