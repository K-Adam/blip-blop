#include "entities/sprite.h"

Sprite::Sprite()
	: pic(NULL),
	  dy(0),
	  dir(0),
	  destroyed(false),
	  anim_step(0),
	  anim_tick(0),
	  grav_tick(0),
	  x(0),
	  y(0),
	  x1(-1000),
	  y1(-1000),
	  x2(-2000),
	  y2(-2000),
	  col_on(false),
	  wait_bubble(0) {}

void Sprite::fall() {
	grav_tick += 1;
	grav_tick %= GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = platform_y(x, y + dy);

	if (ny != 0 && dy > 0)
		y = ny;
	else
		y += dy;
}

void Sprite::fall2() {
	grav_tick += 1;
	grav_tick %= GRAVITY_DELAY;

	if (grav_tick == 0 && dy < GRAVITY_MAX) dy += 1;

	if (dy < 0 && solid_wall(x, y + dy)) dy = GRAVITY_MAX;

	int ny = platform_y(x, y + dy);

	if (ny != 0 && dy > 0 && platform_y(x, y) == 0)
		y = ny;
	else
		y += dy;
}

void Sprite::walk(int ndx) {
	x += ndx;

	int ny = platform_y(x, y);

	if (ny != 0) {
		y = ny;
	} else {
		ny = platform_y(x, y + 5);

		if (ny != 0) {
			y = ny;
		} else {
			ny = platform_y(x, y - 5);

			if (ny != 0) y = ny;
		}
	}
}

void Sprite::colFromPic() {
	if (pic == NULL) {
		col_on = false;
	} else {
		x1 = x - pic->xSpot();
		y1 = y - pic->ySpot();

		x2 = x1 + pic->xSize();
		y2 = y1 + pic->ySize();

		col_on = true;
	}
}

void Sprite::noCol() { col_on = false; }
