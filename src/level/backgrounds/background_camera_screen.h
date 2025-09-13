#pragma once

#include "level/backgrounds/background_animated.h"

class BackgroundCameraScreen : public BackgroundAnimated {
   public:
	BackgroundCameraScreen() : BackgroundAnimated(50, 3, 87, false) {}
	void render() override;
};
