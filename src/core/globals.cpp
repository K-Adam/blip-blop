#include <vector>
#include <list>
#include <memory>

#include "core/misc.h"
#include "engine/picture_bank.h"
#include "engine/font.h"
#include "engine/sound_bank_bb.h"
#include "engine/music_bank.h"
#include "level/scroll.h"
#include "level/backgrounds/background_static.h"
#include "entities/shots/shot_bb_cow.h"
#include "entities/explosion.h"
#include "entities/vehicle.h"
#include "level/events/event.h"
#include "entities/enemies/enemy.h"
#include "level/enemy_generator.h"
#include "entities/bonus.h"
#include "level/bonus_generator.h"
#include "entities/sprite.h"
#include "entities/cool_text.h"
#include "entities/splatter.h"
#include "entities/bubble.h"

#include "core/debug.h"

#define NB_MAX_PLAT 8

int fps_count = 0;
int fps_current_count = 0;

SDL::Surface* primSurface = NULL;	 // Surface primaire
SDL::Surface* backSurface = NULL;	 // pointeur sur le double buffer supposé
SDL::Surface* systemSurface = NULL;	 // Pour optimiser les accès SYSTEM/SYSTEM
SDL::Surface* videoA = NULL;		 // cache video

int offset;		 // offset courant du scroll
int scr_offset;	 // = offset % 640

int scroll_speed;

bool scroll_locked;
int x_lock;
int cond_end_lock;
int flag_end_lock;
int val_end_lock;

bool no_scroll1;
bool no_scroll2;

size_t scr_level_size;		 // taille en écran du niveau
size_t level_size;			 // taille en pixels du niveau
std::vector<int> num_decor;	 // Numéro ecran courant -> surface à utiliser

std::vector<std::vector<int32_t>> y_plat;  // Plateformes (ordonnées)

std::vector<std::vector<char>> murs_opaques;	// Murs opaques (=true)
std::vector<std::vector<char>> murs_sanglants;	// Murs sanglants (=true)

std::vector<Ball*> list_joueurs;  // FIXME: make it owning?
std::list<ShotBB*> list_tirs_bb;
std::list<std::unique_ptr<ShotBBCow>> list_cow;
std::list<std::unique_ptr<Explosion>> list_impacts;

std::list<std::unique_ptr<Vehicle>> list_vehicules;

std::list<std::unique_ptr<Event>> list_event_endormis;
std::list<std::unique_ptr<Event>> list_event;

std::list<std::unique_ptr<Enemy>> list_ennemis;
std::list<std::unique_ptr<Shot>> list_tirs_ennemis;
std::list<std::unique_ptr<EnemyGenerator>> list_gen_ennemis;

std::list<std::unique_ptr<Bonus>> list_bonus;
std::list<std::unique_ptr<BonusGenerator>> list_gen_bonus;

std::list<std::unique_ptr<Sprite>> list_fonds_animes;
std::list<std::unique_ptr<Sprite>> list_fonds_statiques;
std::list<std::unique_ptr<Sprite>> list_premiers_plans;
std::list<std::unique_ptr<Sprite>> list_plateformes_mobiles;

std::list<std::unique_ptr<CoolText>> list_txt_cool;

std::list<std::unique_ptr<Sprite>> list_giclures;
std::list<std::unique_ptr<Sprite>> list_gore;

std::list<Sprite*> list_meteo;
std::list<std::unique_ptr<Bubble>> list_bulles;

bool phase;	 // Pour les clignotements
bool slow_phase;

int game_flag[11];

bool hold_fire;
int flag_hold_fire;
int val_hold_fire;

int rpg_to_play = -1;  // Le numéro de la phase RPG à actionner (-1:aucune)

int nb_ennemis_created;

int intensite_meteo;
int type_meteo;

int dy_tremblement = 0;
int etape_tremblement = 0;
int amplitude_tremblement = 0;
int ddy_tremblement = 0;

bool okBonus;
bool cowBombOn;
bool okLanceFlame;

PictureBank pbk_decor;	// pbk des décors
PictureBank pbk_blip;	// pbk de blip
PictureBank pbk_blop;
PictureBank pbk_bb;		  // pbk commune à blip et blop
PictureBank pbk_misc;	  // pbk globale du jeu (bonus & co)
PictureBank pbk_ennemis;  // pbk des ennemis
PictureBank pbk_niveau;	  // pbk du niveau (fonds animes & co)
PictureBank pbk_rpg;	  // Gueules des RPGs du niveau
PictureBank pbk_rpg_bb;	  // Les gueules de Blip et Blop
PictureBank pbk_inter;	  // L'interface

