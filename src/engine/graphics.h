#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include "core/debug.h"
#include "engine/sdl_surface.h"

class Graphics {
   public:
	void Init();
	void SetGfxMode(int width, int height, bool fullscreen, bool vsync);
	void ToggleFullscreen();
	void SetVSync(bool on);
	std::unique_ptr<SDL::Surface> CreateSurface(int width, int height);
	void CreateScreens(int width, int height);
	// The frame being drawn
	SDL::Surface* back() const { return back_.get(); }
	// A full screen copy for effects drawn over a frozen frame
	SDL::Surface* spare() const { return spare_.get(); }
	// Tests compare the printed hashes to prove a refactor draws exactly the same frames
	void EnableFrameHash() { frame_hash_ = true; }
	void Flip();
	SDL_Renderer* renderer() const { return renderer_.get(); }

	// Some drivers wipe render targets, the scroll cache then has to be rebuilt
	void MarkTargetsLost() { targets_lost_ = true; }
	bool TakeTargetsLost();

   private:
	struct WindowDeleter {
		void operator()(SDL_Window* ptr) { SDL_DestroyWindow(ptr); }
	};
	struct RendererDeleter {
		void operator()(SDL_Renderer* ptr) { SDL_DestroyRenderer(ptr); }
	};

	std::unique_ptr<SDL_Window, WindowDeleter> window_;
	std::unique_ptr<SDL_Renderer, RendererDeleter> renderer_;
	std::unique_ptr<SDL::Surface> back_;
	std::unique_ptr<SDL::Surface> spare_;
	bool frame_hash_ = false;
	bool fullscreen_ = false;
	bool targets_lost_ = false;
};

extern Graphics& graphics;
