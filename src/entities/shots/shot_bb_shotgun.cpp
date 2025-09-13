#include "entities/shots/shot_bb_shotgun.h"

const int dx_shotbb_shotgun[] = {0, 6, 11, 14, 15, 14, 11, 6, 0, -6, -11, -14, -15, -14, -11, -6};
const int dy_shotbb_shotgun[] = {15, 14, 11, 6, 0, -6, -11, -14, -15, -14, -11, -6, 0, 6, 11, 14};

const int delay_big[] = {1, 2, 2, 1, 2, 1};
const int delay_huge[] = {1, 2, 3, 2, 1, 3};
const int d_big[] = {3, -2, -1, 0, 1, 2, 3};
const int d_small[] = {-2, 0, 2, 0, 1, -1, 2, -1, -2, 1};
const int d_min[] = {-1, 0, 1};

int n_delay_big = 0;
int n_delay_huge = 0;
int n_d_big = 0;
int n_d_small = 0;
int n_d_min = 0;

void ShotBBShotgun::setDir(int d) {
	ShotBB::setDir(d);

	n_delay_big += 1;
	n_delay_big %= 6;

	n_d_big += 1;
	n_d_big %= 7;

	n_d_small += 1;
	n_d_small %= 10;

	switch (dir / 2) {
		case 0:	 // Up and down
		case 4:
			delay_x = delay_big[n_delay_big];
			delay_y = 1;
			dx = 0;
			dy = d_big[n_d_big];
			ddx = d_small[n_d_small];
			ddy = 0;
			break;

		case 2:	 // Left and right
		case 6:
			delay_y = delay_big[n_delay_big];
			delay_x = 1;
			dx = d_big[n_d_big];
			dy = 0;
			ddx = 0;
			ddy = d_small[n_d_small];
			break;

		case 1:	 // Up left and down right diagonal
		case 5:
			n_delay_huge += 1;
			n_delay_huge %= 6;

			delay_y = delay_huge[n_delay_huge];

			n_delay_huge += 1;
			n_delay_huge %= 6;

			delay_x = delay_huge[n_delay_huge];

			dx = d_small[n_d_small];
			dy = d_small[n_d_small];

			n_d_big += 1;
			n_d_big %= 7;

			ddx = d_big[n_d_big];

			n_d_big += 1;
			n_d_big %= 7;

			ddy = d_big[n_d_big];
			break;

		case 3:	 // Down left and up right diagonal
		case 7:
			n_delay_huge += 1;
			n_delay_huge %= 3;

			delay_y = delay_huge[n_delay_huge];

			n_delay_huge += 1;
			n_delay_huge %= 3;

			delay_x = delay_huge[n_delay_huge];

			dx = d_small[n_d_small];
			dy = -d_small[n_d_small];

			n_d_big += 1;
			n_d_big %= 7;

			ddx = d_big[n_d_big];

			n_d_big += 1;
			n_d_big %= 7;

			ddy = d_big[n_d_big];
			break;
	}

	dx += dx_shotbb_shotgun[dir];
	dy += dy_shotbb_shotgun[dir];

	lifespan = 10 + rand() % 7;
	age = 0;
}

void ShotBBShotgun::update() {
	y += dy;
	x += dx;

	dy_tick += 1;
	dy_tick %= delay_y;

	if (dy_tick == 0) y += ddy;

	dx_tick += 1;
	dx_tick %= delay_x;

	if (dx_tick == 0) x += ddx;

	age += 1;

	x += dxScroll;

	if (solid_wall(x, y) || x < offset - 20 || x > offset + 660 || y < -20 || y > 500) {
		pic = NULL;
		noCol();
		destroyed = true;
	} else {
		if (age < lifespan)
			pic = pbk_bb[88 + dir];
		else
			pic = pbk_bb[72 + dir];

		colFromPic();
	}
}
