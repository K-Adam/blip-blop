#pragma once

#include "engine/graphics.h"

/*SDL KEYS*/

#define DIK_ESCAPE SDLK_ESCAPE
#define DIK_UP SDLK_UP
#define DIK_DOWN SDLK_DOWN
#define DIK_LEFT SDLK_LEFT
#define DIK_RIGHT SDLK_RIGHT
#define DIK_LCONTROL SDLK_LCTRL
#define DIK_LMENU SDLK_MENU
#define DIK_SPACE SDLK_SPACE
#define DIK_Q SDLK_q
#define DIK_W SDLK_w
#define DIK_E SDLK_e
#define DIK_R SDLK_r
#define DIK_T SDLK_t
#define DIK_Y SDLK_y
#define DIK_U SDLK_u
#define DIK_I SDLK_i
#define DIK_O SDLK_o
#define DIK_P SDLK_p
#define DIK_L SDLK_l
#define DIK_K SDLK_k
#define DIK_J SDLK_j
#define DIK_H SDLK_h
#define DIK_G SDLK_g
#define DIK_F SDLK_f
#define DIK_D SDLK_d
#define DIK_S SDLK_s
#define DIK_A SDLK_a
#define DIK_Z SDLK_z
#define DIK_X SDLK_x
#define DIK_C SDLK_c
#define DIK_V SDLK_v
#define DIK_B SDLK_b
#define DIK_N SDLK_n
#define DIK_M SDLK_m

#define DIK_TAB SDLK_TAB
#define DIK_F1 SDLK_F1
#define DIK_F2 SDLK_F2
#define DIK_F3 SDLK_F3
#define DIK_F4 SDLK_F4
#define DIK_F5 SDLK_F5
#define DIK_F6 SDLK_F6
#define DIK_F7 SDLK_F7
#define DIK_F8 SDLK_F8
#define DIK_F9 SDLK_F9
#define DIK_F10 SDLK_F10
#define DIK_F11 SDLK_F11
#define DIK_F12 SDLK_F12
#define DIK_F13 SDLK_F13
#define DIK_F14 SDLK_F14
#define DIK_F15 SDLK_F15

#define DIK_COMMA SDLK_COMMA
#define DIK_PERIOD SDLK_PERIOD
#define DIK_SLASH SDLK_SLASH
#define DIK_RSHIFT SDLK_RSHIFT
#define DIK_MULTIPLY SDLK_KP_MULTIPLY
#define DIK_CAPITAL SDLK_CAPSLOCK
#define DIK_NUMLOCK SDLK_NUMLOCKCLEAR

#define DIK_RETURN SDLK_RETURN

#define DIK_0 SDLK_0
#define DIK_1 SDLK_1
#define DIK_2 SDLK_2
#define DIK_3 SDLK_3
#define DIK_4 SDLK_4
#define DIK_5 SDLK_5
#define DIK_6 SDLK_6
#define DIK_7 SDLK_7
#define DIK_8 SDLK_8
#define DIK_9 SDLK_9

#define DIK_MINUS SDLK_MINUS
#define DIK_EQUALS SDLK_EQUALS
#define DIK_BACK SDLK_BACKSPACE

#define DIK_LBRACKET SDLK_LEFTBRACKET
#define DIK_RBRACKET SDLK_RIGHTBRACKET

#define DIK_SEMICOLON SDLK_SEMICOLON
#define DIK_APOSTROPHE SDLK_QUOTE
/*#define DIK_GRAVE SDLK_EXCLAIM*/
#define DIK_LSHIFT SDLK_LSHIFT
#define DIK_BACKSLASH SDLK_BACKSLASH
#define DIK_SCROLL SDLK_SCROLLLOCK
#define DIK_BACKSPACE SDLK_BACKSPACE
#define DIK_LALT SDLK_LALT

#define DIK_NUMPAD1 SDLK_KP_1
#define DIK_NUMPAD2 SDLK_KP_2
#define DIK_NUMPAD3 SDLK_KP_3
#define DIK_NUMPAD4 SDLK_KP_4
#define DIK_NUMPAD5 SDLK_KP_5
#define DIK_NUMPAD6 SDLK_KP_6
#define DIK_NUMPAD7 SDLK_KP_7
#define DIK_NUMPAD8 SDLK_KP_8
#define DIK_NUMPAD9 SDLK_KP_9
#define DIK_NUMPAD0 SDLK_KP_0
#define DIK_ADD SDLK_KP_PLUS
#define DIK_SUBTRACT SDLK_MINUS
#define DIK_DECIMAL SDLK_KP_DECIMAL
#define DIK_NUMPADENTER SDLK_KP_ENTER
#define DIK_DIVIDE SDLK_KP_DIVIDE
#define DIK_RMENU SDLK_RALT
#define DIK_PRIOR SDLK_PAGEUP
#define DIK_NEXT SDLK_PAGEDOWN
#define DIK_DELETE SDLK_DELETE
#define DIK_INSERT SDLK_INSERT
#define DIK_HOME SDLK_HOME
#define DIK_END SDLK_END
#define DIK_RCONTROL SDLK_RCTRL

#define MAX_JOY 5

// Pad binding codes are ((slot + 1) << 10) | b, where b is an SDL_GameControllerButton,
// a trigger (PAD_LT, PAD_RT) or a direction (JOY_*) that combines the D-pad and the left stick
#define PAD_LT SDL_CONTROLLER_BUTTON_MAX
#define PAD_RT (SDL_CONTROLLER_BUTTON_MAX + 1)
#define PAD_BUTTONS (SDL_CONTROLLER_BUTTON_MAX + 2)

#define JOY_UP 100
#define JOY_DOWN 101
#define JOY_LEFT 102
#define JOY_RIGHT 103

struct Pad {
	SDL_GameController *ctrl = nullptr;
	SDL_JoystickID id = -1;
	bool buttons[PAD_BUTTONS] = {};
	bool left = false, right = false, up = false, down = false;
};

extern bool app_killed;

class Input {
   private:
	struct State {
		Pad pads[MAX_JOY];
		char buffer[256] = {};
		char specialsbuffer[0x1000] = {};
	};
	State cur_, prev_;
	unsigned int aliastab[256] = {};

	static int scan(const State &s, unsigned int k);
	unsigned int padDefault(int a) const;
	Pad *padById(SDL_JoystickID id);

   public:
	int scanKey(unsigned int k) const { return scan(cur_, k); }
	int scanAlias(int a) const { return scanKey(aliastab[a]) || scanKey(padDefault(a)); }

	// True only on the update where the key went down
	bool pressedKey(unsigned int k) const { return scan(cur_, k) && !scan(prev_, k); }
	bool pressedAlias(int a) const { return pressedKey(aliastab[a]) || pressedKey(padDefault(a)); }

	// Code of the first key or pad button held, -1 if none
	int pressedCode() const;
	unsigned int getAlias(int n) const { return aliastab[n]; }

	bool open();
	void setAlias(int a, unsigned int val);

	unsigned int waitKey();

	void waitClean();
	void update();
	void close();
	bool anyKeyPressed() const;
};

#ifndef BENINPUT_CPP_FILE
extern Input in;
#endif
