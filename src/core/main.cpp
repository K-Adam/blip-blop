#define NAME "Blip'n Blop"

#define WIN32_LEAN_AND_MEAN

#include "engine/graphics.h"
#include <charconv>
#include <iostream>
#include <string_view>

#include "core/debug.h"
#include "core/config.h"
#include "engine/fmod.h"
#include "engine/font.h"
#include "core/game.h"
#include "core/task.h"
#include "core/assets.h"
#include "engine/input.h"
#include "engine/music_bank.h"
#include "engine/picture.h"
#include "engine/picture_bank.h"
#include "level/scroll.h"
#include "engine/sound_bank_bb.h"
#include "core/text_data.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Game game;

#if _WIN32
#include <windows.h>
HWND WinHandle = NULL;
#endif

static bool safeMode = false;

#if _WIN32
static void report(const char* txt, const wchar_t* title, UINT icon) {
	// A message box would hang an unattended test run
	if (autoquit_frames > 0) {
		std::cerr << (icon == MB_ICONERROR ? "Bug: " : "Warning: ") << txt << "\n";
		return;
	}
	std::wstring wide(MultiByteToWideChar(CP_UTF8, 0, txt, -1, NULL, 0), L'\0');
	MultiByteToWideChar(CP_UTF8, 0, txt, -1, wide.data(), (int)wide.size());
	MessageBoxW(WinHandle, wide.c_str(), title, MB_OK | icon);
}

void Bug(const char* txt) { report(txt, L"Blip'n Blop : Error reporting", MB_ICONERROR); }
void Warning(const char* txt) { report(txt, L"Blip'n Blop : Warning", MB_ICONWARNING); }
#else
void Bug(const char* txt) { std::cerr << "Bug: " << txt << "\n"; }
void Warning(const char* txt) { std::cerr << "Warning: " << txt << "\n"; }
#endif

static void parseCmdLine(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		std::string_view arg = argv[i];
		if (arg == "/safe") safeMode = true;
		if (arg == "/cheat") cheat_on = true;
		if (arg == "/quiet") music_on = false;
		if (arg == "/input" && i + 1 < argc && !in.setScript(argv[++i])) std::cerr << "Bug: bad /input script\n";
		if (arg == "/autoquit" && i + 1 < argc) {
			std::string_view n = argv[++i];
			std::from_chars(n.data(), n.data() + n.size(), autoquit_frames);
		}
	}
}

static bool InitApp(int nCmdShow) {
	struct {
		int height;
		int width;
	} win_size = {480, 640};

	debug << "---------------------------------------------------------------\n";
	debug << "Blip & Blop - (c) LOADED Studio - " << __DATE__ << "\n";
	debug << "---------------------------------------------------------------\n";

	if (!FSOUND_Init(44100, CHANNEL_NUMBER, 0)) {
		Warning("Cannot initialise FMOD. Sound will be turned off");
		sound_on = false;
		music_on = false;
	}

	restore_default_saves();
	// Tests run with the default controls whatever the local settings say
	if (autoquit_frames > 0)
		set_default_config(true);
	else
		load_BB3_config(CONFIG_FILE);

	hi_scores.init();

	if (!hi_scores.load(HISCORES_FILE)) {
		debug << "Cannot load hi-scores file. Use default hi-scores\n";
		hi_scores.init();
	} else {
		debug << "Using " << HISCORES_FILE << " as hiscores file\n";
	}

	if (lang_type == LANG_UK) {
		if (!loadTxtData("uk.dat")) {
			Bug("Cannot open the file 'uk.dat'");
			return false;
		}
	} else {
		if (!loadTxtData("fr.dat")) {
			Bug("Cannot open the file 'fr.dat'");
			return false;
		}
	}

	graphics.Init();

	if (!in.open()) {
		Bug("Cannot initialise DirectInput. Make sure DirectX 7 or better is "
			"installed.");
		return false;
	}

	debug << "DI Initialized\n";

	debug << "Exclusive mode set\n";

	static const int BEST_RATE = 85;

	if (safeMode) {
		debug << "Safe mode enabled, using default 640x480x16 refresh rate.\n";
		graphics.SetGfxMode(win_size.width, win_size.height, false);
	} else {
		debug << "Trying to create window\n";
		graphics.SetGfxMode(win_size.width, win_size.height, fullscreen);
		debug << "Window creation done\n";
	}

	backSurface = graphics.CreateSurface(win_size.width, win_size.height);

	if (backSurface == NULL) {
		Bug("Cannot get a primary surface. Please reboot your PC and try to "
			"launch Blip'n Blop again.\nBe sure you have at least 2 Mb of "
			"video memory on your video card.");
		return false;
	}
	debug << "Creating systemSurface\n";
	systemSurface = graphics.CreateSurface(win_size.width, win_size.height);

	if (systemSurface == NULL) {
		Bug("Not enough memory. Blip'n Blop needs 32 Mo of free memory. Try to "
			"close all other applications and launch Blip'n Blop again.");
		return false;
	}
	debug << "systemSurface created\n";

	videoA = graphics.CreateSurface(WANTED_VBUFFER_WIDE, 480);

	if (videoA == NULL) {
		Bug("Not enough memory. Blip'n Blop needs 32 Mo of free "
			"memory. Try to close all other applications and "
			"launch Blip'n Blop again.");
		return false;
	}

	if (!fnt_menu.load("menu.lft")) {
		Bug("Cannot open the file menu.lft");
		return false;
	}

	if (!fnt_menus.load("menus.lft")) {
		Bug("Cannot open the file menus.lft");
		return false;
	}

	if (!fnt_cool.load("cool.lft")) {
		Bug("Cannot open the file cool.lft");
		return false;
	}

	if (!fnt_rpg.load("rpg.lft")) {
		Bug("Cannot open the file rpg.lft");
		return false;
	}

	if (!pbk_inter.loadGFX("inter.gfx")) {
		debug << "Cannot load interface.\n";
		return false;
	} else {
		debug << "Successfully loaded interface.\n";
	}

	if (!mbk_inter.open("inter.mbk", false)) {
		debug << "Cannot load interface musics.\n";
		return false;
	}

	if (!mbk_interl.open("interl.mbk", true)) {
		debug << "Cannot load interface musics (p2).\n";
		return false;
	}

	return true;
}

#undef main

void cleanup() {
	debug << "---------------------------------------------------------------\n";
	debug << "Releasing Blip & Blop\n";
	debug << "---------------------------------------------------------------\n";

	// Test runs start from the repo, saving would rewrite the tracked config files
	if (autoquit_frames == 0) save_settings();
}

int main(int argc, char** argv) {
	int nCmdShow = 0;

	parseCmdLine(argc, argv);

	try {
		if (!InitApp(nCmdShow)) return -1;
	} catch (const std::exception& e) {
		Bug(e.what());
		return -1;
	}

	atexit(cleanup);

	debug << "Game.go"
		  << "\n";

	static Task<> root = game.go();
	root.start();
#ifdef __EMSCRIPTEN__
	// The browser calls this once per animation frame, main never returns
	emscripten_set_main_loop(
		[] {
			if (root.done()) {
				emscripten_cancel_main_loop();
				return;
			}
			in.update();
			resumeFrame();
		},
		0, true);
#else
	while (!root.done()) {
		in.update();
		resumeFrame();
	}
#endif

	return 0;
}
