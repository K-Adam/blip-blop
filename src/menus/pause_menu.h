#pragma once

#include "menus/abstract_menu.h"
#include "menus/menu_list.h"

class PauseMenu : public AbstractMenu {
   public:
	PauseMenu();
	void Draw(SDL::Surface* surf) const override { items_.Draw(surf); }
	int ProcessEvent() override;

   private:
	MenuList items_;
};
