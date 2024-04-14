#define NAME "Blip'n Blop"

#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include <cstdlib>
#include <string.h>

#include "ben_debug.h"
#include "ben_divers.h"
#include "config.h"
#include "dd_gfx.h"
#include "fmod.h"
#include "fonte.h"
#include "game.h"
#include "globals.h"
#include "input.h"
#include "music_bank.h"
#include "picture.h"
#include "picture_bank.h"
#include "scroll.h"
#include "sound_bank_bb.h"
#include "txt_data.h"

Game game;

#if _WIN32
#include <windows.h>
HWND WinHandle = NULL;
#endif

static bool safeMode = false;

void ReleaseAll(void) {
	if (graphicInstance != NULL) {
		if (videoA != NULL) {
			videoA->Release();
			videoA = NULL;
		}

		if (systemSurface != NULL) {
			systemSurface->Release();
			systemSurface = NULL;
		}

		if (primSurface != NULL) {
			primSurface->Release();
			primSurface = NULL;
		}

		DDCloseDirectDraw();
	}

	FSOUND_Close();

	in.close();
}

#if _WIN32
static void report(const char* txt, const wchar_t* title, UINT icon) {
	// A message box would hang an unattended test run
	if (autoquit_frames > 0) {
		std::cerr << txt << "\n";
		return;
	}
	std::wstring wide(MultiByteToWideChar(CP_UTF8, 0, txt, -1, NULL, 0), L'\0');
	MultiByteToWideChar(CP_UTF8, 0, txt, -1, wide.data(), (int)wide.size());
	MessageBoxW(WinHandle, wide.c_str(), title, MB_OK | icon);
}

void Bug(const char* txt) { report(txt, L"Blip'n Blop : Error reporting", MB_ICONERROR); }
void Warning(const char* txt) { report(txt, L"Blip'n Blop : Warning", MB_ICONWARNING); }
#else
void Bug(const char* txt) { std::cerr << txt << "\n"; }
void Warning(const char* txt) { std::cerr << txt << "\n"; }
#endif

static void analyseCmdLine(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "/safe") == 0) safeMode = true;
		if (strcmp(argv[i], "/cheat") == 0) cheat_on = true;
		if (strcmp(argv[i], "/quiet") == 0) music_on = false;
		if (strcmp(argv[i], "/autoquit") == 0 && i + 1 < argc) autoquit_frames = atoi(argv[++i]);
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

	mount_save_dir();
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

	DDInitDirectDraw();

	if (!in.open()) {
		Bug("Cannot initialise DirectInput. Make sure DirectX 7 or better is "
			"installed.");
		return false;
	}

	debug << "DI Initialized\n";

	// Mode EXCLUSIF (bourrin quoi...)

	debug << "Exclusive mode set\n";

	static const int BEST_RATE = 85;

	if (safeMode) {
		debug << "Safe mode enabled, using default 640x480x16 refresh rate.\n";
		DDSetGfxMode(win_size.width, win_size.height, 16, false /* fullscreen */);
	} else {
		debug << "Trying to create window\n";
		DDSetGfxMode(win_size.width, win_size.height, 16, fullscreen /* fullscreen */);
		debug << "Window creation done\n";
	}

	// Crée la surface primaire avec double buffer
	debug << "Creating primSurface\n";
	primSurface = DDCreatePrimary(backSurface);

	if (primSurface == NULL || backSurface == NULL) {
		Bug("Cannot get a primary surface. Please reboot your PC and try to "
			"launch Blip'n Blop again.\nBe sure you have at least 2 Mb of "
			"video memory on your video card.");
		return false;
	}
	debug << "primSurface created\n";

	debug << "Creating systemSurface\n";
	systemSurface = DDCreateSurface(win_size.width, win_size.height, DDSURF_SYSTEM);

	if (systemSurface == NULL) {
		Bug("Not enough memory. Blip'n Blop needs 32 Mo of free memory. Try to "
			"close all other applications and launch Blip'n Blop again.");
		return false;
	}
	debug << "systemSurface created\n";

	videoA = DDCreateSurface(WANTED_VBUFFER_WIDE, 480, DDSURF_SYSTEM);

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

	return true;  // C'est fini!
}

#undef main

void cleanup() {
	debug << "---------------------------------------------------------------\n";
	debug << "Releasing Blip & Blop\n";
	debug << "---------------------------------------------------------------\n";

	// Désalloue tout ce qu'il faut (en théorie)

	// Test runs start from the repo, saving would rewrite the tracked config files
	if (autoquit_frames == 0) save_settings();
}

int main(int argc, char** argv) {
	int nCmdShow = 0;

	analyseCmdLine(argc, argv);

	try {
		if (!InitApp(nCmdShow)) return -1;
	} catch (const std::exception& e) {
		Bug(e.what());
		return -1;
	}

	atexit(cleanup);

	debug << "Game.go"
		  << "\n";

	game.go();

	return 0;

}  // WinMain ------------------------
