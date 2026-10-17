#pragma once

#include "level/backgrounds/background_animated.h"

class BackgroundCameraScreen : public BackgroundAnimated {
   public:
	BackgroundCameraScreen() : BackgroundAnimated("camera_screen", 87, false) {}
	void render() override;
};
