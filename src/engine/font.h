#pragma once

#include <vector>
#include <memory>
#include <string>
#include <string_view>

#include "engine/picture.h"
#include "engine/graphics.h"

class Font {
   private:
	std::unique_ptr<SDL::Surface> atlas_;
	std::vector<std::unique_ptr<Picture>> pictab_;	// Null for characters the font lacks
	int spc = 0;									// Width of a space
	int h = 0;										// Line height

   public:
	bool load(const char* f);

	void print(SDL::Surface* surf, int x, int y, std::string_view txt);

	void printR(SDL::Surface* surf, int x, int y, std::string_view txt);

	void printC(SDL::Surface* surf, int xcenter, int y, std::string_view txt);

	void printM(SDL::Surface* surf, int x, int y, std::string_view txt, int ym);

	void printMW(SDL::Surface* surf, int x, int y, std::string_view txt, int ym);

	int width(std::string_view txt);
};
