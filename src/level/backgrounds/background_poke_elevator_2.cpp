#include "level/backgrounds/background_poke_elevator_2.h"

#include "entities/ball.h"

BackgroundPokeElevator2::BackgroundPokeElevator2() {
	pic = pbk_niveau[51];
	dy = 1;
}

void BackgroundPokeElevator2::update() {
	if (game_flag[2] > 1) {
		int xtmp;

		if (y <= 96)
			dy = 1;
		else if (y >= 387)
			dy = -1;

		for (Ball* joueur : list_joueurs) {
			xtmp = joueur->x;

			if (xtmp > x && xtmp < x + pic->xSize() && plat(xtmp, joueur->y) == y + 20) joueur->y += dy;
		}

		y += dy;

		for (int i = x; i < x + pic->xSize(); i++) set_plat(4, i, y + 20);

		colFromPic();

		if (x < offset - 400) a_detruire = true;
	} else {
		y = -60;
	}
}
