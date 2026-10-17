#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "core/debug.h"
#include "engine/picture.h"

// The frames of one named animation, in order
class Flipbook {
	const Picture* first_;
	int count_;
	std::string_view name_;

   public:
	Flipbook(const Picture* first, int count, std::string_view name) : first_(first), count_(count), name_(name) {}
	// Out of range is logged and clamped, so a bad step shows a wrong frame instead of crashing
	const Picture* operator[](int i) const {
		if (i < 0 || i >= count_) {
			debug << "Bug: no frame " << i << " in " << name_ << std::endl;
			if (count_ == 0) return nullptr;
			i = i < 0 ? 0 : count_ - 1;
		}
		return first_ + i;
	}
	int size() const { return count_; }
};

class PictureBank {
   private:
	std::vector<std::unique_ptr<SDL::Surface>> textures_;
	std::vector<Picture> frames_;
	std::map<std::string, std::pair<int, int>, std::less<>> sheets_;  // First frame and count
	std::string file_;

   public:
	bool loadGFX(const char* file);

	Flipbook anim(std::string_view name) const;
	const Picture* pic(std::string_view name) const { return anim(name)[0]; }
	// "name" or "name:frame", for level and script data
	const Picture* frame(std::string_view ref) const;
};
