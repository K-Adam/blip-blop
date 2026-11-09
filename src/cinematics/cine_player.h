#pragma once

#include <sstream>
#include <string>
#include "engine/music_bank.h"
#include "engine/picture_bank.h"
#include "engine/font.h"
#include "engine/chrono.h"
#include "core/task.h"
#include "engine/update_regulator.h"

#define OBJ_SPRITE 0
#define OBJ_SCROLL 1
#define OBJ_TEXT 2

#define NB_OBJ 20
#define NB_ARG 20

struct OBJECT {
	int id;
	bool show;

	const Picture* pic;
	int x;
	int y;

	int dx;
	int dy;
	int time_mov;

	std::string anim_sheet;
	int nb_anim;
	int anim[20];
	int speed_anim;
	int wait_anim;
	bool loop_anim;
	int anim_step;

	bool x_warp;
	int x_warp1;
	int x_warp2;

	bool y_warp;
	int y_warp1;
	int y_warp2;

	int txt;
};

class CINEPlayer {
   protected:
	UpdateRegulator update_regulator_;

	SDL::Surface* back_surf;

	std::stringstream file_;

	PictureBank pbk;
	MusicBank mbk;

	OBJECT obj[NB_OBJ];

	std::string cmd_name;
	int nb_args;
	int int_arg[NB_ARG];
	std::string str_arg[NB_ARG];

	int line_num;
	bool finished;
	int frame_to_draw;

	int clip_x1;
	int clip_x2;
	int clip_y1;
	int clip_y2;

	int alpha;
	int delta_alpha;

	int delta_vol;

	bool getCommand();
	void initPlayer();
	void closePlayer();
	bool error();
	bool error(const char* er);

	void updateState();

	void drawScene();
	void drawSprite(int n);
	void drawText(int n);
	void renderLoop();
	void updateScene();

   public:
	void loadPBK(const char* f);
	Task<bool> playScene(const char* file, SDL::Surface* back, bool anyInputSkips = false);
};
