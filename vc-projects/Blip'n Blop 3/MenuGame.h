
#ifndef _MenuGame_
#define _MenuGame_

#include <ddraw.h>

class MenuGame
{
public:
	int		current_menu;		// Num�ro du menu courant
	int		focus;				// Num�ro du menu EN ROUGE
	int		nb_focus;			// Nombre de choix du menu actuel
	char ** menu_txt;
	int		redefine;			// Num�ro du schnuff � red�finir
	int		old_menu;

	RECT	rec;

	MenuGame();
	void start();
	int update();	// Retour 0=toujours menu  1=retour jeu  2=quitter
	void stop();
	void updateName();
	void updateRedefine();
	void draw( IDirectDrawSurface7 * surf);
	~MenuGame();
};

#endif