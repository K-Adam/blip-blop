#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "core/task.h"
#include "engine/graphics.h"

#define MAX_JOY 5

// A quarter of the travel, so drifting sticks do not register
#define DEAD_ZONE 8000

// Pad binding codes are PAD_CODE(slot, b), where b is an SDL_GameControllerButton,
// a trigger (PAD_LT, PAD_RT) or a direction (JOY_*) that combines the D-pad and the left stick
#define PAD_CODE(slot, b) ((((slot) + 1) << 10) | (b))
#define PAD_LT SDL_CONTROLLER_BUTTON_MAX
#define PAD_RT (SDL_CONTROLLER_BUTTON_MAX + 1)
#define PAD_BUTTONS (SDL_CONTROLLER_BUTTON_MAX + 2)

#define JOY_UP 100
#define JOY_DOWN 101
#define JOY_LEFT 102
#define JOY_RIGHT 103

// Mouse buttons use a slot far above the pads, b is an SDL_BUTTON_* index
#define MOUSE_SLOT 0x80
#define MOUSE_CODE(b) ((MOUSE_SLOT << 10) | (b))
// The wheel reads as two buttons held for a few frames after a scroll, so at least one game update sees them
#define MOUSE_WHEEL_UP 6
#define MOUSE_WHEEL_DOWN 7
#define MOUSE_BUTTONS 8

// Device filters for heldCode
#define DEV_KEYS 1
#define DEV_MOUSE 2
#define DEV_PADS 4
#define DEV_ALL 7

inline int codeSlot(unsigned int k) { return k >> 10 & 0xFF; }

struct Pad {
	SDL_GameController *ctrl = nullptr;
	SDL_JoystickID id = -1;
	bool buttons[PAD_BUTTONS] = {};
	bool left = false, right = false, up = false, down = false;
	int rx = 0, ry = 0;
};

class Input {
   private:
	struct State {
		Pad pads[MAX_JOY];
		char buffer[256] = {};
		char specialsbuffer[0x1000] = {};
		bool mouse[MOUSE_BUTTONS] = {};
	};
	State cur_, prev_;
	int mouse_x_ = 0, mouse_y_ = 0;
	bool mouse_moved_ = false, new_press_ = false;
	std::uint32_t wheel_until_[2] = {};
	unsigned int aliastab[256] = {};
	std::vector<std::pair<std::uint32_t, SDL_Keycode>> script_;
	bool quit_ = false;
	int autoquit_frames_ = 0;

	static int scan(const State &s, unsigned int k);
	static int scanMenu(const State &s, unsigned int k);
	Pad *padById(SDL_JoystickID id);

   public:
	// Menu keys: any pad or a left click also counts as the arrows, Enter and Escape
	int scanKey(unsigned int k) const { return scanMenu(cur_, k); }
	bool pressedKey(unsigned int k) const { return scanMenu(cur_, k) && !scanMenu(prev_, k); }

	// Exact binding codes, used by the control schemes so one device never drives another player
	int scanCode(unsigned int k) const { return scan(cur_, k); }
	bool pressedCode(unsigned int k) const { return scan(cur_, k) && !scan(prev_, k); }
	bool pressedAlias(int a) const { return pressedCode(aliastab[a]); }

	// Code of the first held input among the devices, -1 if none
	int heldCode(int devices = DEV_ALL) const;
	unsigned int getAlias(int n) const { return aliastab[n]; }
	int mouseX() const { return mouse_x_; }
	int mouseY() const { return mouse_y_; }
	bool mouseMoved() const { return mouse_moved_; }
	// A key, mouse button or pad button went down this frame, unlike the held state of anyKeyPressed
	bool anyNewPress() const { return new_press_; }
	void padStick(int slot, int &x, int &y) const {
		x = cur_.pads[slot].rx;
		y = cur_.pads[slot].ry;
	}

	bool quitRequested() const { return quit_; }
	// Exits after that many frames, set by /autoquit
	void setAutoquit(int frames) { autoquit_frames_ = frames; }
	// Unattended runs must not block on dialogs or write the player's files
	bool isTestRun() const { return autoquit_frames_ > 0; }

	bool open();
	void setAlias(int a, unsigned int val);

	Task<int> waitKey();

	Task<> waitClean();
	// "<frame>:<key>,..." presses each key at that frame and releases it two frames later
	bool setScript(std::string_view script);
	void update();
	void close();
	bool anyKeyPressed() const;
};

#ifndef BENINPUT_CPP_FILE
extern Input in;
#endif
