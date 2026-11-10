#include <algorithm>
#include <cctype>
#include <charconv>
#include <string_view>
#include "cinematics/cine_player.h"
#include "core/debug.h"
#include "engine/input.h"
#include "core/text_data.h"
#include "core/assets.h"
#include "core/config.h"
#include <json.h>
#include "core/asset_path.h"

using json = nlohmann::json;

void CINEPlayer::initPlayer() {
	for (int i = 0; i < NB_OBJ; i++) {
		obj[i].show = false;
		obj[i].time_mov = 0;
		obj[i].nb_anim = 0;
		obj[i].x_warp = false;
		obj[i].y_warp = false;
	}

	line_num = 0;
	frame_to_draw = 0;
	finished = false;
	clip_x1 = 0;
	clip_y1 = 0;
	clip_x2 = 640;
	clip_y2 = 480;
	alpha = 0;
	delta_alpha = 0;
	delta_vol = 0;
}

Task<bool> CINEPlayer::playScene(const char* file, SDL::Surface* back, bool anyInputSkips) {
	bool skiped = false;
	initPlayer();

	back_surf = back;

	auto dir = asset_path_prefix("cin", file);
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "CINEPlayer -> Cannot open file " << dir << ".json"
			  << "\n";
		co_return false;
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
		file_ << "; " << name << "\n";
		file_ << ";"
			  << "\n";

		auto items = block["items"];
		for (auto item_it = items.begin(); item_it != items.end(); ++item_it) {
			auto item = *item_it;

			std::string name = item["name"];
			std::stringstream param_stream;

			bool first = true;
			for (auto param : item["params"]) {
				if (first) {
					first = false;
				} else {
					param_stream << ",";
				}
				param_stream << " " << param;
			}

			file_ << name << "(" << param_stream.str() << ")";

			if ((block_it + 1) != blocks.end() || (item_it + 1) != items.end()) {
				file_ << "\n";
			}
		}
	}

	while (!finished && !app_killed) {
		updateState();
		update_regulator_.Skip();

		while (frame_to_draw > 0 && !finished && !app_killed) {
			renderLoop();

			// Enter also reads a click or pad A, a fresh press so fire held from the level does not skip
			if (anyInputSkips ? in.anyNewPress() : in.scanKey(DIK_ESCAPE) || in.pressedKey(DIK_RETURN)) {
				finished = true;
				skiped = true;
			}

			co_await nextFrame();
		}
	}

	closePlayer();

	co_return !skiped;
}

void CINEPlayer::updateScene() {
	int vol = delta_vol;

	if (vol < 0) vol = 0;

	alpha += delta_alpha;

	if (alpha > 255)
		alpha = 255;
	else if (alpha < 0)
		alpha = 0;

	for (int i = 0; i < NB_OBJ; i++) {
		if (obj[i].show) {
			if (obj[i].time_mov > 0) {
				obj[i].x += obj[i].dx;
				obj[i].y += obj[i].dy;
				obj[i].time_mov -= 1;
			}

			if (obj[i].nb_anim > 0) {
				obj[i].wait_anim += 1;
				obj[i].wait_anim %= obj[i].speed_anim;

				obj[i].pic = pbk.anim(obj[i].anim_sheet)[obj[i].anim[obj[i].anim_step]];

				if (obj[i].wait_anim == 0) {
					obj[i].anim_step += 1;

					if (obj[i].anim_step >= obj[i].nb_anim) {
						if (obj[i].loop_anim) {
							obj[i].anim_step = 0;
							obj[i].pic = pbk.anim(obj[i].anim_sheet)[obj[i].anim[0]];
						} else {
							obj[i].nb_anim = 0;
						}
					}
				}
			}

			if (obj[i].x_warp) {
				if (obj[i].x == obj[i].x_warp1) obj[i].x = obj[i].x_warp2;
			}

			if (obj[i].y_warp) {
				if (obj[i].y == obj[i].y_warp1) obj[i].y = obj[i].y_warp2;
			}
		}
	}

	frame_to_draw -= 1;
}

