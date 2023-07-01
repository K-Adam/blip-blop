#include "half_tone.h"

void halfTone(SDL::Surface* surf, Rect* r) {
	SDL::SurfaceInfo ddsd;

	if (surf->Lock(&ddsd, DDLOCK_SURFACEMEMORYPTR, NULL) == false) return;

	unsigned int* ptr = (unsigned int*)ddsd.lpSurface;
	ptr += r->top * surf->Get()->w;
	for (int line = 0; line < r->bottom - r->top; ++line) {
		unsigned int* ptrl = ptr + r->left;
		for (int col = 0; col < r->right - r->left; ++col) {
			(*ptrl) = 0xFF000000 | ((*ptrl) & 0xFF) >> 1 | ((((*ptrl) & 0xFF00) >> 1) & 0xFF00) | ((((*ptrl) & 0xFF0000) >> 1) & 0xFF0000);
			++ptrl;
		}
		ptr += surf->Get()->w;
	}

	surf->Unlock();
}
