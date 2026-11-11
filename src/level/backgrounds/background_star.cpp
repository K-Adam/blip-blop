#include "level/backgrounds/background_star.h"

const int anim_star[] = {1, 1, 2, 3, 3, 4, 4, 4, 3, 3, 2, 1, 1, 0, 0, 0};

void BackgroundStar::update() { pic = assets().pics.level.anim("star_twinkle")[animate(anim_star, 16, 3)]; }
