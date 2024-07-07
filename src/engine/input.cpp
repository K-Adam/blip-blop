#define BENINPUT_CPP_FILE

#include "core/misc.h"
#include "engine/input.h"

#include "core/debug.h"

Input in;

// A quarter of the travel, so drifting sticks and resting triggers do not register
static const int DEAD_ZONE = 8000;
static const int TRIGGER_THRESHOLD = 8192;

bool Input::open() {
	if (SDL_GameControllerAddMappingsFromFile("assets/gamecontrollerdb.txt") < 0) {
		debug << "Cannot load controller mappings: " << SDL_GetError() << "\n";
	}
	// Already connected pads arrive as SDL_CONTROLLERDEVICEADDED events
	return true;
}

Pad* Input::padById(SDL_JoystickID id) {
	for (Pad& p : cur_.pads) {
		if (p.ctrl && p.id == id) return &p;
	}
	return nullptr;
}

void Input::update() {
	prev_ = cur_;
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				app_killed = true;
				break;

			case SDL_KEYDOWN:
			case SDL_KEYUP: {
				char down = e.type == SDL_KEYDOWN;
				SDL_Keycode k = e.key.keysym.sym;
				// Unicode keycodes of non Latin layouts would collide with pad codes
				if (k >= 255 && !(k & SDLK_SCANCODE_MASK)) k = SDL_SCANCODE_TO_KEYCODE(e.key.keysym.scancode);
				if (k >= 0 && k < 255) {
					cur_.buffer[k] = down;
				} else {
					cur_.specialsbuffer[k & 0xFFF] = down;
				}
				break;
			}

			case SDL_CONTROLLERDEVICEADDED: {
				SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(e.cdevice.which);
				if (padById(id)) break;
				for (Pad& p : cur_.pads) {
					if (p.ctrl) continue;
					p.ctrl = SDL_GameControllerOpen(e.cdevice.which);
					if (p.ctrl) {
						p.id = id;
						debug << "Controller connected: " << SDL_GameControllerName(p.ctrl) << "\n";
					}
					break;
				}
				break;
			}

			case SDL_CONTROLLERDEVICEREMOVED:
				if (Pad* p = padById(e.cdevice.which)) {
					SDL_GameControllerClose(p->ctrl);
					*p = Pad();
				}
				break;

			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				if (Pad* p = padById(e.cbutton.which)) {
					p->buttons[e.cbutton.button] = e.cbutton.state == SDL_PRESSED;
				}
				break;

			case SDL_CONTROLLERAXISMOTION:
				if (Pad* p = padById(e.caxis.which)) {
					int v = e.caxis.value;
					switch (e.caxis.axis) {
						case SDL_CONTROLLER_AXIS_LEFTX:
							p->left = v < -DEAD_ZONE;
							p->right = v > DEAD_ZONE;
							break;
						case SDL_CONTROLLER_AXIS_LEFTY:
							p->up = v < -DEAD_ZONE;
							p->down = v > DEAD_ZONE;
							break;
						case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
							p->buttons[PAD_LT] = v > TRIGGER_THRESHOLD;
							break;
						case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
							p->buttons[PAD_RT] = v > TRIGGER_THRESHOLD;
							break;
					}
				}
				break;
		}
	}
}

int Input::pressedCode() const {
	for (int i = 0; i < 255; i++) {
		if (cur_.buffer[i]) return i;
	}
	for (int i = 0; i < 0x1000; i++) {
		if (cur_.specialsbuffer[i]) return i | 0x40000000;
	}
	for (int s = 0; s < MAX_JOY; s++) {
		int base = (s + 1) << 10;
		for (int d = JOY_UP; d <= JOY_RIGHT; d++) {
			if (scanKey(base + d)) return base + d;
		}
		for (int b = 0; b < PAD_BUTTONS; b++) {
			bool dpad = b >= SDL_CONTROLLER_BUTTON_DPAD_UP && b <= SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
			if (!dpad && cur_.pads[s].buttons[b]) return base + b;
		}
	}
	return -1;
}

unsigned int Input::waitKey() {
	waitClean();
	int k;
	while ((k = pressedCode()) < 0 && !app_killed) {
		yieldToBrowser();
		update();
	}
	return k < 0 ? 0 : k;
}

void Input::waitClean() {
	update();
	while (pressedCode() >= 0 && !app_killed) {
		yieldToBrowser();
		update();
	}
}

void Input::setAlias(int a, unsigned int val) { aliastab[a] = val; }

void Input::close() {
	for (Pad& p : cur_.pads) {
		if (p.ctrl) SDL_GameControllerClose(p.ctrl);
		p = Pad();
	}
}

bool Input::anyKeyPressed() const { return pressedCode() >= 0; }

// Pad N drives player N with a standard layout until a binding takes over that player or that pad
unsigned int Input::padDefault(int a) const {
	int player = a / 10, i = a % 10;
	if (player > 1 || i >= 7) return 0;
	for (int b = 0; b < 17; b++) {
		int slot = aliastab[b] >> 10 & 0xFF;
		if (b % 10 < 7 && slot && (slot == player + 1 || b / 10 == player)) return 0;
	}
	static const unsigned int layout[7] = {JOY_UP, JOY_DOWN, JOY_LEFT, JOY_RIGHT, SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B};
	return ((player + 1) << 10) | layout[i];
}

int Input::scan(const State& st, unsigned int k) {
	if (k == 0) return 0;

	int j = k >> 10 & 0xFF;
	if (j > 0) {
		if (j > MAX_JOY) return 0;
		const Pad& p = st.pads[j - 1];
		int b = k & 0x3FF;
		switch (b) {
			case JOY_UP:
				return p.up || p.buttons[SDL_CONTROLLER_BUTTON_DPAD_UP];
			case JOY_DOWN:
				return p.down || p.buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN];
			case JOY_LEFT:
				return p.left || p.buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT];
			case JOY_RIGHT:
				return p.right || p.buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT];
		}
		return b < PAD_BUTTONS && p.buttons[b];
	}

	// Any pad can navigate, confirm and pause, so menus, cutscenes and the hi-score entry work without a keyboard
	int joy = k == DIK_UP ? JOY_UP : k == DIK_DOWN ? JOY_DOWN : k == DIK_LEFT ? JOY_LEFT : k == DIK_RIGHT ? JOY_RIGHT : 0;
	for (int s = 0; s < MAX_JOY; s++) {
		const Pad& p = st.pads[s];
		if (joy && scan(st, ((s + 1) << 10) | joy)) return 1;
		if (k == DIK_RETURN && p.buttons[SDL_CONTROLLER_BUTTON_A]) return 1;
		if (k == DIK_ESCAPE && (p.buttons[SDL_CONTROLLER_BUTTON_START] || p.buttons[SDL_CONTROLLER_BUTTON_BACK])) return 1;
	}

	if (k < 255) return st.buffer[k];
	return st.specialsbuffer[k & 0xFFF];
}
