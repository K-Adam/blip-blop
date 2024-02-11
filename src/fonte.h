#pragma once

#include <vector>
#include <memory>
#include <string>

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
	void print(SDL::Surface* surf, int x, int y, const char* txt);

	// Affiche texte justifié à droite
	void printR(SDL::Surface* surf, int x, int y, const char* txt);

	// Affiche texte centré
	void printC(SDL::Surface* surf, int xtaille, int y, const char* txt);

	// Affiche texte multiligne
	void printM(SDL::Surface* surf, int x, int y, const char* txt, int ym);

	// Affiche texte multiligne sans couper les mots
	void printMW(SDL::Surface* surf, int x, int y, const char* txt, int ym);

	// Renvoie la longueur en pixels d'un texte donné
	int width(const char* txt);
};
