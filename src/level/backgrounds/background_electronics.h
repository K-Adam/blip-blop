#pragma once

#include "entities/sprite.h"

class BackgroundElectronic : public Sprite {
   public:
	explicit BackgroundElectronic(const char* overlay = nullptr) : overlay_(overlay) {}

	void update() override { pic = assets().pics.level.anim("electronic_board")[step(2, 15)]; }

	void render() override {
		Sprite::render();
		if (overlay_) draw(x, y, assets().pics.level.pic(overlay_));
	}

   private:
	const char* overlay_;
};
