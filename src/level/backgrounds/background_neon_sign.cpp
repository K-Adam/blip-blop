#include "level/backgrounds/background_neon_sign.h"

static const int FLICKER[] = {2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2,
							  1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 0};
static const int FLICKER_LENGTH = 57;

BackgroundNeonSign::BackgroundNeonSign(const char* digit, int digit_period, std::span<const NeonLetter> letters)
	: digit_(digit), digit_period_(digit_period), letters_(letters), steps_(letters.size()) {
	pic = pbk_level.anim(digit_)[0];
}

void BackgroundNeonSign::update() {
	anim_step = (anim_step + 1) % digit_period_;
	for (size_t i = 0; i < letters_.size(); i++) steps_[i] = (steps_[i] + 1) % letters_[i].period;

	if (anim_step < FLICKER_LENGTH) pic = pbk_level.anim(digit_)[FLICKER[anim_step]];
}

void BackgroundNeonSign::render() {
	Sprite::render();

	for (size_t i = 0; i < letters_.size(); i++) {
		if (steps_[i] < FLICKER_LENGTH) draw(x + letters_[i].dx, y - 19, pbk_level.anim(letters_[i].sheet)[FLICKER[steps_[i]]]);
	}
}
