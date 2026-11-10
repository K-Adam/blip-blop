#include "core/config.h"

#include "level/events/event_animated_background.h"

#include <unordered_map>

#include "level/backgrounds/background_bomb.h"
#include "level/backgrounds/background_poke_sky.h"
#include "level/backgrounds/background_poke_elevator.h"
#include "level/backgrounds/background_poke_switch.h"
#include "level/backgrounds/background_poke_crate_falling.h"
#include "level/backgrounds/background_neon_sign.h"

#include "level/backgrounds/background_snork_airlock.h"
#include "level/backgrounds/background_fish.h"

#include "level/backgrounds/background_fan.h"
#include "level/backgrounds/background_light.h"
#include "level/backgrounds/background_water.h"

#include "level/backgrounds/background_animated.h"
#include "level/backgrounds/background_electronics.h"
#include "level/backgrounds/background_camera_screen.h"
#include "level/backgrounds/background_assembler.h"
#include "level/backgrounds/background_windows.h"

#include "level/backgrounds/background_battle_cruiser.h"
#include "level/backgrounds/background_star.h"
#include "level/backgrounds/background_monolith.h"
#include "level/backgrounds/background_rock.h"
#include "level/backgrounds/background_rock_hyperspace.h"

#define BGID_POKE_ALARM1 100
#define BGID_POKE_ALARM2 101
#define BGID_POKE_ALARM3 102
#define BGID_POKE_ARROW 103
#define BGID_POKE_SKY 104
#define BGID_POKE_ELEVATOR 105
#define BGID_POKE_SWITCH 106
#define BGID_POKE_CRATE_FALLING 107
#define BGID_POKE_ELEVATOR2 108
#define BGID_BOMB 109
#define BGID_BOMB_FIXED 900
#define BGID_POKE_LEVEL2 110
#define BGID_POKE_LEVEL3 111

#define BGID_SNORK_AIRLOCK1 300
#define BGID_SNORK_AIRLOCK2 301
#define BGID_FISH_RED 302
#define BGID_FISH_PURPLE 303
#define BGID_FISH_RED_CAMOUFLAGED 304
#define BGID_FISH_PURPLE_CAMOUFLAGED 305

#define BGID_FAN 400
#define BGID_LIGHT 401
#define BGID_WATER 402
#define BGID_LIGHT2 403

#define BGID_CUBE 500
#define BGID_COIN 501
#define BGID_RING 502
#define BGID_SUNFLOWER1 503
#define BGID_SUNFLOWER2 504
#define BGID_SUNFLOWER3 505
#define BGID_SUNFLOWER4 506
#define BGID_FLAG 507

#define BGID_LCD 600
#define BGID_WORLDMAP 601
#define BGID_ELECTRONIC1 602
#define BGID_ELECTRONIC2 603
#define BGID_ELECTRONIC3 604
#define BGID_ELECTRONIC4 605
#define BGID_SNOW_SCREEN 606
#define BGID_CAMERA_SCREEN 607
#define BGID_ASSEMBLER 608
#define BGID_WINDOWS 609
#define BGID_DOS 610
#define BGID_BOSS_FAN 611

#define BGID_BATTLECRUISER 700
#define BGID_ROCK_HYPERSPACE 701
#define BGID_MONOLITH 702
#define BGID_ROCK1 703
#define BGID_ROCK2 704
#define BGID_STAR 705

static const int ANIM_ARROW[] = {0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0};
static const int ANIM_LCD[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 10, 11, 11, 10, 10, 11, 11,
							   10, 10, 11, 11, 10, 10, 11, 11, 10, 10, 12, 12, 12, 12, 12, 12, 12};
static const int ANIM_WORLDMAP[] = {0, 1, 0, 1, 0, 1, 2, 3, 4, 5, 6, 7, 6, 7, 6, 7, 8, 9, 10, 11, 12, 13, 12, 13, 12, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13};

