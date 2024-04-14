#include <gtest/gtest.h>

#include <cstdlib>
#include <string>

#ifndef _WIN32
#include <sys/wait.h>
#endif

// Spawns the real exe so the test covers the player startup path
static int runGame(const std::string& args) {
	std::string cmd = std::string("\"") + GAME_EXE + "\" " + args;
#ifdef _WIN32
	// cmd.exe strips the outer quotes of the whole line
	return std::system(("\"" + cmd + "\"").c_str());
#else
	int status = std::system(cmd.c_str());
	return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
#endif
}

TEST(Startup, GameRunsAndQuits) { EXPECT_EQ(runGame("/autoquit 300"), 0); }
