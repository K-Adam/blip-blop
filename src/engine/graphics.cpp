#include "engine/graphics.h"

#include <vector>

#include "core/debug.h"
#include "engine/chrono.h"
#include "engine/errors.h"

// Never destroyed, global picture banks free their textures at exit and need the renderer alive
Graphics& graphics = *new Graphics;

void Graphics::Init() {
	if (SDL_Init((SDL_INIT_EVERYTHING & ~SDL_INIT_HAPTIC)) == -1) {
		throw std::runtime_error(std::string("Can't initialize SDL") + SDL_GetError());
	}
}

void Graphics::ToggleFullscreen() {
	fullscreen_ = !fullscreen_;
	SDL_SetWindowFullscreen(window_.get(), fullscreen_ ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void Graphics::SetGfxMode(int width, int height, bool fullscreen, bool vsync) {
	fullscreen_ = fullscreen;
	renderer_.reset();
#ifdef __EMSCRIPTEN__
	// The canvas keeps its 640x480 backing store and the page CSS scales it
	Uint32 mode = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
#else
	Uint32 mode = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;
#endif
	window_.reset(SDL_ErrWrap(SDL_CreateWindow("Blip&Blop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | mode)));

#ifdef _WIN32
	// Direct3D 9 drops render targets on every resize, fullscreen or vsync change
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");
	// Naming a driver turns batching off unless asked for
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
#endif
	Uint32 flags = SDL_RENDERER_TARGETTEXTURE | (vsync ? SDL_RENDERER_PRESENTVSYNC : 0);
	renderer_.reset(SDL_ErrWrap(SDL_CreateRenderer(window_.get(), -1, flags)));
	// The game draws a fixed 640x480 frame, the renderer scales it to the window and letterboxes the rest
	SDL_RenderSetLogicalSize(renderer_.get(), width, height);
}

void Graphics::SetVSync(bool on) { SDL_RenderSetVSync(renderer_.get(), on); }

SDL::Surface* Graphics::CreateSurface(int width, int height) { return new SDL::Surface(width, height); }

void Graphics::CreateScreens(int width, int height) {
	back_.reset(CreateSurface(width, height));
	spare_.reset(CreateSurface(width, height));
}

bool Graphics::TakeTargetsLost() {
	bool lost = targets_lost_;
	targets_lost_ = false;
	return lost;
}

static void PrintFrameHash(SDL::Surface* frame) {
	static std::uint32_t last = 0;
	std::uint32_t frames_played = FrameClock::frames();
	if (frames_played % 60 != 0 || frames_played == last) return;
	last = frames_played;
	std::vector<Uint8> pixels(frame->w() * frame->h() * 4);
	frame->ReadPixels(pixels.data());
	std::uint32_t h = 2166136261u;
	for (Uint8 b : pixels) h = (h ^ b) * 16777619u;
	debug << "frame " << frames_played << " hash " << std::hex << h << std::dec << "\n";
}

void Graphics::Flip() {
	SDL_Renderer* r = renderer_.get();
	SDL_SetRenderTarget(r, nullptr);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderClear(r);
	// The window shows the frame as opaque, whatever alpha the fills left in it
	SDL_SetTextureBlendMode(back_->Get(), SDL_BLENDMODE_NONE);
	SDL_RenderCopy(r, back_->Get(), nullptr, nullptr);
	SDL_SetTextureBlendMode(back_->Get(), SDL_BLENDMODE_BLEND);
	SDL_RenderPresent(r);
	if (frame_hash_) PrintFrameHash(back_.get());
}
