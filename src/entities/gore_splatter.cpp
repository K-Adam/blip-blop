#include "entities/gore_splatter.h"

GoreSplatter::GoreSplatter(int vx, int vy) : dx(vx) {
	dy = vy;

	if ((dx > -2) && (dx < 0)) {
		dx = -2;
	} else if ((dx < 2) && (dx >= 0)) {
		dx = 2;
	}

	int shift;
	if (dx < 0) {
		shift = 0;
	} else {
		shift = 7;
	}
	if (dy >= 6) {
		pic = pbk_misc[92 + shift];
	} else if (dy >= 4) {
		pic = pbk_misc[91 + shift];
	} else if (dy >= 1) {
		pic = pbk_misc[90 + shift];
	} else if (dy >= 0) {
		pic = pbk_misc[89 + shift];
	} else if (dy >= -2) {
		pic = pbk_misc[88 + shift];
	} else if (dy >= -4) {
		pic = pbk_misc[87 + shift];
	} else {
		pic = pbk_misc[86 + shift];
	}
}

void GoreSplatter::update() {
	int shift;
	if (dx < 0) {
		shift = 0;
	} else {
		shift = 7;
	}

	if (dy >= 6) {
		pic = pbk_misc[92 + shift];
	} else if (dy >= 4) {
		pic = pbk_misc[91 + shift];
	} else if (dy >= 1) {
		pic = pbk_misc[90 + shift];
	} else if (dy >= 0) {
		pic = pbk_misc[89 + shift];
	} else if (dy >= -2) {
		pic = pbk_misc[88 + shift];
	} else if (dy >= -4) {
		pic = pbk_misc[87 + shift];
	} else {
		pic = pbk_misc[86 + shift];
	}
	x += dx;
	fall();

	if ((dy > 0) && platform_y(x, y) != 0) {
		destroyed = true;
	}
	updateDestroyed();
}
