#define TEXTE_COOL_CPP

#include "level/world.h"
#include "core/assets.h"
#include "entities/cool_text.h"
#include "core/text_data.h"

bool text_cool_free[10];

CoolText::CoolText() : x(960), t(0) {
	nn = 0;

	while (nn < 10 && !text_cool_free[nn]) nn++;

	if (nn >= 10) nn = 0;

	y = 150 + nn * 30;

	text_cool_free[nn] = false;
}

void CoolText::update() {
	if (x == 320) {
		t += 1;

		if (t >= 350) {
			x -= 16;
			text_cool_free[nn] = true;
		}
	} else
		x -= 16;
}

void CoolText::render() { fnt_cool.printC(backSurface, x, y, txt_data[ntxt]); }

void clearCoolText() {
	for (int i = 0; i < 10; i++) text_cool_free[i] = true;
}
