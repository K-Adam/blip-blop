#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include "core/debug.h"
#include "engine/sdl_surface.h"

class Graphics {
   public:
	void Init();
	void SetGfxMode(int width, int height, bool fullscreen);
	void ToggleFullscreen();
	void SetVSync(bool on);
	SDL::Surface* CreateSurface(int width, int height);
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
	bool fullscreen_ = false;
	bool targets_lost_ = false;
};

extern Graphics& graphics;
