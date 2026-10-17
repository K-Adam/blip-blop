#include "entities/splatter.h"

void Splatter::update() {
	if (parent->destroyed || parent->status == STATE_DYING) {
		destroyed = true;
		return;
	}

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4)
		destroyed = true;
	else
		pic = pbk_misc.anim("splatter")[dir * 4 + anim_step];

	x = parent->x + deltax;
	y = parent->y + deltay;
}
