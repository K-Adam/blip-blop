#include "level/world.h"
#include "entities/cool_text.h"
#include "core/app.h"

namespace {
// Screen rows a cool text can slide into, one text per row
struct CoolTextRows {
	bool free[10] = {true, true, true, true, true, true, true, true, true, true};
};
}  // namespace

CoolText::CoolText() : x(960), t(0) {
	bool* free = level().shared<CoolTextRows>().free;
	nn = 0;

	while (nn < 10 && !free[nn]) nn++;

	if (nn >= 10) nn = 0;

	y = 150 + nn * 30;

	free[nn] = false;
}

void CoolText::update() {
	if (x == 320) {
		t += 1;

		if (t >= 350) {
			x -= 16;
			level().shared<CoolTextRows>().free[nn] = true;
		}
	} else
		x -= 16;
}

void CoolText::render() { assets().fonts.cool.printC(graphics.back(), x, y, texts()[ntxt]); }
