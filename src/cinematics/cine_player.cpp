#include <algorithm>
#include <cctype>
#include <charconv>
#include <string_view>
#include "cinematics/cine_player.h"
#include "core/misc.h"
#include "core/debug.h"
#include "engine/input.h"
#include "core/text_data.h"
#include "core/globals.h"
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

	num_ligne = 0;
	frame_to_draw = 0;
	fini = false;
	clip_x1 = 0;
	clip_y1 = 0;
	clip_x2 = 640;
	clip_y2 = 480;
	alpha = 0;
	delta_alpha = 0;
	color[0] = color[1] = 0;
	clip_color[0] = clip_color[1] = 0;
	delta_vol = 0;
}

Task<bool> CINEPlayer::playScene(const char* file, SDL::Surface* s1, SDL::Surface* s2, bool anyInputSkips) {
	bool skiped = false;
	initPlayer();

	/*first_surf = s1;*/
	back_surf = s2;

	auto dir = asset_path_prefix("cin", file);
	std::ifstream input(dir + ".json");
	if (!input.good()) {
		debug << "CINEPlayer -> Cannot open file " << dir << ".json"
			  << "\n";
		co_return false;
	}

	json data = json::parse(input);

	// Reproduce old file format in a string stream
	fic_ = std::stringstream{};

	auto blocks = data["blocks"];
	for (auto block_it = blocks.begin(); block_it != blocks.end(); ++block_it) {
		auto block = *block_it;

		std::string name = block["name"];

		fic_ << ";"
			 << "\n";
		fic_ << "; " << name << "\n";
		fic_ << ";"
			 << "\n";

		auto items = block["items"];
		for (auto item_it = items.begin(); item_it != items.end(); ++item_it) {
			auto item = *item_it;

			std::string name = item["name"];
			std::stringstream param_stream;

			bool first = false;
			for (auto param : item["params"]) {
				if (first) {
					first = false;
				} else {
					param_stream << ",";
				}
				param_stream << " " << param;
			}

			fic_ << name << "(" << param_stream.str() << ")";

			if ((block_it + 1) != blocks.end() || (item_it + 1) != items.end()) {
				fic_ << "\n";
			}
		}
	}

	while (!fini && !app_killed) {
		updateState();
		update_regulator_.Skip();

		while (frame_to_draw > 0 && !fini && !app_killed) {
			renderLoop();

			// Enter also reads a click or pad A, a fresh press so fire held from the level does not skip
			if (anyInputSkips ? in.anyNewPress() : in.scanKey(DIK_ESCAPE) || in.pressedKey(DIK_RETURN)) {
				fini = true;
				skiped = true;
			}

			co_await nextFrame();
		}
	}

	closePlayer();

	co_return !skiped;
}

