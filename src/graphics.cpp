#include "graphics.h"

#include "ben_divers.h"
#include "config.h"
#include "errors.h"

extern SDL::Surface* backSurface;

void Graphics::Init() {
	if (SDL_Init((SDL_INIT_EVERYTHING & ~SDL_INIT_HAPTIC)) == -1) {
		throw std::runtime_error(std::string("Can't initialize SDL") + SDL_GetError());
	}
}

void Graphics::ToggleFullscreen() {
	fullscreen_ = !fullscreen_;
	SDL_SetWindowFullscreen(window_.get(), fullscreen_ ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}
void Graphics::SetGfxMode(int x, int y, int d, bool fullscreen) {
	x_ = x;
	y_ = y;
	d_ = d;
	fullscreen_ = fullscreen;
	screen_.reset();
	renderer_.reset();
	Uint32 mode = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;
	window_.reset(SDL_ErrWrap(SDL_CreateWindow("Blip&Blop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x, y, SDL_WINDOW_SHOWN | mode)));

	renderer_.reset(SDL_ErrWrap(SDL_CreateRenderer(window_.get(), -1, vSyncOn ? SDL_RENDERER_PRESENTVSYNC : 0)));
	// The game draws a fixed 640x480 frame, the renderer scales it to the window and letterboxes the rest
	SDL_RenderSetLogicalSize(renderer_.get(), x, y);
}

void Graphics::SetVSync(bool on) { SDL_RenderSetVSync(renderer_.get(), on); }

SDL::Surface* Graphics::CreatePrimary() {
	debug << "CreatePrimary() - Creating a 640 x 480 Surface"
		  << "\n";
	return CreateSurface(640, 480, 0);
}

SDL::Surface* Graphics::CreatePrimary(SDL::Surface*& back) {
	debug << "Graphics::CreatePrimary(SDL::Surface * & back) - Creating a "
			 "640x480 surface"
		  << "\n";
	SDL::Surface* tmp = CreateSurface(640, 480);
	back = CreateSurface(640, 480);
	tmp->SetBackBuffer(back);
	return tmp;
}

SDL::Surface* Graphics::CreateSurface(int x, int y) { return CreateSurface(x, y, 0); }

SDL::Surface* Graphics::CreateSurface(int x, int y, int flags) {
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
	SDL_Surface* surf = SDL_CreateRGBSurface(0, x, y, 32, rmask, gmask, bmask, amask);

	SDL::Surface* tmp = new SDL::Surface(surf);
	tmp->FillRect(0, 0xFF000000);
	return tmp;
}

SDL_Surface* Graphics::CreateSDLSurface(int x, int y) {
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
	SDL_Surface* surf = SDL_CreateRGBSurface(0, x, y, 32, rmask, gmask, bmask, amask);
	return (surf);
}

SDL::Surface* Graphics::LoadBMP(char* file) { return this->LoadBMP(file, 0); }

SDL::Surface* Graphics::LoadBMP(char* file, int flags) {
	SDL_Surface* bmp = 0;
	bmp = SDL_LoadBMP(file);
	if (bmp == 0) {
		std::cout << SDL_GetError() << std::endl;
		return 0;
	}
	return new SDL::Surface(bmp);
}

bool Graphics::SetColorKey(SDL::Surface* surf, Pixel rgb) {
	SDL_SetColorKey(surf->Get(), SDL_TRUE, SDL_MapRGB(surf->Get()->format, (rgb & 0xFF), ((rgb >> 8) & 0xFF), ((rgb >> 16) & 0xFF)));
	// TODO: set color key
	return true;
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
	static int frames = 0;
	if (autoquit_frames > 0 && ++frames == autoquit_frames) app_killed = true;
}

void Graphics::FlipV() { Flip(); }
