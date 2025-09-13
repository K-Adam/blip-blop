#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_assembler.h"

BackgroundAssembler::BackgroundAssembler() {}

void BackgroundAssembler::update() {
	anim_step += 1;
	anim_step %= 432;
}

void BackgroundAssembler::render() {
	SDL::Surface* surf;
	int xs;
	int ys;
	Rect r;
	int width_px;

	surf = pbk_level[53]->Surf();
	xs = pbk_level[53]->xSize();
	ys = pbk_level[53]->ySize();

	r.top = anim_step;
	r.left = 0;

	if (x + 75 > offset + 640)
		width_px = offset + 640 - x;
	else
		width_px = 75;

	r.right = width_px;
	r.bottom = 87 + anim_step;

	backSurface->BltFast(x - offset, y, surf, &r);
}
