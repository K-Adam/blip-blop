#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_poke_sky.h"

BackgroundPokeSky::BackgroundPokeSky() { pic = pbk_level[48]; }

void BackgroundPokeSky::update() {
	anim_step += 4;
	anim_step %= 384;

	colFromPic();
}

void BackgroundPokeSky::render() {
	Sprite::render();

	SDL::Surface* surf;
	int xs;
	int shift = 0;
	int ys;
	Rect r;
	int width_px;

	surf = pbk_level[50]->Surf();
	xs = pbk_level[50]->xSize();
	ys = pbk_level[50]->ySize();

	r.top = 0;
	r.left = anim_step;

	if (x + 192 > offset + 640) {
		width_px = offset + 640 - x;
	}

	else if (x < offset) {
		width_px = 192;
		r.left = offset - x + anim_step;
		shift = offset - x;
	} else {
		width_px = 192;
	}

	r.right = anim_step + width_px;
	r.bottom = ys;

	backSurface->BltFast(x - offset + shift, y + 50, surf, &r);

	draw(x, y, pbk_level[49]);
}
