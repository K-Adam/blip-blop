#include "menus/pause_menu.h"

#include "core/app.h"
#include "menus/txt_defines.h"

#include <cassert>

PauseMenu::PauseMenu() {
	items_.AddEntry(texts()[TXT_RESUME]);
	items_.AddEntry(texts()[TXT_OPTIONS]);
	items_.AddEntry(texts()[TXT_EXIT]);
}
int PauseMenu::ProcessEvent() {
	if (items_.Navigate()) {
		switch (items_.focused()) {
			case 0:
				return MenuType::Play;
			case 1:
				return MenuType::Options;
			case 2:
				return MenuType::Exit;
		}
		return MenuType::Main;
	}
	return -1;
}
