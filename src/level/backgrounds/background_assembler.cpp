#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_assembler.h"

BackgroundAssembler::BackgroundAssembler() {}

void BackgroundAssembler::update() {
	anim_step += 1;
	anim_step %= 432;
}

void BackgroundAssembler::render() {
	Rect r;
	int width_px;

	r.top = anim_step;
	r.left = 0;

	if (x + 75 > offset + 640)
		width_px = offset + 640 - x;
	else
		width_px = 75;

	r.right = width_px;
	r.bottom = 87 + anim_step;

	pbk_level.pic("assembler_listing")->BlitPart(backSurface, x - offset, y, r);
}
