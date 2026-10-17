#include "level/backgrounds/background_worldmap.h"

const int anim_Worldmap[] = {0, 1, 0, 1, 0, 1, 2, 3, 4, 5, 6, 7, 6, 7, 6, 7, 8, 9, 10, 11, 12, 13, 12, 13, 12, 13, 12, 13, 13, 13, 13, 13, 13, 13, 13};

void BackgroundWorldmap::update() { pic = pbk_level.anim("worldmap")[animate(anim_Worldmap, 35, 10)]; }
