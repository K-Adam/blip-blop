#pragma once

#include <vector>
#include <memory>
#include <string>
#include <string_view>

#include "picture.h"
#include "dd_gfx.h"

class /* Poele en */ Fonte {
   private:
	std::vector<std::unique_ptr<Picture>> pictab_;
	int spc;  // Largeur d'un espace
	int h;	  // Hauteur d'un saut de ligne

   public:
	bool load(const char* f);

	// Affiche du texte normal
	void print(SDL::Surface* surf, int x, int y, std::string_view txt);

	// Affiche texte justifié à droite
	void printR(SDL::Surface* surf, int x, int y, std::string_view txt);

	// Affiche texte centré
	void printC(SDL::Surface* surf, int xtaille, int y, std::string_view txt);

	// Affiche texte multiligne
	void printM(SDL::Surface* surf, int x, int y, std::string_view txt, int ym);

	// Affiche texte multiligne sans couper les mots
	void printMW(SDL::Surface* surf, int x, int y, std::string_view txt, int ym);

	// Renvoie la longueur en pixels d'un texte donné
	int width(std::string_view txt);
};
