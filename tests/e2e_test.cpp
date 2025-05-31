#include <gtest/gtest.h>

#include <cstdio>
#include <string>
#include <vector>

#ifndef _WIN32
#include <sys/wait.h>
#endif

struct GameRun {
	int code = -1;
	std::string out;
};

// Spawns the real exe so the tests cover the player's startup and game flow
static GameRun runGame(const std::string& args) {
	std::string cmd = std::string("\"") + GAME_EXE + "\" " + args + " 2>&1";
	GameRun r;
#ifdef _WIN32
	// cmd.exe strips the outer quotes of the whole line
	FILE* p = _popen(("\"" + cmd + "\"").c_str(), "r");
#else
	FILE* p = popen(cmd.c_str(), "r");
#endif
	if (!p) return r;
	char buf[4096];
	while (size_t n = fread(buf, 1, sizeof buf, p)) r.out.append(buf, n);
#ifdef _WIN32
	r.code = _pclose(p);
#else
	int status = pclose(p);
	r.code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
#endif
	return r;
}

static void expectInOrder(const std::string& out, const std::vector<std::string>& lines) {
	size_t at = 0;
	for (const std::string& line : lines) {
		size_t found = out.find(line, at);
		ASSERT_NE(found, std::string::npos) << "missing in order: " << line << "\n" << out;
		at = found + line.size();
	}
}

static std::vector<std::string> frameHashes(const std::string& out) {
	std::vector<std::string> hashes;
	for (size_t at = out.find("frame "); at != std::string::npos; at = out.find("frame ", at + 1)) {
		hashes.push_back(out.substr(at, out.find('\n', at) - at));
	}
	return hashes;
}

TEST(E2E, Startup) {
	GameRun r = runGame("/autoquit 300");
	EXPECT_EQ(r.code, 0);
	EXPECT_EQ(r.out.find("Bug:"), std::string::npos) << r.out;
	expectInOrder(r.out, {"Title menu"});
}

// Intro skip, menus, join, character select, pause, two cheat level skips, the score screen and the next world
static const char* WORLD1 =
	"/cheat /autoquit 2200 /input "
	"200:Return,230:Return,260:Z,290:Z,350:Return,700:X,900:Escape,930:Return,960:F9,1300:F9,1700:X,1950:X";

TEST(E2E, CheatPlaysThroughWorld1) {
	GameRun r = runGame(WORLD1);
	EXPECT_EQ(r.code, 0);
	EXPECT_EQ(r.out.find("Bug:"), std::string::npos) << r.out;
	expectInOrder(r.out, {"Title menu", "Loading level <lvl/snuf1.lvl>", "Now getting into game loop.", "Pause menu", "Loading level <lvl/snuf2.lvl>",
						  "Now getting into game loop.", "Loading level <lvl/dork.lvl>", "Now getting into game loop."});
	// Uninitialised memory or wall clock timing would make a second run draw different frames
	std::vector<std::string> hashes = frameHashes(r.out);
	EXPECT_FALSE(hashes.empty());
	EXPECT_EQ(frameHashes(runGame(WORLD1).out), hashes);
}
