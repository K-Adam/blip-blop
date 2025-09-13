#include <fstream>
#include "cinematics/rpg_player.h"
#include "engine/input.h"
#include "level/world.h"
#include "core/assets.h"
#include "core/text_data.h"
#include "entities/ball.h"
#include "level/scroll.h"
#include "engine/half_tone.h"
#include <json.h>
#include "core/asset_path.h"

using json = nlohmann::json;

RPGPlayer::RPGPlayer() : focus(0), key_released(false), skiped(false) {
	pic_tab[0] = NULL;
	pic_tab[1] = NULL;

	nimage[0] = 0;
	nimage[1] = 0;

	ntxt[0] = 0;
	ntxt[1] = 0;
}

void RPGPlayer::attachFile(const char* f) { fic_name_ = f; }

bool RPGPlayer::startPlay(int n) {
	auto dir = asset_path_prefix("rpg", fic_name_.c_str());
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "RPGPlayer::startPlay() -> Cannot open " << dir + ".json"
			  << "\n";
		return false;
	}

	json data = json::parse(input);

	// The parser reads the old text format, so the JSON script is rebuilt as that text
	fic_ = std::stringstream{};

	auto blocks = data["blocks"];
	for (auto block_it = blocks.begin(); block_it != blocks.end(); ++block_it) {
		auto block = *block_it;

		std::string name = block["name"];

		fic_ << ";"
			 << "\n";

		if (block.find("flagged") != block.end() && block["flagged"]) {
			fic_ << ". " << name << "\n";
		} else {
			fic_ << "; " << name << "\n";
		}

		fic_ << ";"
			 << "\n";

		auto items = block["items"];
		for (auto item : items) {
			std::string key = item[0];

			if (item.size() == 2) {
				std::string value = remove_quotes(item[1].dump());
				fic_ << key << "=" << value << "\n";
			} else {
				fic_ << key << "\n";
			}
		}

		fic_ << "stop";

		if ((block_it + 1) != blocks.end()) {
			fic_ << "\n";
		}
	}

	buffer1_ = "rpg=" + std::to_string(n);
	std::getline(fic_, buffer2_);

	while (!fic_.eof() && buffer1_ != buffer2_) std::getline(fic_, buffer2_);

	if (fic_.eof()) {
		debug << "RPGPlayer::startPlay() -> Ne trouve pas la scene " << n << "\n";
		return false;
	}

	nimage[0] = -1;
	nimage[1] = -1;
	ntxt[0] = -1;
	ntxt[1] = -1;

	nbjoueurs = list_players.size();
	key_released = false;
	skiped = false;
	focus = 0;
	cur_joueur = 0;
	wait_.Reset(0);

	int i = 0;

	for (Ball* c : list_players) {
		if (c->id_ball == ID_BLIP)
			base_joueur[i] = 0;
		else
			base_joueur[i] = 6;

		i += 1;
	}

	return true;
}

void RPGPlayer::stopPlay() { fic_ = std::stringstream{}; }

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

	if (pic_tab[0] != NULL && nimage[0] >= 0 && (size_t)nimage[0] < (*pic_tab[0]).getSize()) (*pic_tab[0])[nimage[0]]->PasteTo(surf, 0, 120);

	if (vbuffer_wide > 640) {
		if (pic_tab[1] != NULL && nimage[1] >= 0 && (size_t)nimage[1] < (*pic_tab[1]).getSize()) (*pic_tab[1])[nimage[1]]->PasteTo(surf, 540, 300);
	} else {
		if (pic_tab[1] != NULL && nimage[1] >= 0 && (size_t)nimage[1] < (*pic_tab[1]).getSize()) (*pic_tab[1])[nimage[1]]->PasteTo(surf, 538, 300);
	}

	if (ntxt[0] != -1) fnt_rpg.printMW(surf, 120, 135, txt_data[ntxt[0]], 640);

	if (ntxt[1] != -1) fnt_rpg.printMW(surf, 20, 315, txt_data[ntxt[1]], 535);

	return not_finished;
}

bool RPGPlayer::updateScene() {
	bool ready_to_draw = false;

	while (!ready_to_draw && read() && buffer1_ != "stop") {
		if (buffer1_ == "endif") {
		} else if (buffer1_ == "bbswap") {
			cur_joueur += 1;
			cur_joueur %= nbjoueurs;
		} else {
			size_t pos = buffer1_.find('=');
			buffer2_ = buffer1_.substr(pos + 1);
			buffer1_ = buffer1_.substr(0, pos);

			if (buffer1_ == "ifnbj") {
				if (nbjoueurs != std::stoi(buffer2_)) {
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
					pic_tab[focus] = &pbk_rpg_bb;
					id[focus] = ID_JOUEUR;
				} else if (buffer2_ == "ennemi") {
					pic_tab[focus] = &pbk_rpg;
					id[focus] = ID_ENNEMI;
				} else
					error(buffer2_);
			} else if (buffer1_ == "img") {
				if (id[focus] == ID_JOUEUR)
					nimage[focus] = std::stoi(buffer2_) + base_joueur[cur_joueur];
				else
					nimage[focus] = std::stoi(buffer2_);
			} else if (buffer1_ == "txt") {
				ntxt[focus] = std::stoi(buffer2_);
			} else if (buffer1_ == "affiche") {
				time_.Reset();
				wait_.Reset(std::stoi(buffer2_));
				ready_to_draw = true;
			} else if (buffer1_ == "flag") {
				size_t pos = buffer2_.find('.');
				int val = std::stoi(buffer2_.substr(pos + 1));
				buffer2_ = buffer2_.substr(0, pos);

				game_flag[std::stoi(buffer2_)] = val;
			} else {
				error(buffer1_);
			}
		}
	}

	return ready_to_draw;
}

bool RPGPlayer::read() {
	std::getline(fic_, buffer1_);

	while (!fic_.eof() && buffer1_[0] == ';') std::getline(fic_, buffer1_);

	return !fic_.eof();
}

void RPGPlayer::error(const std::string& err_msg) { debug << "RPGPlayer::updateScene() -> Erreur de syntaxe :'" << err_msg << "'\n"; }
