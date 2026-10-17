#pragma once

#include <sstream>
#include <string>

#include "engine/graphics.h"
#include "engine/picture_bank.h"
#include "engine/chrono.h"

#define ID_PLAYER 0
#define ID_ENEMY 1

class RPGPlayer {
   public:
	std::stringstream file_;
	std::string file_name_;
	std::string buffer1_;
	std::string buffer2_;

	PictureBank* pic_tab[2];
	int nimage[2];
	int ntxt[2];
	int id[2];
	int focus;

	Chrono time_;
	Countdown wait_;
	bool key_released;
	bool skiped;

	size_t nb_players;
	int cur_player;									 // Player the dialog currently refers to (0 or 1)
	const char* player_sheet[2] = {"blip", "blip"};	 // Blip or Blop
	const char* sheet[2] = {"portraits", "portraits"};

	RPGPlayer();

	void attachFile(const char* f);
	const Picture* portrait(int n) const;  // Null when nothing is shown on that side
	bool startPlay(int n);
	bool drawScene(SDL::Surface* surf);
	bool updateScene();
	void stopPlay();
	bool read();
	void error(const std::string& msg);
};
