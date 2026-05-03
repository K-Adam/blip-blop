#pragma once

#include <algorithm>
#include <cstdint>

#include "engine/chrono.h"

// The game updates in fixed steps, this decides how many steps to run per drawn frame
class UpdateRegulator {
   public:
	// Call once per drawn frame
	int Step() {
		if (fixedStep()) return 1;
		std::uint32_t t = now();
		// A window drag or a hidden browser tab pauses the game instead of replaying the stall in one frame
		acc_ += std::min<std::uint32_t>(t - last_, MAX_CATCH_UP_MS);
		last_ = t;
		int steps = acc_ / UPDATE_PERIOD_MS;
		acc_ %= UPDATE_PERIOD_MS;
		return steps;
	}

	// After loading, a dialog or a menu, so the wait is not replayed
	void Skip() {
		last_ = now();
		acc_ = 0;
	}

   private:
	static constexpr std::uint32_t MAX_CATCH_UP_MS = 100;

	std::uint32_t last_ = now();
	std::uint32_t acc_ = 0;
};
