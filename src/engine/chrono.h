#pragma once

#include <cassert>
#include <cstdint>

#include <SDL2/SDL.h>

// Frames played so far, counted once per frame by Input::update
inline std::uint32_t frames_played = 0;

// Tests set BB_FIXEDSTEP so time follows frames and every run repeats exactly. Read lazily since
// global objects build their timers before main
inline bool fixedStep() {
	static const bool on = SDL_getenv("BB_FIXEDSTEP") != nullptr;
	return on;
}

// The game updates in fixed steps of this length
inline constexpr int UPDATE_PERIOD_MS = 11;

// One update period per frame in fixed step mode
inline std::uint32_t now() { return fixedStep() ? frames_played * UPDATE_PERIOD_MS : SDL_GetTicks(); }

class Chrono {
   private:
	std::uint32_t start_;
	std::uint32_t saved_elapsed_;

   public:
	Chrono() : start_(now()), saved_elapsed_(0) {}

	std::uint32_t start() const { return start_; }

	std::uint32_t elapsed() const { return now() - start_; }
	std::uint32_t saved_elapsed() const { return saved_elapsed_; }

	void Reset() { start_ = now(); }

	void Stop() { saved_elapsed_ = elapsed(); }
};

class Countdown {
   private:
	std::uint32_t end_;

   public:
	Countdown(int duration) : end_(now() + duration) {}
	Countdown() : end_(0) {}

	void Reset(int duration) { end_ = now() + duration; }

	bool is_zero() const { return int(end_) - int(now()) < 0; }
};
