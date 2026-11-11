#include <gtest/gtest.h>

#include <SDL2/SDL.h>

#include <climits>
#include <filesystem>
#include <string>

#include "core/game.h"
#include "core/startup.h"
#include "engine/controller.h"
#include "engine/input.h"
#include "entities/blip.h"

class HeadlessApp : public ::testing::Environment {
   public:
	void SetUp() override {
		SDL_setenv("SDL_VIDEODRIVER", "dummy", 1);
		SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
		// Ties game time to frames so every run repeats exactly
		SDL_setenv("BB_FIXEDSTEP", "1", 1);
		// A test run starts from default settings, never saves and reports bugs on stderr
		in.setAutoquit(INT_MAX);
		ASSERT_TRUE(InitApp(true));
	}
};

static auto* const headless = ::testing::AddGlobalTestEnvironment(new HeadlessApp);

struct HeldKeys : Controller {
	bool right = false, jumping = false, firing = false;

	int leftHeld() const override { return 0; }
	int upHeld() const override { return 0; }
	int rightHeld() const override { return right; }
	int downHeld() const override { return 0; }
	int fire() const override { return firing; }
	int jump() const override { return jumping; }
	int super() const override { return 0; }
	int nextWeapon() const override { return 0; }
	int prevWeapon() const override { return 0; }
};

// Steps the real update of a level without the menus, the loading pause and the drawing around it
struct TestGame : Game {
	using Game::loadLevel;
	using Game::releaseLevel;

	HeldKeys pad;

	bool start(const char* level_name) {
		if (!loadGame() || !loadLevel(level_name)) return false;
		player1 = std::make_unique<Blip>();
		player1->ctrl = &pad;
		player1->x = xstart1;
		player1->y = ystart1;
		// Movement checks must not depend on when the first enemy lands a hit
		player1->invincible = INT_MAX;
		session().players.push_back(player1.get());
		level().camera.offset = xstart1 - xstart1 % 640;
		return true;
	}

	void step(int frames) {
		while (frames-- > 0) {
			in.update();
			updateAll();
		}
	}

	Ball& player() { return *player1; }
	size_t dormantEvents() const { return list_event_dormant.size(); }
};

TEST(Level, LoadsTerrainAndEvents) {
	TestGame game;
	ASSERT_TRUE(game.start("lvl/snuf1.lvl"));
	const Terrain& t = level().terrain;
	EXPECT_GT(t.screens, 0u);
	EXPECT_EQ(t.width, t.screens * 640);
	ASSERT_EQ(t.solid.size(), 60u);
	EXPECT_EQ(t.solid[0].size(), t.width / 8);
	EXPECT_GT(game.dormantEvents(), 0u);
}

TEST(Level, MissingLevelFails) {
	TestGame game;
	EXPECT_FALSE(game.loadLevel("lvl/missing.lvl"));
}

TEST(Level, EveryLevelLoads) {
	TestGame game;
	int count = 0;
	for (const auto& f : std::filesystem::directory_iterator("assets/lvl")) {
		std::string name = f.path().filename().string();
		if (!name.ends_with(".lvl.json")) continue;
		std::string level_name = "lvl/" + name.substr(0, name.size() - 5);
		EXPECT_TRUE(game.loadLevel(level_name.c_str())) << level_name;
		game.releaseLevel();
		count++;
	}
	EXPECT_GT(count, 0);
}

TEST(Play, PlayerLandsAndWalksRight) {
	TestGame game;
	ASSERT_TRUE(game.start("lvl/snuf1.lvl"));
	game.step(100);
	int landed_y = game.player().y;
	game.step(20);
	EXPECT_EQ(game.player().y, landed_y);

	int x0 = game.player().x;
	game.pad.right = true;
	game.step(200);
	EXPECT_GT(game.player().x, x0 + 100);
	EXPECT_GT(level().camera.offset, 0);
}

TEST(Play, ReleaseLevelKeepsSession) {
	struct Marker {
		int value = 0;
	};
	TestGame game;
	ASSERT_TRUE(game.start("lvl/snuf1.lvl"));
	size_t dormant = game.dormantEvents();
	game.pad.right = true;
	game.step(300);
	EXPECT_LT(game.dormantEvents(), dormant) << "walking should trigger level events";

	level().shared<Marker>().value = 1;
	session().cow_bomb = true;
	game.releaseLevel();

	EXPECT_EQ(level().shared<Marker>().value, 0);
	EXPECT_TRUE(level().entities.enemies.empty());
	EXPECT_EQ(level().terrain.width, 0u);
	EXPECT_EQ(game.dormantEvents(), 0u);
	ASSERT_EQ(session().players.size(), 1u);
	EXPECT_EQ(session().players[0], &game.player());
	EXPECT_TRUE(session().cow_bomb);
}

struct Outcome {
	int x, y, offset, score;
	size_t enemies;
	bool operator==(const Outcome&) const = default;
};

// Runs, jumps and fires far enough to meet and kill the first enemies
static Outcome playScripted() {
	TestGame game;
	EXPECT_TRUE(game.start("lvl/snuf1.lvl"));
	game.pad.right = game.pad.firing = true;
	for (int i = 0; i < 1000; i++) {
		game.pad.jumping = i % 60 < 10;
		game.step(1);
	}
	Ball& p = game.player();
	return {p.x, p.y, level().camera.offset, p.getScore(), level().entities.enemies.size()};
}

// Uninitialised memory or state left over from an earlier game would make the runs diverge
TEST(Play, SameInputsSameOutcome) {
	Outcome first = playScripted();
	EXPECT_GT(first.score, 0);
	EXPECT_EQ(first, playScripted());
}
