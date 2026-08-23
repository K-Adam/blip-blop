#pragma once

#include <cmath>
#include <list>
#include <memory>
#include <numbers>
#include <vector>

#include "core/debug.h"
#include "engine/graphics.h"
#include "engine/picture.h"

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

extern int offset;

extern int scroll_speed;

extern bool scroll_locked;
extern int x_lock;
extern int cond_end_lock;
extern int flag_end_lock;
extern int val_end_lock;

extern bool no_scroll1;	 // Stops the scroll while that player is dead
extern bool no_scroll2;

extern bool okBonus;
extern bool cowBombOn;
extern bool okFlamethrower;

extern size_t scr_level_size;		// In screens
extern size_t level_size;			// In pixels
extern std::vector<int> num_decor;	// Decor surface of each screen

extern std::vector<std::vector<int32_t>> y_platform;  // Platform heights

extern std::vector<std::vector<char>> solid_walls;
extern std::vector<std::vector<char>> bloody_walls;

class Ball;
// Not owning, Game::player1 and player2 own them. Dead players leave the list and come back after a bonus level
extern std::vector<Ball*> list_players;
// Lists rather than vectors because entries get appended while iterating
// and must not relocate
class ShotBB;
extern std::list<ShotBB*> list_bb_shots;
class ShotBBCow;
extern std::list<std::unique_ptr<ShotBBCow>> list_cow;

class Vehicle;
extern std::list<std::unique_ptr<Vehicle>> list_vehicles;

class Enemy;
extern std::list<std::unique_ptr<Enemy>> list_enemies;
class Shot;
extern std::list<std::unique_ptr<Shot>> list_enemy_shots;
class EnemyGenerator;
extern std::list<std::unique_ptr<EnemyGenerator>> list_enemy_gens;

class Bonus;
extern std::list<std::unique_ptr<Bonus>> list_bonus;
class BonusGenerator;
extern std::list<std::unique_ptr<BonusGenerator>> list_gen_bonus;

class Sprite;
extern std::list<std::unique_ptr<Sprite>> list_animated_bgs;
extern std::list<std::unique_ptr<Sprite>> list_static_bgs;
extern std::list<std::unique_ptr<Sprite>> list_foregrounds;
extern std::list<std::unique_ptr<Sprite>> list_moving_platforms;

class CoolText;
extern std::list<std::unique_ptr<CoolText>> list_txt_cool;

// Short lived debris such as blood, body parts and rings
extern std::list<std::unique_ptr<Sprite>> list_splatters;
extern std::list<std::unique_ptr<Sprite>> list_gore;

extern bool phase;	// Drives blinking

extern int game_flag[11];

extern bool hold_fire;
extern int flag_hold_fire;
extern int val_hold_fire;

extern int rpg_to_play;	 // -1 means none

extern int nb_enemies_created;

extern int weather_intensity;
extern int weather_type;

void set_platform(int n, int x, int y);
int platform_y(int x, int y);  // 0 when there is no platform
int platform_y2(int x, int y);
bool solid_wall(int x, int y);
bool bloody_wall(int x, int y);
bool engrave(int x, int y, Picture* pic);

inline void draw(int x, int y, const Picture* pic) {
	if (pic != NULL) pic->BlitTo(backSurface, x - offset, y);
}

inline void drawRotated(int x, int y, const Picture* pic, double deg) {
	if (pic != NULL) pic->BlitRotated(backSurface, x - offset, y, deg);
}

// Counterclockwise as seen on screen, where y points down
inline void rotateAround(int& px, int& py, int gx, int gy, double deg) {
	double a = deg * std::numbers::pi / 180, c = std::cos(a), s = std::sin(a);
	int ox = px - gx, oy = py - gy;
	px = gx + (int)std::lround(ox * c + oy * s);
	py = gy + (int)std::lround(-ox * s + oy * c);
}
