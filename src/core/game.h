#pragma once

#include "core/task.h"
#include "engine/scheme_controller.h"

#include <string>

#include "engine/graphics.h"
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

class Bubble;
class Event;
class Explosion;
class Sprite;

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

	bool joueurs_morts;
	bool niveau_fini;
	bool niveau_bonus;
	bool skipped;
	bool last_perfect1;
	bool last_perfect2;

	int nframe;
	int etape_timer;

	// Victory needs offset >= vic_x, game_flag[vic_flag1] == vic_val1 and game_flag[vic_flag2] == vic_val2
	int vic_x;
	int vic_flag1;
	int vic_val1;
	int vic_flag2;
	int vic_val2;

	int wait_for_victory;  // Delays the end of the level after victory
	int wait_for_death;
	int wait_cheat;

	int xstart1;
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

	int fps_count = 0;
	int fps_current_count = 0;
	bool slow_phase = false;
	std::list<std::unique_ptr<Explosion>> list_impacts;
	std::list<std::unique_ptr<Event>> list_event_endormis;
	std::list<std::unique_ptr<Event>> list_event;
	std::list<Sprite*> list_meteo;
	std::list<std::unique_ptr<Bubble>> list_bulles;

   public:
	Game();
	~Game();

	// Controls of the players who joined, the balls point at them during a game
	SchemeController controllers_[2];

	Task<> jouePartie(int nbj, int idj);

	Task<bool> joueNiveau(const char* nom_niveau, int type);

	bool chargeNiveau(const char* nom_niveau);

	void releaseNiveau();

	void updateAll();

	void drawAll(bool flip = true);

	Task<> gameLoop();

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

	void updateLock();
	void updateHoldFire();
	void updateTeteTurc();
	Task<> updateRPG();
	void updateFlags();
	void updateVictoryAndDefeat();
	Task<> updateMenu();
	void updateCheat();
	void manageCollisions();

	template <class T>
	void RemoveDestroyed(T& xs);

	template <class T>
	void DrawCollection(const T& xs);
	void drawHUB();
	void drawHUBpv(int x, int y, int hp);
	void drawTimer();

	void drawDebugInfos();

	void drawDeformation();

	void cleanLists();
	Task<> showPE(bool bonus, bool fuckOff = false);
	void drawLoading();
	Task<> getHiscore();
	Task<> getName(Player* hero, int i);
	Task<> showGameOver();
	Task<> showHighScores();

	void showBriefing(const char* fn);

	Task<> showCredits(bool theEnd = false);
	Task<> showMainScreen();

	Task<> go();
};
