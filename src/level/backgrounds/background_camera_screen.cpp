#include "level/world.h"
#include "core/assets.h"
#include "level/backgrounds/background_camera_screen.h"

void BackgroundCameraScreen::render() {
	Sprite::render();
	draw(x, y + anim_tick, pbk_level.pic("camera_screen_frame"));
}
