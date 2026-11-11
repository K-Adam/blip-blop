#pragma once

#include <string>
#include <vector>

#include "engine/graphics.h"

class MenuList {
   public:
	void Draw(SDL::Surface* surf) const;

	void MoveUp() {
		focused_ -= 1;
		if (focused_ < 0) {
			focused_ = static_cast<int>(items_.size()) - 1;
		}
	}

	void MoveDown() {
		focused_ += 1;
		if (static_cast<size_t>(focused_) >= items_.size()) {
			focused_ = 0;
		}
	}

	int focused() const { return focused_; }

	// Keys, aliases, hover and click. True when the focused item is chosen
	bool Navigate();

	void AddEntry(const std::string& entry) { items_.push_back(entry); }
	void ChangeEntry(int idx, const std::string& entry) { items_[idx] = entry; }

	size_t size() const { return items_.size(); }

   private:
	int ComputeWidth() const;
	int ItemAt(int x, int y) const;
	void ShadeTextBox(SDL::Surface* surf) const;

	std::vector<std::string> items_;
	int focused_ = 0;
};
