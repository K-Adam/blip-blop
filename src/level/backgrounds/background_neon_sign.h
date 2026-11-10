#pragma once

#include <span>
#include <vector>

#include "entities/sprite.h"

struct NeonLetter {
	const char* sheet;
	int dx;
	int period;
};

// Each part flickers on its own period so the sign never blinks in sync
class BackgroundNeonSign : public Sprite {
   public:
	BackgroundNeonSign(const char* digit, int digit_period, std::span<const NeonLetter> letters);
	void update() override;
	void render() override;

   private:
	const char* digit_;
	int digit_period_;
	std::span<const NeonLetter> letters_;
	std::vector<int> steps_;
};
