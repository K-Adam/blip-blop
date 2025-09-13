#include "level/backgrounds/background_star.h"

const int anim_star[] = {37, 37, 38, 39, 39, 40, 40, 40, 39, 39, 38, 37, 37, 36, 36, 36};

void BackgroundStar::update() { pic = pbk_level[animate(anim_star, 16, 3)]; }
