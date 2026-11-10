#include "core/app.h"

#include <filesystem>
#include <string>

#include "core/debug.h"
#include "engine/input.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Assets& assets() { return App::current().assets; }

// IndexedDB starts empty on a first visit, the preloaded files are the defaults
static void restoreDefaultSaves() {
#ifdef __EMSCRIPTEN__
	for (const char* f : {CONFIG_FILE, HISCORES_FILE}) {
		std::error_code ec;
		std::filesystem::copy_file(std::string("config_defaults/") + std::filesystem::path(f).filename().string(), f,
								   std::filesystem::copy_options::skip_existing, ec);
	}
#endif
}

bool App::load() {
	restoreDefaultSaves();
	// Tests run with the default controls whatever the local settings say
	if (in.isTestRun())
		settings.setDefaults(in, true);
	else
		settings.load(CONFIG_FILE, in);

	if (!hi_scores.load(HISCORES_FILE)) {
		debug << "Cannot load hi-scores file. Use default hi-scores\n";
		hi_scores.init();
	} else {
		debug << "Using " << HISCORES_FILE << " as hiscores file\n";
	}

	const char* texts_file = settings.lang == LANG_UK ? "uk.dat" : "fr.dat";
	if (!texts.load(texts_file)) {
		debug << "Cannot open " << texts_file << "\n";
		return false;
	}
	return true;
}

void App::save() const {
	// Test runs start from the repo, saving would rewrite the tracked config files
	if (in.isTestRun()) return;
	if (!hi_scores.save(HISCORES_FILE)) debug << "Cannot save hi-scores\n";
	settings.save(CONFIG_FILE, in);
#ifdef __EMSCRIPTEN__
	EM_ASM(FS.syncfs(false, function(err){}););
#endif
}
