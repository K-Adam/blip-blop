#pragma once

#include <list>
#include <memory>
#include <vector>

#include "core/debug.h"
#include "engine/graphics.h"
#include "engine/picture.h"

#define LATENCE_GRAVITE 7
#define GRAVITE_MAX 4

#define NB_MAX_PLAT 8

#define METEO_PLUIE 0
#define METEO_NEIGE 1
#define METEO_DEFORME 2

#define FLAG_USER0 0
#define FLAG_USER1 1
#define FLAG_USER2 2
#define FLAG_USER3 3

#define FLAG_BULLES 4
#define FLAG_BONUS 5
#define FLAG_TIMER 6
#define FLAG_GEN_OFF 7
#define FLAG_NB_KILL 8
#define FLAG_NB_ENN 9
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
extern bool okLanceFlame;

extern size_t scr_level_size;		// In screens
extern size_t level_size;			// In pixels
extern std::vector<int> num_decor;	// Decor surface of each screen

extern std::vector<std::vector<int32_t>> y_plat;  // Platform heights

extern std::vector<std::vector<char>> murs_opaques;
extern std::vector<std::vector<char>> murs_sanglants;

class Ball;
extern std::vector<Ball*> list_joueurs;	 // FIXME: make it owning?
// Lists rather than vectors because entries get appended while iterating
// and must not relocate
class ShotBB;
extern std::list<ShotBB*> list_tirs_bb;
class ShotBBCow;
extern std::list<std::unique_ptr<ShotBBCow>> list_cow;

class Vehicle;
extern std::list<std::unique_ptr<Vehicle>> list_vehicules;

class Enemy;
extern std::list<std::unique_ptr<Enemy>> list_ennemis;
class Shot;
extern std::list<std::unique_ptr<Shot>> list_tirs_ennemis;
class EnemyGenerator;
extern std::list<std::unique_ptr<EnemyGenerator>> list_gen_ennemis;

class Bonus;
extern std::list<std::unique_ptr<Bonus>> list_bonus;
class BonusGenerator;
extern std::list<std::unique_ptr<BonusGenerator>> list_gen_bonus;

class Sprite;
extern std::list<std::unique_ptr<Sprite>> list_fonds_animes;
extern std::list<std::unique_ptr<Sprite>> list_fonds_statiques;
extern std::list<std::unique_ptr<Sprite>> list_premiers_plans;
extern std::list<std::unique_ptr<Sprite>> list_plateformes_mobiles;

class CoolText;
extern std::list<std::unique_ptr<CoolText>> list_txt_cool;

// FIXME: should be a list of Splatter once GoreSplatter inherits it
extern std::list<std::unique_ptr<Sprite>> list_giclures;
// FIXME: list_gore entries share no base below Sprite
extern std::list<std::unique_ptr<Sprite>> list_gore;

extern bool phase;	// Drives blinking

extern int game_flag[11];

extern bool hold_fire;
extern int flag_hold_fire;
extern int val_hold_fire;

extern int rpg_to_play;	 // -1 means none

extern int nb_ennemis_created;

extern int intensite_meteo;
extern int type_meteo;

void set_plat(int n, int x, int y);
int plat(int x, int y);	 // 0 when there is no platform
int plat2(int x, int y);
bool mur_opaque(int x, int y);
bool mur_sanglant(int x, int y);
bool grave(int x, int y, Picture* pic);

inline void draw(int x, int y, const Picture* pic) {
	if (pic != NULL) pic->BlitTo(backSurface, x - offset, y);
}

inline void drawRotated(int x, int y, const Picture* pic, double deg) {
	if (pic != NULL) pic->BlitRotated(backSurface, x - offset, y, deg);
}
