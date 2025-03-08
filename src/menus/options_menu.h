#pragma once

#include "engine/control_alias.h"
#include "menus/abstract_menu.h"

#include "menus/keys_menu.h"
#include "menus/options_panel.h"

class OptionsMenu : public AbstractMenu {
   public:
	OptionsMenu() : keyboard_menu_(SCHEME_KEYBOARD), mouse_menu_(SCHEME_MOUSE), pad_menu_(SCHEME_PAD), active_menu_(&options_menu_) {}
	void Draw(SDL::Surface* surf) const override { active_menu_->Draw(surf); }
	int ProcessEvent() override;

   private:
	OptionsPanel options_menu_;
	KeysMenu keyboard_menu_;
	KeysMenu mouse_menu_;
	KeysMenu pad_menu_;
	AbstractMenu* active_menu_;
};
