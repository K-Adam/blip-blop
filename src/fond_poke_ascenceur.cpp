
#include "fond_poke_ascenceur.h"

#include "couille.h"


FondPokeAscenceur::FondPokeAscenceur()
{
	pic = pbk_niveau[51];
	dy = 1;
}

void FondPokeAscenceur::update()
{
	if (game_flag[2] > 0) {
		int		xtmp;

		// Fait des allers/retour
		//
		if (y <= 96)
			dy = 1;
		else if (y >= 387)
			dy = -1;

		// Si un joueur est sur la plateforme, on le déplace
		//
                for (Couille* joueur : list_joueurs) {
			xtmp = joueur->x;

			if (xtmp > x && xtmp < x + pic->xSize() && plat(xtmp, joueur->y) == y + 20)
				joueur->y += dy;
		}

		// Déplace la plateforme
		//
		y += dy;

		for (int i = x; i < x + pic->xSize(); i++)
			y_plat[4][i] = y + 20;

		colFromPic();

		if (x < offset - 400)
			a_detruire = true;
	}
}
