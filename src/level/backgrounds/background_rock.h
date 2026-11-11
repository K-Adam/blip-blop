#pragma once

#include <span>

#include "entities/sprite.h"

struct RockSpec {
	const char* sheet;
	std::span<const int> frames;
	int frame_delay;
	std::span<const int> rise;	// Played forward going up and backward coming down
	int rise_delay;
	int top;  // Height of the walkable surface above y
};

class BackgroundRock : public Sprite {
   public:
	explicit BackgroundRock(const RockSpec& spec) : spec_(spec) {}
	void update() override;

   private:
	int bob() const;

	RockSpec spec_;
	int levitation_tick = 0;
	int levitation_step = 0;
};
