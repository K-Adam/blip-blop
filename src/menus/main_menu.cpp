#include "menus/main_menu.h"

#include "core/text_data.h"
#include "menus/txt_defines.h"

MainMenu::MainMenu() {
	items_.AddEntry(txt_data[TXT_START_GAME]);
	items_.AddEntry("HIGH SCORES");
	items_.AddEntry("OPTIONS");
	items_.AddEntry(txt_data[TXT_EXIT]);
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
