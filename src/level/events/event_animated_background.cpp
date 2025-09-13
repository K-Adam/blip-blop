#include "core/config.h"

#include "level/events/event_animated_background.h"

#include "level/backgrounds/background_bomb.h"
#include "level/backgrounds/background_poke_alarm.h"
#include "level/backgrounds/background_poke_arrow.h"
#include "level/backgrounds/background_poke_sky.h"
#include "level/backgrounds/background_poke_elevator.h"
#include "level/backgrounds/background_poke_switch.h"
#include "level/backgrounds/background_poke_crate_falling.h"
#include "level/backgrounds/background_poke_elevator_2.h"
#include "level/backgrounds/background_poke_level_2.h"
#include "level/backgrounds/background_poke_level_3.h"

#include "level/backgrounds/background_snork_airlock.h"
#include "level/backgrounds/background_fish.h"

#include "level/backgrounds/background_fan.h"
#include "level/backgrounds/background_light.h"
#include "level/backgrounds/background_light_2.h"
#include "level/backgrounds/background_water.h"

#include "level/backgrounds/background_animated.h"
#include "level/backgrounds/background_lcd.h"
#include "level/backgrounds/background_worldmap.h"
#include "level/backgrounds/background_electronics.h"
#include "level/backgrounds/background_camera_screen.h"
#include "level/backgrounds/background_assembler.h"
#include "level/backgrounds/background_windows.h"

#include "level/backgrounds/background_battle_cruiser.h"
#include "level/backgrounds/background_star.h"
#include "level/backgrounds/background_monolith.h"
#include "level/backgrounds/background_rock_1.h"
#include "level/backgrounds/background_rock_2.h"
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

void EventAnimatedBackground::doEvent() {
	Sprite* s = NULL;
	bool moving_platform = false;

	switch (bg_id) {
		case BGID_FAN:
			s = new BackgroundFan();
			break;

		case BGID_LIGHT:
			s = new BackgroundLight();
			break;

		case BGID_WATER:
			s = new BackgroundWater();
			break;

		case BGID_LIGHT2:
			s = new BackgroundLight2();
			break;

		case BGID_POKE_ALARM1:
			s = new BackgroundPokeAlarm1();
			break;

		case BGID_POKE_ALARM2:
			s = new BackgroundPokeAlarm2();
			break;

		case BGID_POKE_ALARM3:
			s = new BackgroundPokeAlarm3();
			break;

		case BGID_POKE_ARROW:
			s = new BackgroundPokeArrow();
			break;

		case BGID_POKE_SKY:
			s = new BackgroundPokeSky();
			break;

		case BGID_POKE_ELEVATOR:
			s = new BackgroundPokeElevator();
			moving_platform = true;
			break;

		case BGID_POKE_SWITCH:
			s = new BackgroundPokeSwitch();
			moving_platform = true;
			break;

		case BGID_POKE_CRATE_FALLING:
			s = new BackgroundPokeCrateFalling();
			moving_platform = true;
			break;

		case BGID_POKE_ELEVATOR2:
			s = new BackgroundPokeElevator2();
			moving_platform = true;
			break;

		case BGID_BOMB:
			s = new BackgroundBomb();
			moving_platform = true;
			break;

		case BGID_POKE_LEVEL2:
			s = new BackgroundPokeLevel2();
			break;

		case BGID_POKE_LEVEL3:
			s = new BackgroundPokeLevel3();
			break;

		case BGID_SNORK_AIRLOCK1:
			s = new BackgroundSnorkAirlock1();
			moving_platform = true;
			break;

		case BGID_SNORK_AIRLOCK2:
			s = new BackgroundSnorkAirlock2();
			moving_platform = true;
			break;

		case BGID_FISH_RED:
			s = new BackgroundFishRed();
			break;

		case BGID_FISH_PURPLE:
			s = new BackgroundFishPurple();
			break;

		case BGID_FISH_RED_CAMOUFLAGED:
			s = new BackgroundFishRedCamouflaged();
			break;

		case BGID_FISH_PURPLE_CAMOUFLAGED:
			s = new BackgroundFishPurpleCamouflaged();
			break;

		case BGID_CUBE:
			s = new BackgroundCube();
			break;

		case BGID_COIN:
			s = new BackgroundCoin();
			break;

		case BGID_RING:
			s = new BackgroundRing();
			break;

		case BGID_SUNFLOWER1:
			s = new BackgroundSunflower1();
			break;

		case BGID_SUNFLOWER2:
			s = new BackgroundSunflower2();
			break;

		case BGID_SUNFLOWER3:
			s = new BackgroundSunflower3();
			break;

		case BGID_SUNFLOWER4:
			s = new BackgroundSunflower4();
			break;

		case BGID_FLAG:
			s = new BackgroundFlag();
			break;

		case BGID_LCD:
			s = new BackgroundLCD();
			break;

		case BGID_WORLDMAP:
			s = new BackgroundWorldmap();
			break;

		case BGID_ELECTRONIC1:
			s = new BackgroundElectronic1();
			break;

		case BGID_ELECTRONIC2:
			s = new BackgroundElectronic2();
			break;

		case BGID_ELECTRONIC3:
			s = new BackgroundElectronic3();
			break;

		case BGID_ELECTRONIC4:
			s = new BackgroundElectronic4();
			break;

		case BGID_SNOW_SCREEN:
			s = new BackgroundSnowScreen();
			break;

		case BGID_CAMERA_SCREEN:
			s = new BackgroundCameraScreen();
			break;

		case BGID_ASSEMBLER:
			s = new BackgroundAssembler();
			break;

		case BGID_WINDOWS:
			s = new BackgroundWindows();
			break;

		case BGID_DOS:
			s = new BackgroundDos();
			break;

		case BGID_BOSS_FAN:
			s = new BackgroundBossFan();
			break;

		case BGID_BATTLECRUISER:
			s = new BackgroundBattleCruiser();
			break;

		case BGID_ROCK_HYPERSPACE:
			s = new BackgroundRockHyperspace();
			break;

		case BGID_MONOLITH:
			s = new BackgroundMonolith();
			moving_platform = true;
			break;

		case BGID_ROCK1:
			s = new BackgroundRock1();
			moving_platform = true;
			break;

		case BGID_ROCK2:
			s = new BackgroundRock2();
			moving_platform = true;
			break;

		case BGID_STAR:
			s = new BackgroundStar();
			break;

		case 900:
			s = new BackgroundBomb();
			break;
	}

	if (s != NULL) {
		s->x = x;
		s->y = y;

		if (moving_platform)
			list_moving_platforms.emplace_back(s);
		else
			list_animated_bgs.emplace_back(s);
	} else {
		debug << "Erreur : identité de fond animé inconnue (" << bg_id << ")\n";
	}
}
