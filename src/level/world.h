#pragma once

#include <cmath>
#include <list>
#include <memory>
#include <numbers>
#include <random>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "core/debug.h"
#include "engine/graphics.h"
#include "engine/picture.h"
#include "level/scroll.h"

#define GRAVITY_DELAY 7
#define GRAVITY_MAX 4

#define NB_MAX_PLATFORMS 8

#define WEATHER_RAIN 0
#define WEATHER_SNOW 1
#define WEATHER_DISTORT 2

#define FLAG_USER0 0
#define FLAG_USER1 1
#define FLAG_USER2 2
#define FLAG_USER3 3

#define FLAG_BUBBLES 4
#define FLAG_BONUS 5
#define FLAG_TIMER 6
#define FLAG_GEN_OFF 7
#define FLAG_NB_KILL 8
#define FLAG_NB_ENEMIES 9
#define FLAG_NB_GEN 10

#define NB_FLAGS 11

class Ball;
class Bonus;
class BonusGenerator;
class Character;
class CoolText;
class Enemy;
class EnemyGenerator;
class Shot;
class ShotBB;
class ShotBBCow;
class Sprite;
class Vehicle;
struct Entities;

struct Camera {
	int offset = 0;
	int speed = 0;	// Forced scroll, 0 follows the players

	// Holds the left edge at x until cond is met. The level files store cond as a number
	struct Lock {
		enum Until { NoEnemies, NoGenerators, FlagEquals, FlagAtLeast };
		bool on = false;
		int x = 0;
		int cond = NoEnemies;
		int flag = 0;
		int val = 0;

		bool released(const Entities& entities, const int* flags) const;
	} lock;

	bool frozen = false;  // A dying player holds the scroll

	void follow(const std::vector<Ball*>& players, bool forced, int level_width);
	void clamp(int level_width);
};

class Terrain {
   public:
	size_t screens = 0;
	size_t width = 0;							  // In pixels
	std::vector<int> decor;						  // Decor surface of each screen
	std::vector<std::vector<int32_t>> platforms;  // Height of each platform at each x
	std::vector<std::vector<char>> solid;		  // 8x8 cells
	std::vector<std::vector<char>> bloody;

	void set_platform(int n, int x, int y);
	int platform_y(int x, int y) const;	  // 0 when there is no platform
	int platform_y2(int x, int y) const;  // Index of the platform, -1 when none
	bool solid_wall(int x, int y) const;
	bool bloody_wall(int x, int y) const;
};

// Lists rather than vectors because entries get appended while iterating and must not relocate
struct Entities {
	std::list<ShotBB*> bb_shots;  // Not owning, the weapons pool them
	std::list<std::unique_ptr<ShotBBCow>> cows;
	std::list<std::unique_ptr<Vehicle>> vehicles;
	std::list<std::unique_ptr<Enemy>> enemies;
	std::list<std::unique_ptr<Shot>> enemy_shots;
	std::list<std::unique_ptr<EnemyGenerator>> enemy_gens;
	std::list<std::unique_ptr<Bonus>> bonus;
	std::list<std::unique_ptr<BonusGenerator>> bonus_gens;
	std::list<std::unique_ptr<Sprite>> animated_bgs;
	std::list<std::unique_ptr<Sprite>> static_bgs;
	std::list<std::unique_ptr<Sprite>> foregrounds;
	std::list<std::unique_ptr<Sprite>> moving_platforms;
	std::list<std::unique_ptr<CoolText>> cool_texts;
	// Short lived debris such as blood, body parts and rings
	std::list<std::unique_ptr<Sprite>> splatters;
	std::list<std::unique_ptr<Sprite>> gore;
};

struct LevelRules {
	int flags[NB_FLAGS] = {};

	// Players cannot fire until flags[flag] == val
	struct HoldFire {
		bool on = false;
		int flag = 0;
		int val = 0;

		bool released(const int* flags) const { return flags[flag] == val; }
	} hold_fire;

	bool bonus_ok = false;
	bool flamethrower_ok = true;
	int bonus_mode = 0;	 // Row of the bonus table MakeBonus draws from

	int weather_type = WEATHER_RAIN;
	int weather_intensity = 0;
};

// Everything that lives as long as one level
// State shared by every instance of one entity type, created on first use and gone with its owner
class SharedState {
   public:
	template <class T>
	T& shared() {
		auto& slot = shared_[typeid(T)];
		if (!slot) slot = std::make_shared<T>();
		return *static_cast<T*>(slot.get());
	}

   private:
	std::unordered_map<std::type_index, std::shared_ptr<void>> shared_;
};

class Level : public SharedState {
   public:
	Camera camera;
	Terrain terrain;
	Entities entities;
	LevelRules rules;

	Level();
	~Level();
};

// Everything that outlives a level
struct Session : SharedState {
	// Not owning, Game::player1 and player2 own them. Dead players leave the list and come back after a bonus level
	std::vector<Ball*> players;
	Character* targeted_player = nullptr;
	bool cow_bomb = false;
	int enemies_created = 0;
	int rpg_to_play = -1;  // -1 means none
	bool phase = false;	   // Flips every frame, drives blinking
};

// Owned by Game. Entities reach it through level() and session() so their update signatures stay free of it
class World {
   public:
	Session session;
	ScrollRenderer scroll;
	// Same sequence on every standard library, unlike rand()
	std::mt19937 rng;

	World();
	~World();
	World(const World&) = delete;
	World& operator=(const World&) = delete;

	Level& level() { return *level_; }
	void resetLevel();

	static World& current() { return *current_; }

   private:
	std::unique_ptr<Level> level_;
	inline static World* current_ = nullptr;
};

inline Level& level() { return World::current().level(); }
inline Session& session() { return World::current().session; }
// Shift keeps the result non-negative so rand() % n call sites stay valid
inline int rnd() { return static_cast<int>(World::current().rng() >> 1); }

bool engrave(int x, int y, const Picture* pic);

inline void draw(int x, int y, const Picture* pic) {
	if (pic != nullptr) pic->BlitTo(graphics.back(), x - level().camera.offset, y);
}

inline void drawRotated(int x, int y, const Picture* pic, double deg) {
	if (pic != nullptr) pic->BlitRotated(graphics.back(), x - level().camera.offset, y, deg);
}

// Counterclockwise as seen on screen, where y points down
inline void rotateAround(int& px, int& py, int gx, int gy, double deg) {
	double a = deg * std::numbers::pi / 180, c = std::cos(a), s = std::sin(a);
	int ox = px - gx, oy = py - gy;
	px = gx + static_cast<int>(std::lround(ox * c + oy * s));
	py = gy + static_cast<int>(std::lround(-ox * s + oy * c));
}
