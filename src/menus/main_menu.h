#pragma once

#include "menus/abstract_menu.h"
#include "menus/menu_list.h"

class MainMenu : public AbstractMenu {
   public:
	MainMenu();
	void Draw(SDL::Surface* surf) const override { items_.Draw(surf); }
	int ProcessEvent() override;

   private:
	MenuList items_;
};
