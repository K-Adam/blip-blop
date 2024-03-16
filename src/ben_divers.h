#pragma once

extern bool app_killed;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Blocking loops must hand control back to the browser each frame, ASYNCIFY resumes them
inline void yieldToBrowser() {
#ifdef __EMSCRIPTEN__
	emscripten_sleep(0);
#endif
}
