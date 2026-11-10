#define NAME "Blip'n Blop"

#define WIN32_LEAN_AND_MEAN

#include "engine/graphics.h"
#include <charconv>
#include <iostream>
#include <string_view>

#include "core/debug.h"
#include "core/app.h"
#include "core/game.h"
#include "core/task.h"
#include "engine/input.h"
#include "engine/picture.h"
#include "level/world.h"
#include "engine/sound.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Game game;

#if _WIN32
#include <windows.h>

static void report(const char* txt, const wchar_t* title, UINT icon) {
	// A message box would hang an unattended test run
	if (in.isTestRun()) {
		std::cerr << (icon == MB_ICONERROR ? "Bug: " : "Warning: ") << txt << "\n";
		return;
	}
	std::wstring wide(MultiByteToWideChar(CP_UTF8, 0, txt, -1, NULL, 0), L'\0');
	MultiByteToWideChar(CP_UTF8, 0, txt, -1, wide.data(), (int)wide.size());
	MessageBoxW(NULL, wide.c_str(), title, MB_OK | icon);
}

void Bug(const char* txt) { report(txt, L"Blip'n Blop : Error reporting", MB_ICONERROR); }
void Warning(const char* txt) { report(txt, L"Blip'n Blop : Warning", MB_ICONWARNING); }
#else
void Bug(const char* txt) { std::cerr << "Bug: " << txt << "\n"; }
void Warning(const char* txt) { std::cerr << "Warning: " << txt << "\n"; }
#endif

// Returns true for /safe, which keeps the window whatever the settings say
static bool parseCmdLine(int argc, char** argv) {
	bool safe_mode = false;
	for (int i = 1; i < argc; i++) {
		std::string_view arg = argv[i];
		if (arg == "/safe") safe_mode = true;
		if (arg == "/cheat") settings().cheat = true;
		if (arg == "/quiet") audio.muteMusic();
		if (arg == "/input" && i + 1 < argc && !in.setScript(argv[++i])) std::cerr << "Bug: bad /input script\n";
		if (arg == "/autoquit" && i + 1 < argc) {
			std::string_view n = argv[++i];
			int frames = 0;
			std::from_chars(n.data(), n.data() + n.size(), frames);
			in.setAutoquit(frames);
			graphics.EnableFrameHash();
		}
	}
	return safe_mode;
}

static bool InitApp(bool safe_mode) {
	struct {
		int height;
		int width;
	} win_size = {480, 640};

	debug << "---------------------------------------------------------------\n";
	debug << "Blip & Blop - (c) LOADED Studio - " << __DATE__ << "\n";
	debug << "---------------------------------------------------------------\n";

	if (!audio.init()) Warning("Cannot initialise audio. Sound will be turned off");

	App& app = App::current();
	if (!app.load()) {
		Bug("Cannot open the texts file");
		return false;
	}

	graphics.Init();

	if (!in.open()) {
		Bug("Cannot initialise DirectInput. Make sure DirectX 7 or better is "
			"installed.");
		return false;
	}

	debug << "DI Initialized\n";

	debug << "Exclusive mode set\n";

	if (safe_mode) {
		debug << "Safe mode enabled, using default 640x480x16 refresh rate.\n";
		graphics.SetGfxMode(win_size.width, win_size.height, false, app.settings.vsync);
	} else {
		debug << "Trying to create window\n";
		graphics.SetGfxMode(win_size.width, win_size.height, app.settings.fullscreen, app.settings.vsync);
		debug << "Window creation done\n";
	}

	graphics.CreateScreens(win_size.width, win_size.height);

	World::current().scroll.init();

	if (!assets().fonts.menu.load("menu.lft")) {
		Bug("Cannot open the file menu.lft");
		return false;
	}

	if (!assets().fonts.menus.load("menus.lft")) {
		Bug("Cannot open the file menus.lft");
		return false;
	}

	if (!assets().fonts.cool.load("cool.lft")) {
		Bug("Cannot open the file cool.lft");
		return false;
	}

	if (!assets().fonts.rpg.load("rpg.lft")) {
		Bug("Cannot open the file rpg.lft");
		return false;
	}

	if (!assets().pics.inter.loadGFX("inter.gfx")) {
		debug << "Cannot load interface.\n";
		return false;
	} else {
		debug << "Successfully loaded interface.\n";
	}

	if (!assets().music.inter.open("inter.mbk", false)) {
		debug << "Cannot load interface musics.\n";
		return false;
	}

	if (!assets().music.interl.open("interl.mbk", true)) {
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

	App::current().save();
}

int main(int argc, char** argv) {
	bool safe_mode = parseCmdLine(argc, argv);

	try {
		if (!InitApp(safe_mode)) return -1;
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
