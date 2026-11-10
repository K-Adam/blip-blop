#define BENINPUT_CPP_FILE

#include <charconv>
#include <string>

#include "engine/input.h"

#include "core/debug.h"
#include "engine/chrono.h"
#include "engine/graphics.h"
#include "engine/key_translator.h"

Input in;

// A quarter of the travel, so resting triggers do not register
static const int TRIGGER_THRESHOLD = 8192;

bool Input::open() {
	if (SDL_GameControllerAddMappingsFromFile("assets/gamecontrollerdb.txt") < 0) {
		debug << "Cannot load controller mappings: " << SDL_GetError() << "\n";
	}
	SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR));
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
	for (int w = 0; w < 2; w++) cur_.mouse[MOUSE_WHEEL_UP + w] = FrameClock::frames() < wheel_until_[w];
	mouse_moved_ = new_press_ = false;
	std::uint32_t frames_played = ++FrameClock::frames_;
	if (isTestRun() && (int)frames_played >= autoquit_frames_) quit_ = true;
	for (auto [frame, key] : script_) {
		if (frame != frames_played && frame + 2 != frames_played) continue;
		SDL_Event k{};
		k.type = frame == frames_played ? SDL_KEYDOWN : SDL_KEYUP;
		k.key.state = k.type == SDL_KEYDOWN ? SDL_PRESSED : SDL_RELEASED;
		k.key.keysym.sym = key;
		k.key.keysym.scancode = SDL_GetScancodeFromKey(key);
		SDL_PushEvent(&k);
	}
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				quit_ = true;
				break;

			case SDL_RENDER_TARGETS_RESET:
				graphics.MarkTargetsLost();
				break;

			case SDL_KEYDOWN:
			case SDL_KEYUP: {
				char down = e.type == SDL_KEYDOWN;
				if (down && !e.key.repeat) new_press_ = true;
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
					if (e.cbutton.state == SDL_PRESSED) new_press_ = true;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				// Higher buttons would read as the wheel
				if (e.button.button < MOUSE_WHEEL_UP) cur_.mouse[e.button.button] = e.button.state == SDL_PRESSED;
				if (e.button.state == SDL_PRESSED) new_press_ = true;
				break;

			case SDL_MOUSEWHEEL: {
				int dy = e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -e.wheel.y : e.wheel.y;
				if (dy) {
					int w = dy > 0 ? 0 : 1;
					cur_.mouse[MOUSE_WHEEL_UP + w] = true;
					wheel_until_[w] = FrameClock::frames() + 2;
				}
				break;
			}

			// SDL already reports these in the 640x480 logical space of the renderer
			case SDL_MOUSEMOTION:
				mouse_x_ = e.motion.x;
				mouse_y_ = e.motion.y;
				mouse_moved_ = true;
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
						case SDL_CONTROLLER_AXIS_RIGHTX:
							p->rx = v;
							break;
						case SDL_CONTROLLER_AXIS_RIGHTY:
							p->ry = v;
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

int Input::heldCode(int devices) const {
	if (devices & DEV_KEYS) {
		for (int i = 0; i < 255; i++) {
			if (cur_.buffer[i]) return i;
		}
		for (int i = 0; i < 0x1000; i++) {
			if (cur_.specialsbuffer[i]) return i | 0x40000000;
		}
	}
	if (devices & DEV_MOUSE) {
		for (int b = 1; b < MOUSE_BUTTONS; b++) {
			if (cur_.mouse[b]) return MOUSE_CODE(b);
		}
	}
	if (devices & DEV_PADS) {
		for (int s = 0; s < MAX_JOY; s++) {
			for (int d = JOY_UP; d <= JOY_RIGHT; d++) {
				if (scanCode(PAD_CODE(s, d))) return PAD_CODE(s, d);
			}
			for (int b = 0; b < PAD_BUTTONS; b++) {
				bool dpad = b >= SDL_CONTROLLER_BUTTON_DPAD_UP && b <= SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
				if (!dpad && cur_.pads[s].buttons[b]) return PAD_CODE(s, b);
			}
		}
	}
	return -1;
}

Task<int> Input::waitKey() {
	co_await waitClean();
	int k;
	while ((k = heldCode()) < 0 && !quit_) co_await nextFrame();
	co_return k < 0 ? 0 : k;
}

Task<> Input::waitClean() {
	while (heldCode() >= 0 && !quit_) co_await nextFrame();
}

void Input::setAlias(int a, unsigned int val) { aliastab[a] = val; }

void Input::close() {
	for (Pad& p : cur_.pads) {
		if (p.ctrl) SDL_GameControllerClose(p.ctrl);
		p = Pad();
	}
}

bool Input::anyKeyPressed() const { return heldCode() >= 0; }

int Input::scan(const State& st, unsigned int k) {
	if (k == 0) return 0;

	int j = codeSlot(k);
	if (j == MOUSE_SLOT) {
		int b = k & 0x3FF;
		return b < MOUSE_BUTTONS && st.mouse[b];
	}
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

	if (k < 255) return st.buffer[k];
	return st.specialsbuffer[k & 0xFFF];
}

int Input::scanMenu(const State& st, unsigned int k) {
	if (scan(st, k)) return 1;

	// Menus, cutscenes and the hi-score entry work with whatever device the player holds
	if (k == DIK_RETURN && st.mouse[SDL_BUTTON_LEFT]) return 1;
	int joy = k == DIK_UP ? JOY_UP : k == DIK_DOWN ? JOY_DOWN : k == DIK_LEFT ? JOY_LEFT : k == DIK_RIGHT ? JOY_RIGHT : 0;
	for (int s = 0; s < MAX_JOY; s++) {
		const Pad& p = st.pads[s];
		if (joy && scan(st, PAD_CODE(s, joy))) return 1;
		if (k == DIK_RETURN && p.buttons[SDL_CONTROLLER_BUTTON_A]) return 1;
		if (k == DIK_ESCAPE && (p.buttons[SDL_CONTROLLER_BUTTON_START] || p.buttons[SDL_CONTROLLER_BUTTON_BACK])) return 1;
	}
	return 0;
}

bool Input::setScript(std::string_view script) {
	while (!script.empty()) {
		size_t comma = script.find(',');
		std::string_view step = script.substr(0, comma);
		script = comma == std::string_view::npos ? std::string_view() : script.substr(comma + 1);
		size_t colon = step.find(':');
		if (colon == std::string_view::npos) return false;
		std::uint32_t frame = 0;
		auto [end, err] = std::from_chars(step.data(), step.data() + colon, frame);
		unsigned int key = codeFromName(std::string(step.substr(colon + 1)));
		bool keyboard = key < 255 || (key & SDLK_SCANCODE_MASK);
		if (err != std::errc() || end != step.data() + colon || frame == 0 || key == 0 || !keyboard) return false;
		script_.emplace_back(frame, SDL_Keycode(key));
	}
	return true;
}