void CINEPlayer::renderLoop() {
	int n_updates = update_regulator_.Step();
	for (; n_updates > 0; --n_updates) {
		updateScene();
	}

	drawScene();
}

void CINEPlayer::updateState() {
	bool to_draw = false;
	int n;
	int i;

	while (!finished && !to_draw && getCommand()) {
		if (cmd_name == "endscene") {
			finished = true;
		} else if (cmd_name == "loadgfx") {
			if (!pbk.loadGFX(str_arg[0].c_str())) error("cannot load GFX file");
		} else if (cmd_name == "loadmbk") {
			if (!mbk.open(str_arg[0].c_str())) error("cannot load MBK");
		} else if (cmd_name == "playmusic") {
			mbk.play(int_arg[0]);
		} else if (cmd_name == "stopmusic") {
			mbk.stop();
		} else if (cmd_name == "setpos") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else {
				obj[n].x = int_arg[1];
				obj[n].y = int_arg[2];
			}
		} else if (cmd_name == "setpic") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else {
				const Picture* pic = pbk.frame(str_arg[1]);

				if (pic == nullptr)
					error("invalid picture number");
				else {
					obj[n].id = OBJ_SPRITE;
					obj[n].pic = pic;
					obj[n].show = true;
				}
			}
		} else if (cmd_name == "setmove") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else {
				obj[n].dx = int_arg[1];
				obj[n].dy = int_arg[2];
				obj[n].time_mov = int_arg[3];
			}
		} else if (cmd_name == "settext") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else {
				obj[n].id = OBJ_TEXT;
				obj[n].txt = int_arg[1];
				obj[n].show = true;
			}
		} else if (cmd_name == "reset") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else {
				obj[n].show = false;
				obj[n].dx = 0;
				obj[n].dy = 0;
				obj[n].nb_anim = 0;
				obj[n].x_warp = false;
				obj[n].y_warp = false;
			}
		} else if (cmd_name == "resetall") {
			for (i = 0; i < NB_OBJ; i++) {
				obj[i].show = false;
				obj[i].dx = 0;
				obj[i].dy = 0;
				obj[i].nb_anim = 0;
				obj[i].x_warp = false;
				obj[i].y_warp = false;
			}
		} else if (cmd_name == "setclip") {
			clip_x1 = int_arg[0];
			clip_y1 = int_arg[1];
			clip_x2 = int_arg[2];
			clip_y2 = int_arg[3];
		} else if (cmd_name == "noclip") {
			clip_x1 = 0;
			clip_y1 = 0;
			clip_x2 = 640;
			clip_y2 = 480;
		} else if (cmd_name == "setanim") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else if (nb_args < 5)
				error("setanim needs at least one frame");
			else {
				obj[n].speed_anim = int_arg[1];
				if (int_arg[2])
					obj[n].loop_anim = true;
				else
					obj[n].loop_anim = false;

				obj[n].id = OBJ_SPRITE;
				obj[n].anim_sheet = str_arg[3];
				obj[n].wait_anim = 0;
				obj[n].anim_step = 0;
				obj[n].nb_anim = nb_args - 4;

				for (i = 4; i < nb_args; i++) {
					obj[n].anim[i - 4] = int_arg[i];
				}
				obj[n].pic = pbk.anim(obj[n].anim_sheet)[obj[n].anim[0]];
			}
		} else if (cmd_name == "setxwarp") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else {
				obj[n].x_warp = true;
				obj[n].x_warp1 = int_arg[1];
				obj[n].x_warp2 = int_arg[2];
			}
		} else if (cmd_name == "setywarp") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else {
				obj[n].y_warp = true;
				obj[n].y_warp1 = int_arg[1];
				obj[n].y_warp2 = int_arg[2];
			}
		} else if (cmd_name == "stopanim") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("invalid object number");
			else {
				obj[n].nb_anim = false;
			}
		} else if (cmd_name == "setalpha") {
			alpha = int_arg[0];
			delta_alpha = 0;
		} else if (cmd_name == "setdeltaalpha") {
			delta_alpha = int_arg[0];
		} else if (cmd_name == "setbackcolor") {
			if (int_arg[0] < 0 || int_arg[0] > 1) error("invalid layer number");
		} else if (cmd_name == "setclipcolor") {
			if (int_arg[0] < 0 || int_arg[0] > 1) error("invalid layer number");
		} else if (cmd_name == "setvolume") {
			delta_vol = 0;
		} else if (cmd_name == "setdeltavolume") {
			delta_vol = int_arg[0];
		} else if (cmd_name == "renderscene") {
			frame_to_draw = int_arg[0];
			to_draw = true;
		} else {
			error();
		}
	}
}

