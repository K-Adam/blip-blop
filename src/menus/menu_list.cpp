#include "menus/menu_list.h"

#include "core/assets.h"
#include "engine/control_alias.h"
#include "engine/half_tone.h"
#include "engine/input.h"

int MenuList::ComputeWidth() const {
	int width = 0;

	for (const std::string& s : items_) {
		int tmp = assets().fonts.menu.width(s);

		if (tmp > width) {
			width = tmp;
		}
	}

	width += 60;
	width /= 2;

	if (width < 200)
		width = 200;
	else if (width > 320)
		width = 320;
	return width;
}

void MenuList::ShadeTextBox(SDL::Surface* surf) const {
	int ys = (int)items_.size() * 15;
	int width = ComputeWidth();

	Rect rec;
	rec.top = 220 - ys;
	rec.left = 320 - width;
	rec.bottom = 260 + ys;
	rec.right = 320 + width;

	halfTone(surf, &rec);
}

void MenuList::Draw(SDL::Surface* surf) const {
	ShadeTextBox(surf);

	int y = 240 - (int)items_.size() * 15;
	for (size_t i = 0; i < items_.size(); ++i) {
		if ((size_t)focused_ == i)
			assets().fonts.menus.printC(surf, 320, y, items_[i]);
		else
			assets().fonts.menu.printC(surf, 320, y, items_[i]);

		y += 30;
	}
}

int MenuList::ItemAt(int x, int y) const {
	int top = 240 - (int)items_.size() * 15;
	int width = ComputeWidth();
	if (x < 320 - width || x >= 320 + width || y < top) return -1;
	int i = (y - top) / 30;
	return i < (int)items_.size() ? i : -1;
}

bool MenuList::Navigate() {
	if (in.pressedKey(DIK_UP) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_UP)) {
		MoveUp();
	} else if (in.pressedKey(DIK_DOWN) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_DOWN)) {
		MoveDown();
	}
	int hovered = ItemAt(in.mouseX(), in.mouseY());
	if (in.mouseMoved() && hovered >= 0) focused_ = hovered;
	// A click also reads as Enter, it only chooses the item under the cursor
	if (in.pressedCode(MOUSE_CODE(SDL_BUTTON_LEFT))) {
		if (hovered < 0) return false;
		focused_ = hovered;
		return true;
	}
	return in.pressedKey(DIK_RETURN) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_FIRE);
}
