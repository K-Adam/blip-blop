#include "entities/gore_splatter.h"

GoreSplatter::GoreSplatter(int vx, int vy) : dx(vx) {
	dy = vy;

	if ((dx > -2) && (dx < 0)) {
		dx = -2;
	} else if ((dx < 2) && (dx >= 0)) {
		dx = 2;
	}

	int pic_offset;
	if (dx < 0) {
		pic_offset = 0;
	} else {
		pic_offset = 7;
	}
	if (dy >= 6) {
		pic = pbk_misc[92 + pic_offset];
	} else if (dy >= 4) {
		pic = pbk_misc[91 + pic_offset];
	} else if (dy >= 1) {
		pic = pbk_misc[90 + pic_offset];
	} else if (dy >= 0) {
		pic = pbk_misc[89 + pic_offset];
	} else if (dy >= -2) {
		pic = pbk_misc[88 + pic_offset];
	} else if (dy >= -4) {
		pic = pbk_misc[87 + pic_offset];
	} else {
		pic = pbk_misc[86 + pic_offset];
	}
}

void GoreSplatter::update() {
	int pic_offset;
	if (dx < 0) {
		pic_offset = 0;
	} else {
		pic_offset = 7;
	}

	if (dy >= 6) {
		pic = pbk_misc[92 + pic_offset];
	} else if (dy >= 4) {
		pic = pbk_misc[91 + pic_offset];
	} else if (dy >= 1) {
		pic = pbk_misc[90 + pic_offset];
	} else if (dy >= 0) {
		pic = pbk_misc[89 + pic_offset];
	} else if (dy >= -2) {
		pic = pbk_misc[88 + pic_offset];
	} else if (dy >= -4) {
		pic = pbk_misc[87 + pic_offset];
	} else {
		pic = pbk_misc[86 + pic_offset];
	}
	x += dx;
	fall();

	if ((dy > 0) && platform_y(x, y) != 0) {
		destroyed = true;
	}
	updateDestroyed();
}
