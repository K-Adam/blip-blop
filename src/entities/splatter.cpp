#include "entities/splatter.h"

void Splatter::update() {
	if (pere->destroyed || pere->status == ETAT_MEURE) {
		destroyed = true;
		return;
	}

	anim_tick += 1;
	anim_tick %= 5;

	if (anim_tick == 0) anim_step += 1;

	if (anim_step == 4)
		destroyed = true;
	else
		pic = pbk_misc[15 + dir * 4 + anim_step];

	x = pere->x + deltax;
	y = pere->y + deltay;
}
