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

#define IDFOND_POKEALARME1 100
#define IDFOND_POKEALARME2 101
#define IDFOND_POKEALARME3 102
#define IDFOND_POKEFLECHE 103
#define IDFOND_POKECIEL 104
#define IDFOND_POKEASCENC 105
#define IDFOND_INTERRUPTEUR 106
#define IDFOND_CAISSETOMBANTE 107
#define IDFOND_POKEASCENC2 108
#define IDFOND_BOMBE 109
#define IDFOND_LEVEL2 110
#define IDFOND_LEVEL3 111

#define IDFOND_SAS1 300
#define IDFOND_SAS2 301
#define IDFOND_POISSONROUGE 302
#define IDFOND_POISSONVIOLET 303
#define IDFOND_POISSONROUGECAMOUFLE 304
#define IDFOND_POISSONVIOLETCAMOUFLE 305

#define IDFOND_VENTILO 400
#define IDFOND_LUMIERE 401
#define IDFOND_EAU 402
#define IDFOND_LUMIERE2 403

#define IDFOND_CUBE 500
#define IDFOND_PIECE 501
#define IDFOND_RING 502
#define IDFOND_TOURNESOL1 503
#define IDFOND_TOURNESOL2 504
#define IDFOND_TOURNESOL3 505
#define IDFOND_TOURNESOL4 506
#define IDFOND_DRAPEAU 507

#define IDFOND_LCD 600
#define IDFOND_WORLDMAP 601
#define IDFOND_ELECTRONIC1 602
#define IDFOND_ELECTRONIC2 603
#define IDFOND_ELECTRONIC3 604
#define IDFOND_ELECTRONIC4 605
#define IDFOND_ECRANNEIGE 606
#define IDFOND_ECRANCAMERA 607
#define IDFOND_ASSEMBLEUR 608
#define IDFOND_WINDOWS 609
#define IDFOND_DOS 610
#define IDFOND_BOSSVENTILO 611

#define IDFOND_BATTLECRUISER 700
#define IDFOND_ROCHER_HYPERSPACE 701
#define IDFOND_MONOLITE 702
#define IDFOND_ROCHER1 703
#define IDFOND_ROCHER2 704
#define IDFOND_ETOILE 705

void EventAnimatedBackground::doEvent() {
	Sprite* s = NULL;
	bool plat_mobile = false;

	switch (id_fond) {
		case IDFOND_VENTILO:
			s = new BackgroundFan();
			break;

		case IDFOND_LUMIERE:
			s = new BackgroundLight();
			break;

		case IDFOND_EAU:
			s = new BackgroundWater();
			break;

		case IDFOND_LUMIERE2:
			s = new BackgroundLight2();
			break;

		case IDFOND_POKEALARME1:
			s = new BackgroundPokeAlarm1();
			break;

		case IDFOND_POKEALARME2:
			s = new BackgroundPokeAlarm2();
			break;

		case IDFOND_POKEALARME3:
			s = new BackgroundPokeAlarm3();
			break;

		case IDFOND_POKEFLECHE:
			s = new BackgroundPokeArrow();
			break;

		case IDFOND_POKECIEL:
			s = new BackgroundPokeSky();
			break;

		case IDFOND_POKEASCENC:
			s = new BackgroundPokeElevator();
			plat_mobile = true;
			break;

		case IDFOND_INTERRUPTEUR:
			s = new BackgroundPokeSwitch();
			plat_mobile = true;
			break;

		case IDFOND_CAISSETOMBANTE:
			s = new BackgroundPokeCrateFalling();
			plat_mobile = true;
			break;

		case IDFOND_POKEASCENC2:
			s = new BackgroundPokeElevator2();
			plat_mobile = true;
			break;

		case IDFOND_BOMBE:
			s = new BackgroundBomb();
			plat_mobile = true;
			break;

		case IDFOND_LEVEL2:
			s = new BackgroundPokeLevel2();
			break;

		case IDFOND_LEVEL3:
			s = new BackgroundPokeLevel3();
			break;

		case IDFOND_SAS1:
			s = new BackgroundSnorkAirlock1();
			plat_mobile = true;
			break;

		case IDFOND_SAS2:
			s = new BackgroundSnorkAirlock2();
			plat_mobile = true;
			break;

		case IDFOND_POISSONROUGE:
			s = new BackgroundFishRed();
			break;

		case IDFOND_POISSONVIOLET:
			s = new BackgroundFishPurple();
			break;

		case IDFOND_POISSONROUGECAMOUFLE:
			s = new BackgroundFishRedCamouflaged();
			break;

		case IDFOND_POISSONVIOLETCAMOUFLE:
			s = new BackgroundFishPurpleCamouflaged();
			break;

		case IDFOND_CUBE:
			s = new BackgroundCube();
			break;

		case IDFOND_PIECE:
			s = new BackgroundCoin();
			break;

		case IDFOND_RING:
			s = new BackgroundRing();
			break;

		case IDFOND_TOURNESOL1:
			s = new BackgroundSunflower1();
			break;

		case IDFOND_TOURNESOL2:
			s = new BackgroundSunflower2();
			break;

		case IDFOND_TOURNESOL3:
			s = new BackgroundSunflower3();
			break;

		case IDFOND_TOURNESOL4:
			s = new BackgroundSunflower4();
			break;

		case IDFOND_DRAPEAU:
			s = new BackgroundFlag();
			break;

		case IDFOND_LCD:
			s = new BackgroundLCD();
			break;

		case IDFOND_WORLDMAP:
			s = new BackgroundWorldmap();
			break;

		case IDFOND_ELECTRONIC1:
			s = new BackgroundElectronic1();
			break;

		case IDFOND_ELECTRONIC2:
			s = new BackgroundElectronic2();
			break;

		case IDFOND_ELECTRONIC3:
			s = new BackgroundElectronic3();
			break;

		case IDFOND_ELECTRONIC4:
			s = new BackgroundElectronic4();
			break;

		case IDFOND_ECRANNEIGE:
			s = new BackgroundSnowScreen();
			break;

		case IDFOND_ECRANCAMERA:
			s = new BackgroundCameraScreen();
			break;

		case IDFOND_ASSEMBLEUR:
			s = new BackgroundAssembler();
			break;

		case IDFOND_WINDOWS:
			s = new BackgroundWindows();
			break;

		case IDFOND_DOS:
			s = new BackgroundDos();
			break;

		case IDFOND_BOSSVENTILO:
			s = new BackgroundBossFan();
			break;

		case IDFOND_BATTLECRUISER:
			s = new BackgroundBattleCruiser();
			break;

		case IDFOND_ROCHER_HYPERSPACE:
			s = new BackgroundRockHyperspace();
			break;

		case IDFOND_MONOLITE:
			s = new BackgroundMonolith();
			plat_mobile = true;
			break;

		case IDFOND_ROCHER1:
			s = new BackgroundRock1();
			plat_mobile = true;
			break;

		case IDFOND_ROCHER2:
			s = new BackgroundRock2();
			plat_mobile = true;
			break;

		case IDFOND_ETOILE:
			s = new BackgroundStar();
			break;

		case 900:
			s = new BackgroundBomb();
			break;
	}

	if (s != NULL) {
		s->x = x;
		s->y = y;

		if (plat_mobile)
			list_plateformes_mobiles.emplace_back(s);
		else
			list_fonds_animes.emplace_back(s);
	} else {
		debug << "Erreur : identité de fond animé inconnue (" << id_fond << ")\n";
	}
}
