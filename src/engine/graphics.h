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
	// Opaque black, like a cleared screen
	SDL::Surface* CreateSurface(int width, int height);
	void Flip();

   private:
	struct WindowDeleter {
		void operator()(SDL_Window* ptr) { SDL_DestroyWindow(ptr); }
	};
	struct RendererDeleter {
		void operator()(SDL_Renderer* ptr) { SDL_DestroyRenderer(ptr); }
	};
	struct TextureDeleter {
		void operator()(SDL_Texture* ptr) { SDL_DestroyTexture(ptr); }
	};

	std::unique_ptr<SDL_Window, WindowDeleter> window_;
	std::unique_ptr<SDL_Renderer, RendererDeleter> renderer_;
	std::unique_ptr<SDL_Texture, TextureDeleter> screen_;
	bool fullscreen_ = false;
};

extern Graphics graphics;
