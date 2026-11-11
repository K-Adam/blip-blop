#define NAME "Blip'n Blop"

#include "engine/graphics.h"
#include <charconv>
#include <iostream>
#include <string_view>

#include "core/debug.h"
#include "core/app.h"
#include "core/startup.h"
#include "core/game.h"
#include "core/task.h"
#include "engine/input.h"
#include "engine/picture.h"
#include "engine/sound.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Game game;

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
