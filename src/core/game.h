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
#include "level/world.h"

#include "level/weather_snow.h"
#include "level/weather_rain.h"

#define NB_RAINDROPS 500
#define NB_SNOWFLAKES 500

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
	World world_;
	HUD hud_;
	UpdateRegulator update_regulator_;
	PictureBank pbk_briefing;
	bool briefing = false;

	int nb_part = 0;
	std::string file_names[MAX_PART];
	int type_part[MAX_PART];
	int type_lvl[MAX_PART];

	bool players_dead = false;
	bool level_done = false;
	bool bonus_level = false;
	bool skipped = false;
	bool last_perfect1 = false;
	bool last_perfect2 = false;

	int nframe = 0;
	int timer_tick = 0;

	// Victory needs the camera offset >= vic_x, flags[vic_flag1] == vic_val1 and flags[vic_flag2] == vic_val2
	int vic_x = 0;
	int vic_flag1 = 0;
	int vic_val1 = 0;
	int vic_flag2 = 0;
	int vic_val2 = 0;

	int wait_for_victory = 0;  // Delays the end of the level after victory
	int wait_for_death = 0;
	int wait_cheat = 0;

	int xstart1 = 0;
	int ystart1 = 0;
	int xstart2 = 0;
	int ystart2 = 0;

	std::unique_ptr<Ball> player1;
	std::unique_ptr<Ball> player2;

	RPGPlayer rpg;

	GoArrow go_;
	int last_x_go_ = 0;

	// Enemies aim at it when no player is alive
	Character dummy_player_;
	size_t targeted_idx_ = 0;
	int attract_step_ = 0;

	bool show_fps = false;
	bool show_lists = false;

	int phi_deform = 0;

	WeatherSnow snow[NB_SNOWFLAKES];
	int next_snowflake = 0;

	WeatherRain rain[NB_RAINDROPS];
	int next_raindrop = 0;

	int fps_count = 0;
	int fps_current_count = 0;
	bool slow_phase = false;
	std::list<std::unique_ptr<Explosion>> list_impacts;
	std::list<std::unique_ptr<Event>> list_event_dormant;
	std::list<std::unique_ptr<Event>> list_event;
	std::list<Sprite*> list_weather;
	std::list<std::unique_ptr<Bubble>> list_bubbles;

   public:
	Game();
	~Game();

	// Controls of the players who joined, the balls point at them during a game
	SchemeController controllers_[2];

	Task<> playGame(int player_count, int player_idx);

	Task<bool> playLevel(const char* level_name, int type);

	bool loadLevel(const char* level_name);

	void releaseLevel();

	void updateAll();

	void drawAll(bool flip = true);

	Task<> gameLoop();

	void releaseGame();

	bool loadGame();
	bool loadList(const char* file_path);

	void spawnBubble(Sprite* s);

	template <class T>
	void UpdateCollection(const T& xs);
	void updateGoArrow();
	void updateWeather();
	void updateEvents();
	void updateDeformation();
	void updateBubbles();

	void updateLock();
	void updateHoldFire();
	void updateTargetedPlayer();
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
	void drawHUBhp(int x, int y, int hp);
	void drawTimer();

	void drawDebugInfos();

	void drawDeformation();

	void cleanLists();
	Task<> showPE(bool bonus, bool bonus_failed = false);
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
