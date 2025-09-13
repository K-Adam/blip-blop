#include <vector>
#include <list>
#include <memory>

#include "level/world.h"
#include "level/scroll.h"
#include "level/backgrounds/background_static.h"
#include "entities/shots/shot_bb_cow.h"
#include "entities/vehicle.h"
#include "entities/enemies/enemy.h"
#include "level/enemy_generator.h"
#include "entities/bonus.h"
#include "level/bonus_generator.h"
#include "entities/sprite.h"
#include "entities/cool_text.h"
#include "entities/splatter.h"

#include "core/debug.h"

int offset;

int scroll_speed;

bool scroll_locked;
int x_lock;
int cond_end_lock;
int flag_end_lock;
int val_end_lock;

bool no_scroll1;
bool no_scroll2;

size_t scr_level_size;
size_t level_size;
std::vector<int> num_decor;

std::vector<std::vector<int32_t>> y_platform;

std::vector<std::vector<char>> solid_walls;
std::vector<std::vector<char>> bloody_walls;

std::vector<Ball*> list_players;
std::list<ShotBB*> list_shots_bb;
std::list<std::unique_ptr<ShotBBCow>> list_cow;

std::list<std::unique_ptr<Vehicle>> list_vehicles;

std::list<std::unique_ptr<Enemy>> list_enemies;
std::list<std::unique_ptr<Shot>> list_enemy_shots;
std::list<std::unique_ptr<EnemyGenerator>> list_enemy_gens;

std::list<std::unique_ptr<Bonus>> list_bonus;
std::list<std::unique_ptr<BonusGenerator>> list_gen_bonus;

std::list<std::unique_ptr<Sprite>> list_animated_bgs;
std::list<std::unique_ptr<Sprite>> list_static_bgs;
std::list<std::unique_ptr<Sprite>> list_foregrounds;
std::list<std::unique_ptr<Sprite>> list_moving_platforms;

std::list<std::unique_ptr<CoolText>> list_txt_cool;

std::list<std::unique_ptr<Sprite>> list_splatters;
std::list<std::unique_ptr<Sprite>> list_gore;

bool phase;

int game_flag[11];

bool hold_fire;
int flag_hold_fire;
int val_hold_fire;

int rpg_to_play = -1;

int nb_enemies_created;

int weather_intensity;
int weather_type;

bool okBonus;
bool cowBombOn;
bool okFlamethrower;

// Moving platforms can reach past the level edges
void set_platform(int n, int x, int y) {
	if (x >= 0 && size_t(x) < level_size) y_platform[n][x] = y;
}

int platform_y(int x, int y) {
	if (x < 0 || x >= (int)level_size || y >= 480) return 0;

	int tmp;

	for (int i = 0; i < NB_MAX_PLATFORMS; i++) {
		tmp = y_platform[i][x];

		if (y >= tmp && y <= tmp + 10) return tmp;
	}
	return 0;
}

int platform_y2(int x, int y) {
	if (x < 0 || x >= (int)level_size || y < 0 || y >= 480) return -1;

	int tmp;

	for (int i = 0; i < NB_MAX_PLATFORMS; i++) {
		tmp = y_platform[i][x];

		if (y >= tmp && y <= tmp + 10) return i;
	}

	return -1;
}

bool solid_wall(int x, int y) {
	if (x < 0 || x >= (int)level_size || y < 0 || y >= 480) return false;

	return solid_walls[y / 8][x / 8];
}

bool bloody_wall(int x, int y) {
	if (x < 0 || x >= (int)level_size || y < 0 || y >= 480) return false;

	return bloody_walls[y / 8][x / 8];
}

inline void clippedBlit(SDL::Surface* surf, const Picture* pic, int x, int y, Rect* clip) {
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

	surf->BltFast(x1, y1, pic->Surf(), &r);
}

bool engrave(int x, int y, Picture* pic) {
	if (pic == NULL || x > (offset + vbuffer_wide - pic->xSize())) return false;

	BackgroundStatic s;

	s.x = x;
	s.y = y;
	s.pic = pic;

	s.colFromPic();

	for (auto& s2 : list_animated_bgs) {
		if (s.collision(s2.get())) return false;
	}

	for (auto& s2 : list_moving_platforms) {
		if (s.collision(s2.get())) return false;
	}

	Rect r;
	int ni = x / vbuffer_wide;

	r.top = 0;
	r.bottom = 480;

	x %= vbuffer_wide;

	if (ni == n_cache - 1) {
		r.left = next_x;
		r.right = vbuffer_wide;

		clippedBlit(videoA, pic, x, y, &r);

		r.left = 0;
		r.right = next_x;

		clippedBlit(videoA, pic, x - vbuffer_wide, y, &r);

		return true;
	} else if (ni == n_cache) {
		r.left = 0;
		r.right = next_x;

		clippedBlit(videoA, pic, x, y, &r);

		r.left = next_x;
		r.right = vbuffer_wide;

		clippedBlit(videoA, pic, x + vbuffer_wide, y, &r);

		return true;
	}

	return false;
}
