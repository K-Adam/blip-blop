#pragma once

#include "entities/sprite.h"

class BackgroundLight : public Sprite {
   public:
	bool lit;
	int wait_for_blink;
	int blink_interval;

	explicit BackgroundLight(const char* sheet);

	void update() override;

   private:
	const char* sheet_;
};
