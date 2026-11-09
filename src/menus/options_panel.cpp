#include "menus/options_panel.h"

#include "core/config.h"
#include "engine/control_alias.h"
#include "engine/input.h"
#include "core/text_data.h"
#include "menus/txt_defines.h"

#include "engine/graphics.h"

OptionsPanel::OptionsPanel() {
	vsync_on_txt_ = txt_data[TXT_VSYNC] + " " + txt_data[TXT_ON];
	vsync_off_txt_ = txt_data[TXT_VSYNC] + " " + txt_data[TXT_OFF];

	items_.AddEntry("VSYNC");
	items_.AddEntry(txt_data[TXT_FULLSCREEN]);
	items_.AddEntry("KEYBOARD CONTROLS");
	items_.AddEntry("MOUSE AND KEYBOARD");
	items_.AddEntry("CONTROLLER");
	items_.AddEntry(txt_data[TXT_RETURN]);
	RefreshVsync();
}

void OptionsPanel::RefreshVsync() { items_.ChangeEntry(0, vSyncOn ? vsync_on_txt_ : vsync_off_txt_); }

void OptionsPanel::ToggleVsync() {
	vSyncOn = !vSyncOn;
	graphics.SetVSync(vSyncOn);
	RefreshVsync();
}

int OptionsPanel::ProcessEvent() {
	if (items_.focused() == 0 && (in.pressedKey(DIK_RIGHT) || in.pressedAlias(SCHEME_KEYBOARD + ACTION_RIGHT) || in.pressedKey(DIK_LEFT) ||
								  in.pressedAlias(SCHEME_KEYBOARD + ACTION_LEFT))) {
		ToggleVsync();
	}
	if (items_.Navigate()) {
		switch (items_.focused()) {
			case 0:
				ToggleVsync();
				break;
			case 1:
				fullscreen = !fullscreen;
				graphics.ToggleFullscreen();
				break;
			case 2:
				return MenuType::Keys_Keyboard;
			case 3:
				return MenuType::Keys_Mouse;
			case 4:
				return MenuType::Keys_Pad;
			case 5:
				return MenuType::Main;
		}
	}
	return MenuType::Options;
}
