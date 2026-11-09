#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_poke_sky.h"

BackgroundPokeSky::BackgroundPokeSky() { pic = pbk_level.pic("sky_frame"); }

void BackgroundPokeSky::update() {
	anim_step += 4;
	anim_step %= 384;

	colFromPic();
}

void BackgroundPokeSky::render() {
	Sprite::render();

	int pic_offset = 0;
	Rect r;
	int width_px;

	r.top = 0;
	r.left = anim_step;

	if (x + 192 > offset + 640) {
		width_px = offset + 640 - x;
	}

	else if (x < offset) {
		width_px = 192;
		r.left = offset - x + anim_step;
		pic_offset = offset - x;
	} else {
		width_px = 192;
	}

	r.right = anim_step + width_px;
	r.bottom = pbk_level.pic("sky_clouds")->ySize();

	pbk_level.pic("sky_clouds")->BlitPart(backSurface, x - offset + pic_offset, y + 50, r);

	draw(x, y, pbk_level.pic("sky_strut"));
}