void CINEPlayer::drawScene() {
	Rect r;

	back_surf->FillRect(0, 0xFF000000);

	for (int i = 0; i < NB_OBJ; i++) {
		if (obj[i].show) {
			switch (obj[i].id) {
				case OBJ_SPRITE:
					drawSprite(i);
					break;
			}
		}
	}

	r.left = 0;
	r.right = clip_x1;
	r.top = 0;
	r.bottom = 480;
	back_surf->FillRect(&r, 0);

	r.left = clip_x2;
	r.right = 640;
	r.top = 0;
	r.bottom = 480;
	back_surf->FillRect(&r, 0);

	r.left = 0;
	r.right = 640;
	r.top = 0;
	r.bottom = clip_y1;
	back_surf->FillRect(&r, 0);

	r.left = 0;
	r.right = 640;
	r.top = clip_y2;
	r.bottom = 480;
	back_surf->FillRect(&r, 0);

	for (int i = 0; i < NB_OBJ; i++) {
		if (obj[i].show && obj[i].id == OBJ_TEXT) drawText(i);
	}

	graphics.Flip();
}

void CINEPlayer::drawSprite(int n) {
	if (n < NB_OBJ) obj[n].pic->BlitTo(back_surf, obj[n].x, obj[n].y);
}

void CINEPlayer::drawText(int n) {
	if (n < NB_OBJ) fnt_rpg.printC(back_surf, obj[n].x, obj[n].y, txt_data[obj[n].txt]);
}

bool CINEPlayer::getCommand() {
	std::string line;

	do {
		// A script without endscene would otherwise spin forever on EOF
		if (!std::getline(file_, line)) {
			finished = true;
			return false;
		}
		line_num += 1;
	} while (line.empty() || line[0] == ';');

	std::string clean;
	for (char c : line) {
		if (c == ';') break;
		if (c != ' ' && c != '"') clean += char(std::tolower((unsigned char)c));
	}

	size_t paren = clean.find('(');
	if (paren == std::string::npos || paren == 0) return error();
	cmd_name = clean.substr(0, paren);

	nb_args = 0;
	std::string_view args(clean);
	args.remove_prefix(paren + 1);
	while (!args.empty() && nb_args < NB_ARG) {
		size_t len = std::min(args.find_first_of(",)"), args.size());
		if (len > 0) {
			str_arg[nb_args] = std::string(args.substr(0, len));
			int_arg[nb_args] = 0;
			std::from_chars(args.data(), args.data() + len, int_arg[nb_args]);
			nb_args += 1;
		}
		args.remove_prefix(std::min(len + 1, args.size()));
	}

	return true;
}

void CINEPlayer::loadPBK(const char* f) { pbk.loadGFX(f); }

void CINEPlayer::closePlayer() { file_ = std::stringstream{}; }

bool CINEPlayer::error() {
	debug << "CINEPlayer -> Syntax error line " << line_num << "\n";
	finished = true;
	return false;
}

bool CINEPlayer::error(const char* er) {
	debug << "CINEPlayer -> Error line  " << line_num << " : " << er << "\n";
	finished = true;
	return false;
}