SoundBankBB sbk_bb;	  // Sons BB/divers communs à tous les niveaux
SoundBankBB sbk_rpg;  // Les musiques des scènes RPG
SoundBankBB sbk_misc;
SoundBankBB sbk_niveau;

MusicBank mbk_niveau;
MusicBank mbk_inter;
MusicBank mbk_interl;

std::string current_mbk;
int current_zik;

Font fnt_score_blip;
Font fnt_score_blop;
Font fnt_ammo;
Font fnt_ammo_used;
Font fnt_cool;
Font fnt_rpg;
Font fnt_menu;
Font fnt_menus;

void tremblement(int amp) {
	amplitude_tremblement = amp;
	ddy_tremblement = (amp >> 1) + 1;
	dy_tremblement = -amp;
	etape_tremblement = 0;
}

// Moving platforms can reach past the level edges
void set_plat(int n, int x, int y) {
	if (x >= 0 && size_t(x) < level_size) y_plat[n][x] = y;
}

int plat(int x, int y) {
	if (x < 0 || x >= (int)level_size || y >= 480) return 0;

	int tmp;

	for (int i = 0; i < NB_MAX_PLAT; i++) {
		tmp = y_plat[i][x];

		if (y >= tmp && y <= tmp + 10) return tmp;
	}
	return 0;
}

int plat2(int x, int y) {
	if (x < 0 || x >= (int)level_size || y < 0 || y >= 480) return -1;

	int tmp;

	for (int i = 0; i < NB_MAX_PLAT; i++) {
		tmp = y_plat[i][x];

		if (y >= tmp && y <= tmp + 10) return i;
	}

	return -1;
}

bool mur_opaque(int x, int y) {
	if (x < 0 || x >= (int)level_size || y < 0 || y >= 480) return false;

	return murs_opaques[y / 8][x / 8];
}

bool mur_sanglant(int x, int y) {
	if (x < 0 || x >= (int)level_size || y < 0 || y >= 480) return false;

	return murs_sanglants[y / 8][x / 8];
}

inline void clipedBlit(SDL::Surface* surf, const Picture* pic, int x, int y, Rect* clip) {
	if (pic == NULL) return;

	Rect r;
	int x1;
	int y1;
	int x2;
	int y2;
	int xs = pic->xSize();
	int ys = pic->ySize();

	x1 = x - pic->xSpot();
	y1 = y - pic->ySpot();

	x2 = x1 + xs;
	y2 = y1 + ys;

	if (x2 < clip->left || x1 > clip->right || y1 > clip->bottom || y2 < clip->top) return;

	if (x1 < clip->left) {
		r.left = clip->left - x1;
		x1 = clip->left;
	} else
		r.left = 0;

	if (y1 < clip->top) {
		r.top = clip->top - y1;
		y1 = clip->top;
	} else
		r.top = 0;

	if (x2 > clip->right)
		r.right = xs - (x2 - clip->right);
	else
		r.right = xs;

	if (y2 > clip->bottom)
		r.bottom = ys - (y2 - clip->bottom);
	else
		r.bottom = ys;

	surf->BltFast(x1, y1, pic->Surf(), &r, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

bool grave(int x, int y, Picture* pic) {
	if (pic == NULL || x > (offset + vbuffer_wide - pic->xSize())) return false;

	BackgroundStatic s;

	s.x = x;
	s.y = y;
	s.pic = pic;

	s.colFromPic();

	for (auto& s2 : list_fonds_animes) {
		if (s.collision(s2.get())) return false;
	}

	for (auto& s2 : list_plateformes_mobiles) {
		if (s.collision(s2.get())) return false;
	}

	Rect r;
	int ni = x / /*640;*/ vbuffer_wide;

	r.top = 0;
	r.bottom = 480;

	x %= vbuffer_wide;

	if (ni == n_cache - 1) {
		r.left = next_x;
		r.right = vbuffer_wide;

		clipedBlit(videoA, pic, x, y, &r);

		r.left = 0;
		r.right = next_x;

		clipedBlit(videoA, pic, x - vbuffer_wide, y, &r);

		return true;
	} else if (ni == n_cache) {
		r.left = 0;
		r.right = next_x;

		clipedBlit(videoA, pic, x, y, &r);

		r.left = next_x;
		r.right = vbuffer_wide;

		clipedBlit(videoA, pic, x + vbuffer_wide, y, &r);

		return true;
	}

	return false;
}