void CINEPlayer::updateScene() {
	// Gére le volume
	int vol = delta_vol;

	if (vol < 0) vol = 0;

	// Gére l'alpha blending
	alpha += delta_alpha;

	if (alpha > 255)
		alpha = 255;
	else if (alpha < 0)
		alpha = 0;

	// Gére les objets
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

				obj[i].pic = pbk[obj[i].anim[obj[i].etape]];

				if (obj[i].wait_anim == 0) {
					obj[i].etape += 1;

					if (obj[i].etape >= obj[i].nb_anim) {
						if (obj[i].loop_anim) {
							obj[i].etape = 0;
							obj[i].pic = pbk[obj[i].anim[0]];
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
	int n2;
	int i;

	while (!fini && !to_draw && getCommand()) {
		if (cmd_name == "endscene") {
			fini = true;
		} else if (cmd_name == "loadgfx") {
			if (!pbk.loadGFX(str_arg[0].c_str())) error("ne peut pas charger le fichier GFX");
		} else if (cmd_name == "loadmbk") {
			if (!mbk.open(str_arg[0].c_str())) error("ne peut pas charger la MBK");
		} else if (cmd_name == "playmusic") {
			mbk.play(int_arg[0]);
		} else if (cmd_name == "stopmusic") {
			mbk.stop();
		} else if (cmd_name == "setpos") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("numéro d'objet invalide");
			else {
				obj[n].x = int_arg[1];
				obj[n].y = int_arg[2];
			}
		} else if (cmd_name == "setpic") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("numéro d'objet invalide");
			else {
				n2 = int_arg[1];

				if (n2 < 0 || (size_t)n2 >= pbk.getSize())
					error("numéro d'image invalide");
				else {
					obj[n].id = OBJ_SPRITE;
					obj[n].pic = pbk[n2];
					obj[n].show = true;
				}
			}
		} else if (cmd_name == "setmove") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("numéro d'objet invalide");
			else {
				obj[n].dx = int_arg[1];
				obj[n].dy = int_arg[2];
				obj[n].time_mov = int_arg[3];
			}
		} else if (cmd_name == "settext") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("numéro d'objet invalide");
			else {
				obj[n].id = OBJ_TEXTE;
				obj[n].txt = int_arg[1];
				obj[n].show = true;
			}
		} else if (cmd_name == "reset") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("numéro d'objet invalide");
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
				error("numéro d'objet invalide");
			else {
				obj[n].speed_anim = int_arg[1];
				if (int_arg[2])
					obj[n].loop_anim = true;
				else
					obj[n].loop_anim = false;

				obj[n].id = OBJ_SPRITE;
				obj[n].pic = pbk[int_arg[3]];
				obj[n].wait_anim = 0;
				obj[n].etape = 0;
				obj[n].nb_anim = nb_args - 3;

				for (i = 3; i < nb_args; i++) {
					obj[n].anim[i - 3] = int_arg[i];
				}
			}
		} else if (cmd_name == "setxwarp") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("numéro d'objet invalide");
			else {
				obj[n].x_warp = true;
				obj[n].x_warp1 = int_arg[1];
				obj[n].x_warp2 = int_arg[2];
			}
		} else if (cmd_name == "setywarp") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("numéro d'objet invalide");
			else {
				obj[n].y_warp = true;
				obj[n].y_warp1 = int_arg[1];
				obj[n].y_warp2 = int_arg[2];
			}
		} else if (cmd_name == "stopanim") {
			n = int_arg[0];

			if (n < 0 || n >= NB_OBJ)
				error("numéro d'objet invalide");
			else {
				obj[n].nb_anim = false;
			}
		} else if (cmd_name == "setalpha") {
			alpha = int_arg[0];
			delta_alpha = 0;
		} else if (cmd_name == "setdeltaalpha") {
			delta_alpha = int_arg[0];
		} else if (cmd_name == "setbackcolor") {
			if (int_arg[0] < 0 || int_arg[0] > 1)
				error("mauvais numéro de calque");
			else {
				if (int_arg[1] == 0) {
					color[int_arg[0]] = 0;
				} else {
					color[int_arg[0]] = 0xFFFF;
				}
			}
		} else if (cmd_name == "setclipcolor") {
			if (int_arg[0] < 0 || int_arg[0] > 1)
				error("mauvais numéro de calque");
			else {
				if (int_arg[1] == 0) {
					clip_color[int_arg[0]] = 0;
				} else {
					clip_color[int_arg[0]] = 0xFFFF;
				}
			}
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
	RenderRect ddfx;

	Rect r;

	back_surf->FillRect(0, 0xFF000000);

	// Affiche tous les sprites/scrolls
	for (int i = 0; i < NB_OBJ; i++) {
		if (obj[i].show) {
			switch (obj[i].id) {
				case OBJ_SPRITE:
					drawSprite(i);
					break;
			}
		}
	}

	// Clippe le tout
	ddfx.dwFillColor = clip_color[0];

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

	// Affiche tous les textes
	for (int i = 0; i < NB_OBJ; i++) {
		if (obj[i].show && obj[i].id == OBJ_TEXTE) drawText(i);
	}

	DDFlip();
}

void CINEPlayer::drawSprite(int n) {
	if (n < NB_OBJ) obj[n].pic->BlitTo(back_surf, obj[n].x, obj[n].y);
}

void CINEPlayer::drawText(int n) {
	if (n < NB_OBJ) fnt_rpg.printC(back_surf, obj[n].x, obj[n].y, txt_data[obj[n].txt]);
}

bool CINEPlayer::getCommand() {
	std::string line;

	// Esquive les lignes commentées et les lignes vides
	do {
		// A script without endscene would otherwise spin forever on EOF
		if (!std::getline(fic_, line)) {
			fini = true;
			return false;
		}
		num_ligne += 1;
	} while (line.empty() || line[0] == ';');

	// Vire les espaces et les " et met tout en minuscule, jusqu'au commentaire
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

void CINEPlayer::closePlayer() { fic_ = std::stringstream{}; }

bool CINEPlayer::error() {
	debug << "CINEPlayer -> Syntax error line " << num_ligne << "\n";
	fini = true;
	return false;
}

bool CINEPlayer::error(const char* er) {
	debug << "CINEPlayer -> Error line  " << num_ligne << " : " << er << "\n";
	fini = true;
	return false;
}
