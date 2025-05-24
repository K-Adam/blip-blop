#pragma once

#include <string>

#include "menus/abstract_menu.h"
#include "menus/menu_list.h"

class KeysMenu : public AbstractMenu {
   public:
	explicit KeysMenu(int scheme);
	void Draw(SDL::Surface* surf) const override { items_.Draw(surf); }
	int ProcessEvent() override;
	void Refresh();

   private:
	enum class State { WaitingRelease, WaitingKey, Browsing };
	void SetKey(unsigned int key);
	std::string KeyText(int action) const;

	MenuList items_;
	State state_ = State::Browsing;
	int scheme_;
	int self_;
	int first_;
};
