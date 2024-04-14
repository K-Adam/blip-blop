#include "menus/options_panel.h"

#include "core/config.h"
#include "engine/control_alias.h"
#include "engine/input.h"
#include "core/text_data.h"
#include "menus/txt_defines.h"

#include "engine/dd_gfx.h"

OptionsPanel::OptionsPanel() {
	vsync_on_txt_ = txt_data[TXT_VSYNC] + " " + txt_data[TXT_ON];
	vsync_off_txt_ = txt_data[TXT_VSYNC] + " " + txt_data[TXT_OFF];

	items_.AddEntry("VSYNC");
	items_.AddEntry(txt_data[TXT_FULLSCREEN]);
	items_.AddEntry(txt_data[TXT_P1KEYS]);
	items_.AddEntry(txt_data[TXT_P2KEYS]);
	items_.AddEntry(txt_data[TXT_RETURN]);
	RefreshVsync();
}

void OptionsPanel::RefreshVsync() { items_.ChangeEntry(0, vSyncOn ? vsync_on_txt_ : vsync_off_txt_); }

void OptionsPanel::ToggleVsync() {
	vSyncOn = !vSyncOn;
	DDSetVSync(vSyncOn);
	RefreshVsync();
}

int OptionsPanel::ProcessEvent() {
	if (in.scanKey(DIK_UP) || in.scanAlias(ALIAS_P1_UP)) {
		items_.MoveUp();
	} else if (in.scanKey(DIK_DOWN) || in.scanAlias(ALIAS_P1_DOWN)) {
		items_.MoveDown();
	}
	if (items_.focused() == 0 && (in.scanKey(DIK_RIGHT) || in.scanAlias(ALIAS_P1_RIGHT) || in.scanKey(DIK_LEFT) || in.scanAlias(ALIAS_P1_LEFT))) {
		ToggleVsync();
	}
	if (in.scanKey(DIK_RETURN) || in.scanAlias(ALIAS_P1_FIRE)) {
		switch (items_.focused()) {
			case 0:
				ToggleVsync();
				break;
			case 1:
				fullscreen = !fullscreen;
				DDToggleFullscreen();
				break;
			case 2:
				return MenuType::Keys_1;
			case 3:
				return MenuType::Keys_2;
			case 4:
				return MenuType::Main;
		}
	}
	return MenuType::Options;
}
