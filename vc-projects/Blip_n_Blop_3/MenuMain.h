
#ifndef _MenuMain_
#define _MenuMain_

#include <ddraw.h>

#define RET_CONTINUE	0
#define RET_START_GAME1	1
#define RET_HISCORES	2
#define RET_EXIT		3
#define RET_CREDITS		4
#define RET_START_GAME2	5

class MenuMain {
public:
	int		current_menu;		// Num�ro du menu courant
	int		focus;				// Num�ro du menu EN ROUGE
	int		nb_focus;			// Nombre de choix du menu actuel
	char ** menu_txt;
	int		redefine;			// Num�ro du schnuff � red�finir
	int		old_menu;
	bool	up;

	RECT	rec;

	bool	start_sound_on;		// Ces deux bool�ens permettent de savoir si on a chang�
	bool	start_music_on;		// l'�tat de sound_on/music_on pour recharger le tout � la fin du menu

	MenuMain();
	void start();
	int update();	// Retour 0=toujours menu  1=retour jeu  2=quitter
	void stop();
	void updateName();
	void updateRedefine();
	void draw(IDirectDrawSurface7 * surf);
	~MenuMain();
};

#endif