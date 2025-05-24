#include "menus/pause_menu.h"

#include "engine/control_alias.h"
#include "engine/input.h"
#include "core/text_data.h"
#include "menus/txt_defines.h"

#include <cassert>

PauseMenu::PauseMenu() {
	items_.AddEntry(txt_data[TXT_RESUME]);
	items_.AddEntry(txt_data[TXT_OPTIONS]);
	items_.AddEntry(txt_data[TXT_EXIT]);
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
