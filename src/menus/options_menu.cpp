#include "menus/options_menu.h"

#include "core/config.h"
#include "engine/input.h"

int OptionsMenu::ProcessEvent() {
	MenuType next = MenuType(active_menu_->ProcessEvent());
	switch (next) {
		case MenuType::Main:
			active_menu_ = &options_menu_;
			save_settings();
			return MenuType::Main;
		case MenuType::Options:
			active_menu_ = &options_menu_;
			break;
		case MenuType::Keys_Keyboard:
		case MenuType::Keys_Mouse:
		case MenuType::Keys_Pad: {
			KeysMenu* keys = next == MenuType::Keys_Keyboard ? &keyboard_menu_ : next == MenuType::Keys_Mouse ? &mouse_menu_ : &pad_menu_;
			// The pause menu and the title screen have their own copy, the other one may have changed a binding
			if (active_menu_ != keys) keys->Refresh();
			active_menu_ = keys;
			break;
		}
	}
	return MenuType::Options;
}