static const int ROCK_FRAMES[] = {0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 0, 4, 4, 5, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 4, 4};
static const int ROCK1_RISE[] = {0, 0, 1, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0};
static const int ROCK2_RISE[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
static const RockSpec ROCK1{.sheet = "rock1_float", .frames = ROCK_FRAMES, .frame_delay = 3, .rise = ROCK1_RISE, .rise_delay = 4, .top = 30};
static const RockSpec ROCK2{.sheet = "rock2_float", .frames = ROCK_FRAMES, .frame_delay = 4, .rise = ROCK2_RISE, .rise_delay = 6, .top = 25};

static const NeonLetter SIGN_LEVEL2[] = {{"sign_l", 0, 155}, {"sign_e", 12, 300}, {"sign_v", 23, 210}, {"sign_e", 36, 190}, {"sign_l", 47, 380}};
static const NeonLetter SIGN_LEVEL3[] = {{"sign_e", 12, 120}, {"sign_v", 23, 180}, {"sign_e", 36, 290}, {"sign_l", 47, 190}};

template <class T>
static Sprite* make() {
	return new T();
}

struct BackgroundKind {
	Sprite* (*make)();
	bool moving_platform;
};

static const std::unordered_map<int, BackgroundKind> BACKGROUNDS = {
	{BGID_FAN, {make<BackgroundFan>, false}},
	{BGID_LIGHT, {[]() -> Sprite* { return new BackgroundLight("light_blink"); }, false}},
	{BGID_WATER, {make<BackgroundWater>, false}},
	{BGID_LIGHT2, {[]() -> Sprite* { return new BackgroundLight("light2_blink"); }, false}},
	{BGID_POKE_ALARM1, {[]() -> Sprite* { return new BackgroundAnimated("alarm_1", 3, true); }, false}},
	{BGID_POKE_ALARM2, {[]() -> Sprite* { return new BackgroundAnimated("alarm_2", 3, true); }, false}},
	{BGID_POKE_ALARM3, {[]() -> Sprite* { return new BackgroundAnimated("alarm_3", 3, true); }, false}},
	{BGID_POKE_ARROW, {[]() -> Sprite* { return new BackgroundSequence("arrow_cargo_storage", ANIM_ARROW, 3); }, false}},
	{BGID_POKE_SKY, {make<BackgroundPokeSky>, false}},
	{BGID_POKE_ELEVATOR, {[]() -> Sprite* { return new BackgroundPokeElevator(0, false); }, true}},
	{BGID_POKE_SWITCH, {make<BackgroundPokeSwitch>, true}},
	{BGID_POKE_CRATE_FALLING, {make<BackgroundPokeCrateFalling>, true}},
	{BGID_POKE_ELEVATOR2, {[]() -> Sprite* { return new BackgroundPokeElevator(1, true); }, true}},
	{BGID_BOMB, {make<BackgroundBomb>, true}},
	{BGID_POKE_LEVEL2, {[]() -> Sprite* { return new BackgroundNeonSign("sign_2", 560, SIGN_LEVEL2); }, false}},
	{BGID_POKE_LEVEL3, {[]() -> Sprite* { return new BackgroundNeonSign("sign_3", 460, SIGN_LEVEL3); }, false}},
	{BGID_SNORK_AIRLOCK1, {make<BackgroundSnorkAirlock1>, true}},
	{BGID_SNORK_AIRLOCK2, {make<BackgroundSnorkAirlock2>, true}},
	{BGID_FISH_RED, {[]() -> Sprite* { return new BackgroundFish("fish_red_right", "fish_red_left"); }, false}},
	{BGID_FISH_PURPLE, {[]() -> Sprite* { return new BackgroundFish("fish_purple_right", "fish_purple_left"); }, false}},
	{BGID_FISH_RED_CAMOUFLAGED, {[]() -> Sprite* { return new BackgroundFish("fish_red_camouflaged_right", "fish_red_camouflaged_left"); }, false}},
	{BGID_FISH_PURPLE_CAMOUFLAGED, {[]() -> Sprite* { return new BackgroundFish("fish_purple_camouflaged_right", "fish_purple_camouflaged_left"); }, false}},
	{BGID_CUBE, {make<BackgroundCube>, false}},
	{BGID_COIN, {make<BackgroundCoin>, false}},
	{BGID_RING, {make<BackgroundRing>, false}},
	{BGID_SUNFLOWER1, {[]() -> Sprite* { return new BackgroundAnimated("sunflower_1", 10, false); }, false}},
	{BGID_SUNFLOWER2, {[]() -> Sprite* { return new BackgroundAnimated("sunflower_2", 10, false); }, false}},
	{BGID_SUNFLOWER3, {[]() -> Sprite* { return new BackgroundAnimated("sunflower_3", 10, false); }, false}},
	{BGID_SUNFLOWER4, {[]() -> Sprite* { return new BackgroundAnimated("sunflower_4", 10, false); }, false}},
	{BGID_FLAG, {[]() -> Sprite* { return new BackgroundAnimated("flag_wave", 5, false); }, false}},
	{BGID_LCD, {[]() -> Sprite* { return new BackgroundSequence("lcd_specs", ANIM_LCD, 4); }, false}},
	{BGID_WORLDMAP, {[]() -> Sprite* { return new BackgroundSequence("worldmap", ANIM_WORLDMAP, 10); }, false}},
	{BGID_ELECTRONIC1, {[]() -> Sprite* { return new BackgroundElectronic("electronic1_overlay"); }, false}},
	{BGID_ELECTRONIC2, {[]() -> Sprite* { return new BackgroundElectronic("electronic2_overlay"); }, false}},
	{BGID_ELECTRONIC3, {[]() -> Sprite* { return new BackgroundElectronic("electronic3_overlay"); }, false}},
	{BGID_ELECTRONIC4, {make<BackgroundElectronic>, false}},
	{BGID_SNOW_SCREEN, {[]() -> Sprite* { return new BackgroundAnimated("snow_screen", 4, true); }, false}},
	{BGID_CAMERA_SCREEN, {make<BackgroundCameraScreen>, false}},
	{BGID_ASSEMBLER, {make<BackgroundAssembler>, false}},
	{BGID_WINDOWS, {make<BackgroundWindows>, false}},
	{BGID_DOS, {[]() -> Sprite* { return new BackgroundAnimated("dos_prompt", 40, true); }, false}},
	{BGID_BOSS_FAN, {[]() -> Sprite* { return new BackgroundAnimated("boss_fan", 3, true); }, false}},
	{BGID_BATTLECRUISER, {make<BackgroundBattleCruiser>, false}},
	{BGID_ROCK_HYPERSPACE, {make<BackgroundRockHyperspace>, false}},
	{BGID_MONOLITH, {make<BackgroundMonolith>, true}},
	{BGID_ROCK1, {[]() -> Sprite* { return new BackgroundRock(ROCK1); }, true}},
	{BGID_ROCK2, {[]() -> Sprite* { return new BackgroundRock(ROCK2); }, true}},
	{BGID_STAR, {make<BackgroundStar>, false}},
	{BGID_BOMB_FIXED, {make<BackgroundBomb>, false}},
};

void EventAnimatedBackground::doEvent() {
	auto it = BACKGROUNDS.find(bg_id);
	if (it == BACKGROUNDS.end()) {
		debug << "Unknown animated background id (" << bg_id << ")\n";
		return;
	}

	Sprite* s = it->second.make();
	s->x = x;
	s->y = y;

	if (it->second.moving_platform)
		list_moving_platforms.emplace_back(s);
	else
		list_animated_bgs.emplace_back(s);
}
