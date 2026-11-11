#include <fstream>
#include "cinematics/rpg_player.h"
#include "engine/input.h"
#include "level/world.h"
#include "core/app.h"
#include "entities/ball.h"
#include "engine/half_tone.h"
#include <json.h>
#include "core/asset_path.h"

using json = nlohmann::json;

RPGPlayer::RPGPlayer() : focus(0), key_released(false), skiped(false) {
	pic_tab[0] = nullptr;
	pic_tab[1] = nullptr;

	nimage[0] = 0;
	nimage[1] = 0;

	ntxt[0] = 0;
	ntxt[1] = 0;
}

void RPGPlayer::attachFile(const char* f) { file_name_ = f; }

bool RPGPlayer::startPlay(int n) {
	auto dir = asset_path_prefix("rpg", file_name_.c_str());
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "RPGPlayer::startPlay() -> Cannot open " << dir + ".json"
			  << "\n";
		return false;
	}

	json data = json::parse(input);

	// The parser reads the old text format, so the JSON script is rebuilt as that text
	file_ = std::stringstream{};

	auto blocks = data["blocks"];
	for (auto block_it = blocks.begin(); block_it != blocks.end(); ++block_it) {
		auto block = *block_it;

		std::string name = block["name"];

		file_ << ";"
			  << "\n";

		if (block.find("flagged") != block.end() && block["flagged"]) {
			file_ << ". " << name << "\n";
		} else {
			file_ << "; " << name << "\n";
		}

		file_ << ";"
			  << "\n";

		auto items = block["items"];
		for (auto item : items) {
			std::string key = item[0];

			if (item.size() == 2) {
				std::string value = remove_quotes(item[1].dump());
				file_ << key << "=" << value << "\n";
			} else if (item.size() == 3) {
				file_ << key << "=" << item[1].get<int>() << "." << item[2].get<int>() << "\n";
			} else {
				file_ << key << "\n";
			}
		}

		file_ << "stop";

		if ((block_it + 1) != blocks.end()) {
			file_ << "\n";
		}
	}

	buffer1_ = "rpg=" + std::to_string(n);
	std::getline(file_, buffer2_);

	while (!file_.eof() && buffer1_ != buffer2_) std::getline(file_, buffer2_);

	if (file_.eof()) {
		debug << "RPGPlayer::startPlay() -> Cannot find scene " << n << "\n";
		return false;
	}

	nimage[0] = -1;
	nimage[1] = -1;
	ntxt[0] = -1;
	ntxt[1] = -1;

	nb_players = session().players.size();
	key_released = false;
	skiped = false;
	focus = 0;
	cur_player = 0;
	wait_.Reset(0);

	int i = 0;

	for (Ball* c : session().players) {
		if (c->id_ball == ID_BLIP)
			player_sheet[i] = "blip";
		else
			player_sheet[i] = "blop";

		i += 1;
	}

	return true;
}

void RPGPlayer::stopPlay() { file_ = std::stringstream{}; }

bool RPGPlayer::drawScene(SDL::Surface* surf) {
	bool not_finished = true;

	if (in.anyKeyPressed()) {
		// The caller waits for the release once the dialog is over
		if (in.scanKey(DIK_ESCAPE)) {
			while (updateScene())
				;

			return false;
		}

		if (key_released && (time_.elapsed() >= 750)) {
			wait_.Reset(0);
			key_released = false;
		}
	} else {
		key_released = true;
	}

	if (wait_.is_zero()) {
		not_finished = updateScene();
	}

	Rect r;

	if (nimage[0] >= 0) {
		r.left = 100;
		r.top = 120;
		r.right = 640;
		r.bottom = 220;

		halfTone(surf, &r);
	}

	if (nimage[1] >= 0) {
		r.left = 0;
		r.top = 300;
		r.right = 540;
		r.bottom = 400;

		halfTone(surf, &r);
	}

	if (const Picture* p = portrait(0)) p->PasteTo(surf, 0, 120);

	if (const Picture* p = portrait(1)) p->PasteTo(surf, 540, 300);

	if (ntxt[0] != -1) assets().fonts.rpg.printMW(surf, 120, 135, texts()[ntxt[0]], 640);

	if (ntxt[1] != -1) assets().fonts.rpg.printMW(surf, 20, 315, texts()[ntxt[1]], 535);

	return not_finished;
}

bool RPGPlayer::updateScene() {
	bool ready_to_draw = false;

	while (!ready_to_draw && read() && buffer1_ != "stop") {
		if (buffer1_ == "endif") {
		} else if (buffer1_ == "bbswap") {
			cur_player += 1;
			cur_player %= nb_players;
		} else {
			size_t pos = buffer1_.find('=');
			buffer2_ = buffer1_.substr(pos + 1);
			buffer1_ = buffer1_.substr(0, pos);

			if (buffer1_ == "ifnbj") {
				if (nb_players != std::stoul(buffer2_)) {
					while (read() && buffer1_ != "endif")
						;
				}
			} else if (buffer1_ == "focus") {
				if (buffer2_ == "haut")
					focus = 0;
				else if (buffer2_ == "bas")
					focus = 1;
				else
					error(buffer2_);
			} else if (buffer1_ == "id") {
				if (buffer2_ == "joueur") {
					pic_tab[focus] = &assets().pics.rpg_bb;
					sheet[focus] = player_sheet[cur_player];
					id[focus] = ID_PLAYER;
				} else if (buffer2_ == "ennemi") {
					pic_tab[focus] = &assets().pics.rpg;
					sheet[focus] = "portraits";
					id[focus] = ID_ENEMY;
				} else
					error(buffer2_);
			} else if (buffer1_ == "img") {
				// Read now, as a dialog may swap the speaking player after choosing it
				if (id[focus] == ID_PLAYER) sheet[focus] = player_sheet[cur_player];
				nimage[focus] = std::stoi(buffer2_);
			} else if (buffer1_ == "txt") {
				ntxt[focus] = std::stoi(buffer2_);
			} else if (buffer1_ == "affiche") {
				time_.Reset();
				wait_.Reset(std::stoi(buffer2_));
				ready_to_draw = true;
			} else if (buffer1_ == "flag") {
				size_t dot = buffer2_.find('.');
				int val = std::stoi(buffer2_.substr(dot + 1));
				buffer2_ = buffer2_.substr(0, dot);

				level().rules.flags[std::stoi(buffer2_)] = val;
			} else {
				error(buffer1_);
			}
		}
	}

	return ready_to_draw;
}

bool RPGPlayer::read() {
	std::getline(file_, buffer1_);

	while (!file_.eof() && buffer1_[0] == ';') std::getline(file_, buffer1_);

	return !file_.eof();
}

void RPGPlayer::error(const std::string& err_msg) { debug << "RPGPlayer::updateScene() -> Syntax error: '" << err_msg << "'\n"; }

const Picture* RPGPlayer::portrait(int n) const {
	if (pic_tab[n] == nullptr || nimage[n] < 0) return nullptr;
	Flipbook f = pic_tab[n]->anim(sheet[n]);
	return nimage[n] < f.size() ? f[nimage[n]] : nullptr;
}
