#pragma once

extern bool app_killed;
// Frame count after which the game exits, 0 means never. Set by /autoquit
extern int autoquit_frames;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Blocking loops must hand control back to the browser each frame, ASYNCIFY resumes them
inline void yieldToBrowser() {
#ifdef __EMSCRIPTEN__
	emscripten_sleep(0);
#endif
}
