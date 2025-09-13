#include "entities/shots/shot_bb_laser.h"
#include "core/debug.h"

ShotBBLaser::ShotBBLaser() : base(0), hauteur(0), width_px(0) {}

void ShotBBLaser::setDir(int d) {
	ShotBB::setDir(d);

	base = hauteur = width_px = 0;

	dir /= 2;

	int nx = x;
	int ny = y;

	switch (dir) {
		case 0:	 // Down

			if (y < 0) {
				ny = y = 0;
			}

			while (ny > -5 && ny < 485 && !solid_wall(nx, ny)) ny += 8;

			hauteur = ny - y;
			base = 140;

			x1 = x;
			y1 = y;
			x2 = x + 6;
			y2 = ny;
			col_on = true;
			break;

		case 2:	 // Right

			while (nx > offset - 5 && nx < offset + 645 && !solid_wall(nx, ny)) nx += 8;

			width_px = nx - x;
			base = 136;

			x1 = x;
			y1 = y;
			x2 = nx;
			y2 = y + 6;
			col_on = true;
			break;

		case 4:	 // Up

			while (ny > -5 && ny < 485 && !solid_wall(nx, ny)) ny -= 8;

			hauteur = y - ny;
			base = 140;

			x1 = x;
			y1 = ny;
			x2 = x + 6;
			y2 = y;
			y = ny;
			col_on = true;
			break;

		case 6:	 // Left

			while (nx > offset - 5 && nx < offset + 645 && !solid_wall(nx, ny)) nx -= 8;

			width_px = x - nx;
			base = 136;

			x1 = nx;
			y1 = y;
			x2 = x;
			y2 = y + 6;
			x = nx;
			col_on = true;
			break;
	}

	anim_tick = 2;
}

void ShotBBLaser::update() {
	anim_tick -= 1;

	if (anim_tick == 0) destroyed = true;
}

void ShotBBLaser::render() {
	SDL::Surface* surf = pbk_bb[base + anim_step]->Surf();
	Rect r;
	int xx = x - offset;
	int nx = x;
	int ny = y;

	switch (dir) {
		case 0:	 // Down
			r.top = 0;
			r.left = 0;
			r.right = 6;

			if (y + hauteur > 479) hauteur = 479 - y;

			r.bottom = hauteur;

			backSurface->BltFast(xx, y, surf, &r);
			if (y + hauteur < 480) draw(x + 2, y + hauteur, pbk_misc[74 + anim_step]);
			break;

		case 1:	 // Down right

			while (nx > offset - 20 && nx < offset + 660 && ny > -50 && ny < 500 && !solid_wall(nx, ny)) {
				draw(nx, ny, pbk_bb[144 + anim_step]);
				nx += 8;
				ny += 8;
			}

			draw(nx, ny + 5, pbk_misc[74 + anim_step]);
			break;

		case 2:	 // Right
			r.top = 0;
			r.left = 0;
			r.bottom = 6;

			if (xx + width_px > 640)
				r.right = 640 - xx;
			else
				r.right = width_px;

			backSurface->BltFast(xx, y, surf, &r);

			if (xx + width_px < 640) draw(x + width_px, y + 3, pbk_misc[74 + anim_step]);
			break;

		case 3:	 // Up right

			while (nx > offset - 20 && nx < offset + 660 && ny > -20 && ny < 500 && !solid_wall(nx, ny)) {
				draw(nx, ny, pbk_bb[148 + anim_step]);
				nx += 8;
				ny -= 8;
			}

			draw(nx - 5, ny + 10, pbk_misc[74 + anim_step]);
			break;

		case 4:	 // Up
			r.top = 0;
			r.left = 0;
			r.right = 6;

			if (y < 0) {
				hauteur += y;
				y = 0;
			}

			r.bottom = hauteur;

			backSurface->BltFast(xx, y, surf, &r);

			if (y > 0) draw(x + 3, y, pbk_misc[74 + anim_step]);
			break;

		case 5:	 // Up left

			while (nx > offset - 20 && nx < offset + 660 && ny > -20 && ny < 500 && !solid_wall(nx, ny)) {
				draw(nx, ny, pbk_bb[144 + anim_step]);
				nx -= 8;
				ny -= 8;
			}

			draw(nx + 5, ny + 10, pbk_misc[74 + anim_step]);
			break;

		case 6:	 // Left
			r.top = 0;
			r.left = 0;
			r.bottom = 6;

			if (xx < 0) {
				r.right = width_px - xx;
				xx = 0;
			} else
				r.right = width_px;

			backSurface->BltFast(xx, y, surf, &r);

			if (xx > 0) draw(x + 2, y + 3, pbk_misc[74 + anim_step]);
			break;

		case 7:	 // Down left

			while (nx > offset - 20 && nx < offset + 660 && ny > -50 && ny < 500 && !solid_wall(nx, ny)) {
				draw(nx, ny, pbk_bb[148 + anim_step]);
				nx -= 8;
				ny += 8;
			}

			draw(nx, ny + 5, pbk_misc[74 + anim_step]);
			break;
	}
}

bool ShotBBLaser::collision(const Sprite* s) const {
	switch (dir) {
		case 0:
		case 2:
		case 4:
		case 6:
			return Sprite::collision(s);
			break;
	}

	int y_laser1;
	int y_laser2;

	int xe1 = s->x1;
	int xe2 = s->x2;
	int ye1 = s->y1;
	int ye2 = s->y2;

	if (s->col_on == false) return false;

	switch (dir) {
		case 1:	 // Down right
			if (xe2 < x || ye2 < y) return false;

			y_laser1 = (xe1 - x) + y;
			y_laser2 = (xe2 - x) + y;

			break;

		case 3:	 // Up right
			if (xe2 < x || ye1 > y) return false;

			y_laser1 = -(xe1 - x) + y;
			y_laser2 = -(xe2 - x) + y;

			break;

		case 5:	 // Up left
			if (xe1 > x || ye1 > y) return false;

			y_laser1 = (xe1 - x) + y;
			y_laser2 = (xe2 - x) + y;

			break;

		case 7:	 // Down left
			if (xe1 > x || ye2 < y) return false;

			y_laser1 = -(xe1 - x) + y;
			y_laser2 = -(xe2 - x) + y;

			break;
	}

	return ((y_laser1 > ye1 && y_laser1 < ye2) || (y_laser2 > ye1 && y_laser2 < ye2));
}
