#include "engine/graphics.h"

#include "core/misc.h"
#include "core/globals.h"
#include "core/config.h"
#include "core/debug.h"
#include "engine/chrono.h"
#include "engine/errors.h"

Graphics graphics;

void Graphics::Init() {
	if (SDL_Init((SDL_INIT_EVERYTHING & ~SDL_INIT_HAPTIC)) == -1) {
		throw std::runtime_error(std::string("Can't initialize SDL") + SDL_GetError());
	}
}

void Graphics::ToggleFullscreen() {
	fullscreen_ = !fullscreen_;
	SDL_SetWindowFullscreen(window_.get(), fullscreen_ ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}
void Graphics::SetGfxMode(int width, int height, bool fullscreen) {
	fullscreen_ = fullscreen;
	screen_.reset();
	renderer_.reset();
#ifdef __EMSCRIPTEN__
	// The canvas keeps its 640x480 backing store and the page CSS scales it
	Uint32 mode = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
#else
	Uint32 mode = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;
#endif
	window_.reset(SDL_ErrWrap(SDL_CreateWindow("Blip&Blop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | mode)));

	renderer_.reset(SDL_ErrWrap(SDL_CreateRenderer(window_.get(), -1, vSyncOn ? SDL_RENDERER_PRESENTVSYNC : 0)));
	// The game draws a fixed 640x480 frame, the renderer scales it to the window and letterboxes the rest
	SDL_RenderSetLogicalSize(renderer_.get(), width, height);
}

void Graphics::SetVSync(bool on) { SDL_RenderSetVSync(renderer_.get(), on); }

SDL::Surface* Graphics::CreateSurface(int width, int height) {
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	SDL_Surface* surf = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

	SDL::Surface* tmp = new SDL::Surface(surf);
	tmp->FillRect(0, 0xFF000000);
	return tmp;
}

// Tests compare these to prove a refactor draws exactly the same frames
static void PrintFrameHash(SDL_Surface* frame) {
	static std::uint32_t last = 0;
	if (frames_played % 60 != 0 || frames_played == last) return;
	last = frames_played;
	std::uint32_t h = 2166136261u;
	for (int y = 0; y < frame->h; y++) {
		const Uint8* row = (const Uint8*)frame->pixels + y * frame->pitch;
		for (int i = 0; i < frame->w * 4; i++) h = (h ^ row[i]) * 16777619u;
	}
	debug << "frame " << frames_played << " hash " << std::hex << h << std::dec << "\n";
}

void Graphics::Flip() {
	SDL_Surface* frame = backSurface->Get();
	if (!screen_) {
		screen_.reset(SDL_ErrWrap(SDL_CreateTexture(renderer_.get(), frame->format->format, SDL_TEXTUREACCESS_STREAMING, frame->w, frame->h)));
	}
	SDL_UpdateTexture(screen_.get(), NULL, frame->pixels, frame->pitch);
	SDL_RenderClear(renderer_.get());
	SDL_RenderCopy(renderer_.get(), screen_.get(), NULL, NULL);
	SDL_RenderPresent(renderer_.get());
	if (autoquit_frames > 0) PrintFrameHash(frame);
}
