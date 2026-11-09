#include "engine/graphics.h"
#include "entities/sprite.h"
#include "level/world.h"
#include "core/assets.h"
#include "level/scroll.h"
#include "core/debug.h"
#include <SDL2/SDL.h>

#include "entities/ball.h"

int vbuffer_wide = WANTED_VBUFFER_WIDE;
SDL::Surface* videoA = nullptr;
int next_x = 0;
int xTex = 0;
int n_img = 0;
int n_cache = 0;

void drawScrolling() {
	Rect r;

	// The cache lives in a render target, rebuild it from the first screen if the driver wiped it
	if (graphics.TakeTargetsLost()) {
		n_img = 0;
		xTex = 0;
		n_cache = 0;
		next_x = 0;
	}

	if (offset < 0)
		offset = 0;
	else if (offset > (int)level_size - 640)
		offset = (int)level_size - 640;

	int x1 = offset % vbuffer_wide;
	int x2 = (offset + 640) % vbuffer_wide;
	int x3 = (offset + vbuffer_wide - 2) % vbuffer_wide;

	r.top = 0;
	r.bottom = 480;

	while (n_img < ((offset + vbuffer_wide - 2) / 640) ||
		   (n_img < (int)scr_level_size && (next_x != ((x3 + 1) % vbuffer_wide)) && (next_x != ((x3) % vbuffer_wide)))) {
		r.left = xTex;
		r.right = xTex + 2;

		pbk_decor.anim("screens")[num_decor[n_img]]->BlitPart(videoA, next_x, 0, r);

		xTex += 2;

		if (xTex == 640) {
			xTex = 0;
			n_img += 1;
		}

		next_x += 2;

		if (next_x == vbuffer_wide) {
			next_x = 0;
			n_cache += 1;
		}
	}

	if (x1 <= vbuffer_wide - 640) {
		r.left = x1;
		r.right = x1 + 640;

		backSurface->BltFast(0, 0, videoA, &r);
	} else {
		r.left = x1;
		r.right = vbuffer_wide;

		backSurface->BltFast(0, 0, videoA, &r);

		r.left = 0;
		r.right = x2;

		backSurface->BltFast(vbuffer_wide - x1, 0, videoA, &r);
	}
}

void updateScrolling(bool forceOk) {
	if (scroll_locked) {
		if (offset < x_lock) offset = x_lock;
	} else if (scroll_speed != 0 && forceOk) {
		offset += scroll_speed;
	} else if (list_players.size() > 0) {
		int x_avg = 0;

		for (Ball* s : list_players) {
			x_avg += s->x;
		}

		x_avg /= (int)list_players.size();
		x_avg -= 320;  // Half of the 640 screen width

		if (x_avg > offset) {
			if ((x_avg - offset) >= 2)
				offset += 2;
			else
				offset = x_avg;
		}
	}

	if (offset < 0)
		offset = 0;
	else if (offset > (int)level_size - 640)
		offset = (int)level_size - 640;
}
