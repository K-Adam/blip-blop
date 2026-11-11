#pragma once

#include "core/task.h"
#include "engine/scheme_controller.h"

#include <string>
#include <vector>

#include "engine/graphics.h"
#include "engine/picture.h"
#include "cinematics/rpg_player.h"
#include "entities/blip.h"
#include "engine/chrono.h"
#include "engine/update_regulator.h"
#include "entities/go_arrow.h"
#include "level/hud.h"
#include "level/effects.h"
#include "level/world.h"

#define PART_CINE 0
#define PART_LEVEL 1
#define PART_BRIEFING 2

#define LVL_BONUS 0
#define LVL_FIRST 1
#define LVL_INTER 2
#define LVL_LAST 3
#define LVL_COMPLETE 4
#define LVL_END 5

class Event;
class Explosion;

class Game {
   protected:
	World world_;
	HUD hud_;
	Effects effects_;
	UpdateRegulator update_regulator_;
	PictureBank pbk_briefing;
	bool briefing = false;

	struct Part {
		int type;
		int lvl;
		std::string file;
	};
	std::vector<Part> parts_;

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

	int fps_count = 0;
	int fps_current_count = 0;
	bool slow_phase = false;
	std::list<std::unique_ptr<Explosion>> list_impacts;
	std::list<std::unique_ptr<Event>> list_event_dormant;
	std::list<std::unique_ptr<Event>> list_event;

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

	template <class T>
	void UpdateCollection(const T& xs);
	void updateGoArrow();
	void updateEvents();

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

	void cleanLists();
	Task<> showPE(bool bonus, bool bonus_failed = false);
	void drawLoading();
	Task<> getHiscore();
	Task<> showGameOver();
	Task<> showHighScores();

	void showBriefing(const char* fn);

	Task<> showCredits(bool theEnd = false);

	Task<> go();
};
