#pragma once

#include <string>

#include "engine/graphics.h"
#include "engine/dd_gfx.h"
#include "engine/picture.h"
#include "cinematics/rpg_player.h"
#include "entities/blip.h"
#include "engine/chrono.h"
#include "engine/update_regulator.h"
#include "entities/go_arrow.h"
#include "level/hud.h"

#include "level/weather_snow.h"
#include "level/weather_rain.h"

#define NB_GOUTTES 500
#define NB_FLOCONS 500

#define MAX_PART 50

#define PART_CINE 0
#define PART_LEVEL 1
#define PART_BRIEFING 2

#define LVL_BONUS 0
#define LVL_FIRST 1
#define LVL_INTER 2
#define LVL_LAST 3
#define LVL_COMPLETE 4
#define LVL_END 5

class Game {
   protected:
	HUD hud_;
	UpdateRegulator update_regulator_;
	PictureBank pbk_briefing;
	bool briefing;

	int nb_part;
	std::string fic_names[MAX_PART];
	int type_part[MAX_PART];
	int type_lvl[MAX_PART];

	bool joueurs_morts;	 // Explicite, isn't it ?
	bool niveau_fini;	 // Yeah!
	bool niveau_bonus;
	bool skipped;  // Jeu skippe
	bool last_perfect1;
	bool last_perfect2;

	int nframe;	 // Pour la synchronisation
	int etape_timer;

	int vic_x;		// Pour obtenir la victoire il faut avoir :
	int vic_flag1;	// offset >= vic_x
	int vic_val1;	// ET game_flag[vic_flag1] == vic_val1
	int vic_flag2;	// ET game_flag[vic_flag2] == vic_val2
	int vic_val2;

	int wait_for_victory;  // Pour que la partie ne s'achève pas immédiatement
	int wait_for_death;
	int wait_cheat;

	int xstart1;  // Coordonnées de départ des joueurs
	int ystart1;
	int xstart2;
	int ystart2;

	std::unique_ptr<Ball> player1;
	std::unique_ptr<Ball> player2;

	RPGPlayer rpg;

	GoArrow go_;
	int last_x_go_;

	bool show_fps;
	bool show_lists;

	int phi_deform;

	WeatherSnow neige[NB_FLOCONS];
	int next_flocon;

	WeatherRain pluie[NB_GOUTTES];
	int next_goutte;

   public:
	// Constructeur -> met tout à NULL
	Game();

	// jouePartie -> Faites chier la vache!
	void jouePartie(int nbj, int idj);

	// joueNiveau -> Balance les PEs! Retourne VRAI si les joueurs
	// passent le niveau avec succès
	bool joueNiveau(const char* nom_niveau, int type);

	// chargeNiveau -> Charge le niveau (appelé par joueNiveau)
	bool chargeNiveau(const char* nom_niveau);

	// releaseNiveau -> Permet de désallouer le schnuff
	void releaseNiveau();

	// updateAll -> fait tout avancer
	void updateAll();

	// drawAll -> affiche tout
	void drawAll(bool flip = true);

	// gameLoop -> let's burn!
	void gameLoop();

	void releasePartie();

	bool chargePartie();
	bool loadList(const char* fic);

	void creeBulle(Sprite* s);

	template <class T>
	void UpdateCollection(const T& xs);
	void updateFlecheGo();
	void updateMeteo();
	void updateEvents();
	void updateDeformation();
	void updateBulles();
	void updateTremblements();

	void updateLock();
	void updateHoldFire();
	void updateTeteTurc();
	void updateRPG();
	void updateFlags();
	void updateVictoryAndDefeat();
	void updateMenu();
	void updateCheat();
	void manageCollisions();

	template <class T>
	void RemoveDestroyed(T& xs);

	template <class T>
	void DrawCollection(const T& xs);
	void drawTremblements();
	void drawHUB();
	void drawHUBpv(int x, int y, int pv);
	void drawTimer();

	void drawDebugInfos();

	void drawDeformation();

	void cleanLists();
	void showPE(bool bonus, bool fuckOff = false);
	void drawLoading();
	void getHiscore();
	void getName(Player* joueur, int i);
	void showGameOver();
	void showHighScores();

	void showBriefing(const char* fn);

	void showCredits(bool theEnd = false);
	void showMainScreen();

	void go();
};
