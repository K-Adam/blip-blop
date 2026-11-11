#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_poke_sky.h"

BackgroundPokeSky::BackgroundPokeSky() { pic = assets().pics.level.pic("sky_frame"); }

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

	if (x + 192 > level().camera.offset + 640) {
		width_px = level().camera.offset + 640 - x;
	}

	else if (x < level().camera.offset) {
		width_px = 192;
		r.left = level().camera.offset - x + anim_step;
		pic_offset = level().camera.offset - x;
	} else {
		width_px = 192;
	}

	r.right = anim_step + width_px;
	r.bottom = assets().pics.level.pic("sky_clouds")->ySize();

	assets().pics.level.pic("sky_clouds")->BlitPart(graphics.back(), x - level().camera.offset + pic_offset, y + 50, r);

	draw(x, y, assets().pics.level.pic("sky_strut"));
}
