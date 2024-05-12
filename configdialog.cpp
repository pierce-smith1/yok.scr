#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#include <algorithm>

#include "configdialog.h"

ConfigDialog::ConfigDialog(HWND dialog)
	: m_dialog(dialog), m_current_config(m_registry.get_config())
{
	HWND pattern_box = GetDlgItem(m_dialog, IDC_YONK_PATTERN);
	for (std::map<SpritePattern, std::string>::iterator entry = pattern_strings.begin(); entry != pattern_strings.end(); ++entry) {
		// They saw in the API great awkwardness...
		// So they solved the problem by - well, get this -
		// Wrapping the function they could have better designed
		// In a _macro_ that hides all its dubious lies...
		ComboBox_AddString(pattern_box, entry->second.c_str());
	}

	refresh();
}

void ConfigDialog::save() {
	Registry registry;

	for (int i = _CONFIG_OPTIONS_START + 1; i < _CONFIG_OPTIONS_SIZE; i++) {
		registry.write((ConfigOption) i, m_current_config[i]);
	}
}

BOOL ConfigDialog::command(WPARAM wparam, LPARAM lparam) {
	switch (LOWORD(wparam)) {
		case IDOK:
			save();
		case IDCANCEL: {
			EndDialog(m_dialog, LOWORD(wparam) == IDOK);
			return TRUE;
		}
		case IDC_DEFAULTS_BUTTON: {
			m_current_config = cfg_defaults;
			refresh();
			return FALSE;
		}
		case IDC_YONK_PATTERN: {
			if (HIWORD(wparam) == CBN_SELENDOK) {
				return combobox_changed((HWND) lparam);
			}
		}
	}


	return FALSE;
}

BOOL ConfigDialog::slider_changed(WPARAM wparam, HWND slider) {
	int value = SendMessage(slider, TBM_GETPOS, 0, 0);

	ConfigOption opt = reverse_lookup(slider_ids, GetDlgCtrlID(slider));
	m_current_config[opt] = decodef(value);

	return FALSE;
}

BOOL ConfigDialog::combobox_changed(HWND combobox) {
	char str[1 << 6] = {0};
	ComboBox_GetText(combobox, str, 1 << 6);
	std::string pattern_name = str;

	m_current_config[YonkPattern] = (float) reverse_lookup(pattern_strings, pattern_name);
	OutputDebugString(pattern_name.c_str());

	return FALSE;
}

// Continuous sliders use integers only...
// What Redmond designer responds to this folly?!
int ConfigDialog::encodef(float value) {
	return (int) (value * 8.0f);
}

float ConfigDialog::decodef(int value) {
	return value / 8.0f;
}

void ConfigDialog::refresh() {
	for (ConfigRanges::iterator entry = config_ranges.begin(); entry != config_ranges.end(); ++entry) {
		HWND slider = GetDlgItem(m_dialog, slider_ids[entry->first]);

		SendMessage(slider, TBM_SETRANGEMIN, TRUE, encodef(entry->second.first));
		SendMessage(slider, TBM_SETRANGEMAX, TRUE, encodef(entry->second.second));
		SendMessage(slider, TBM_SETPOS, TRUE, encodef(m_current_config[entry->first]));
	}

	HWND pattern_box = GetDlgItem(m_dialog, IDC_YONK_PATTERN);
	ComboBox_SelectString(pattern_box, -1, pattern_strings[
		(SpritePattern) (int) m_current_config[YonkPattern]].c_str()
	);
}

std::map<ConfigOption, std::pair<float, float> > get_config_ranges() {
	std::map<ConfigOption, std::pair<float, float> > ranges;

	ranges[YonkHomeDrift] = std::make_pair(0.0f, 5.0f);
	ranges[YonkEmotionScale] = std::make_pair(0.0f, 10.0f);
	ranges[TimeDivisor] = std::make_pair(10.0f, 300.0f);
	ranges[MaxColors] = std::make_pair(2.0f, 47.0f);
	ranges[SpriteCount] = std::make_pair(10.f, 100.0f);
	ranges[SpriteSize] = std::make_pair(10.0f, 100.0f);
	ranges[YonkShakeFactor] = std::make_pair(0.0f, 5.0f);
	ranges[PatternChangeInterval] = std::make_pair(15.0f, 60.0f * 15.0f);

	return ranges;
}

std::map<ConfigOption, int> get_slider_ids() {
	std::map<ConfigOption, int> ids;

	ids[YonkStepSize] = 0;
	ids[YonkHomeDrift] = IDC_YONK_HOME_DRIFT;
	ids[YonkEmotionScale] = IDC_YONK_EMOTION_SCALE;
	ids[TimeDivisor] = IDC_TIME_DIVISOR;
	ids[MaxColors] = IDC_MAX_COLORS;
	ids[SpriteCount] = IDC_SPRITE_COUNT;
	ids[SpriteSize] = IDC_SPRITE_SIZE;
	ids[YonkShakeFactor] = IDC_YONK_SHAKE_FACTOR;
	ids[YonkPattern] = IDC_YONK_PATTERN;
	ids[PatternChangeInterval] = IDC_PATTERN_CHANGE_INTERVAL;

	return ids;
}

std::map<SpritePattern, std::string> get_pattern_strings() {
	std::map<SpritePattern, std::string> pattern_strings;

	pattern_strings[Roamers] = "In the Wind";
	pattern_strings[Waves] = "Waves";
	pattern_strings[Square] = "Right Angles";
	pattern_strings[Bouncy] = "Bouncy";
	pattern_strings[Lissajous] = "Lissajous";
	pattern_strings[Rose] = "Rose";
	pattern_strings[Lattice] = "Lattice";

	return pattern_strings;
}