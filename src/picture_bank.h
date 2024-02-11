#pragma once

#include <memory>
#include <string>
#include <vector>

#include <cassert>

#include "picture.h"

class PictureBank {
   private:
	std::vector<std::unique_ptr<Picture>> tab_;

   public:
	inline Picture* operator[](int n) const {
		assert(n >= 0 && size_t(n) < tab_.size());
		return tab_[n].get();
	};
	inline size_t getSize() const { return tab_.size(); };

	PictureBank();
	bool loadGFX(const char* file);
};
