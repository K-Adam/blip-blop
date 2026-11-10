#include "menus/main_menu.h"

#include "core/app.h"
#include "menus/txt_defines.h"

MainMenu::MainMenu() {
	items_.AddEntry(texts()[TXT_START_GAME]);
	items_.AddEntry("HIGH SCORES");
	items_.AddEntry("OPTIONS");
#ifndef __EMSCRIPTEN__
	// A browser tab cannot be closed by the page, quitting would leave a frozen canvas
	items_.AddEntry(texts()[TXT_EXIT]);
#endif
}

int MainMenu::ProcessEvent() {
	if (items_.Navigate()) {
		switch (items_.focused()) {
			case 0:
				return MenuType::Play;
			case 1:
				return MenuType::Scores;
			case 2:
				return MenuType::Options;
			case 3:
				return MenuType::Exit;
		}
	}
	return MenuType::Main;
}
