#pragma once

#include <list>
#include <string>
#include <vector>

#include "core/debug.h"
#include "engine/picture_bank.h"
#include "engine/font.h"
#include "engine/sound_bank_bb.h"
#include "engine/music_bank.h"

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

extern int fps_count;
extern int fps_current_count;

extern SDL::Surface* backSurface;
extern SDL::Surface* systemSurface;
extern SDL::Surface* videoA;

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

// Lists rather than vectors because entries get appended while iterating
// and must not relocate
class Ball;
extern std::vector<Ball*> list_joueurs;	 // FIXME: make it owning?
class ShotBB;
extern std::list<ShotBB*> list_tirs_bb;
class ShotBBCow;
extern std::list<std::unique_ptr<ShotBBCow>> list_cow;
class Explosion;
// FIXME: should be owning by value but can't, because of circular deps
extern std::list<std::unique_ptr<Explosion>> list_impacts;

class Vehicle;
extern std::list<std::unique_ptr<Vehicle>> list_vehicules;

class Event;
extern std::list<std::unique_ptr<Event>> list_event_endormis;
extern std::list<std::unique_ptr<Event>> list_event;

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

extern std::list<Sprite*> list_meteo;
class Bubble;
extern std::list<std::unique_ptr<Bubble>> list_bulles;

extern bool phase;	// Drives blinking
extern bool slow_phase;

extern int game_flag[11];

extern bool hold_fire;
extern int flag_hold_fire;
extern int val_hold_fire;

extern int rpg_to_play;	 // -1 means none

extern int nb_ennemis_created;

extern int intensite_meteo;
extern int type_meteo;

extern PictureBank pbk_decor;
extern PictureBank pbk_blip;
extern PictureBank pbk_blop;
extern PictureBank pbk_bb;
extern PictureBank pbk_misc;
extern PictureBank pbk_ennemis;
extern PictureBank pbk_niveau;
extern PictureBank pbk_rpg;
extern PictureBank pbk_rpg_bb;
extern PictureBank pbk_inter;

extern SoundBankBB sbk_bb;
extern SoundBankBB sbk_misc;
extern SoundBankBB sbk_niveau;

extern MusicBank mbk_niveau;
extern MusicBank mbk_inter;
extern MusicBank mbk_interl;

extern std::string current_mbk;
extern int current_zik;

extern Font fnt_score_blip;
extern Font fnt_score_blop;
extern Font fnt_ammo;
extern Font fnt_ammo_used;
extern Font fnt_cool;
extern Font fnt_rpg;
extern Font fnt_menu;
extern Font fnt_menus;

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
