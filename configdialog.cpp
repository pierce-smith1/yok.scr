#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#include <algorithm>

#include "configdialog.h"
#include "yokscr.h"

ConfigDialog::ConfigDialog(HWND dialog)
	: m_dialog(dialog), m_current_config(m_registry.get_config())
{ 
	HWND pattern_box = GetDlgItem(m_dialog, IDC_YONK_PATTERN);
	for (const auto &entry : pattern_strings) {
		// They saw in the API great awkwardness...
		// So they solved the problem by - well, get this -
		// Wrapping the function they could have better designed
		// In a _macro_ that hides all its dubious lies...
		ComboBox_AddString(pattern_box, entry.second.c_str());
	}

	HWND palette_box = GetDlgItem(m_dialog, IDC_YONK_PALETTE);
	for (const auto& entry : palette_strings) {
		ComboBox_AddString(palette_box, entry.second.c_str());
	}

	refresh();
}

void ConfigDialog::save() {
	Registry registry;

	for (const auto &entry : m_current_config) {
		registry.write(entry.first.name, entry.second);
	}
}

BOOL ConfigDialog::command(WPARAM wparam, LPARAM lparam) {
	switch (LOWORD(wparam)) {
		case IDOK:
			save();
			[[fallthrough]];
		case IDCANCEL: {
			EndDialog(m_dialog, LOWORD(wparam) == IDOK);
			return TRUE;
		}
		case IDC_DEFAULTS_BUTTON: {
			for (const ConfigOption &opt : ConfigOptions::All) {
				m_current_config[opt] = opt.default_;
			}
			refresh();
			return FALSE;
		}
		case IDC_YONK_PATTERN: {
			if (HIWORD(wparam) == CBN_SELENDOK) {
				return combobox_changed((HWND) lparam, IDC_YONK_PATTERN);
			}
			break;
		}
		case IDC_YONK_PALETTE: {
			if (HIWORD(wparam) == CBN_SELENDOK) {
				return combobox_changed((HWND) lparam, IDC_YONK_PALETTE);
			}
			break;
		}
		case IDC_PATTERN_FIX: {
			if (HIWORD(wparam) == BN_CLICKED) {
				return pattern_fix_checked(wparam, (HWND)lparam);
			}
		}
	}


	return FALSE;
}

BOOL ConfigDialog::slider_changed(WPARAM wparam, HWND slider) {
	int value = SendMessage(slider, TBM_GETPOS, 0, 0);

	ConfigOption opt = *std::find_if(ConfigOptions::All.begin(), ConfigOptions::All.end(), [&](const ConfigOption &opt) {
		return opt.dialog_control_id == GetDlgCtrlID(slider);
	});
	m_current_config[opt] = decodef(value);

	return FALSE;
}

BOOL ConfigDialog::combobox_changed(HWND combobox, int option_type) {
	wchar_t str[1 << 6];
	ComboBox_GetText(combobox, str, 1 << 6);
	std::wstring option_name(str);

	switch (option_type) {
		case IDC_YONK_PATTERN:
			m_current_config[ConfigOptions::YonkPattern] = (float) reverse_lookup(pattern_strings, option_name);
			break;
		case IDC_YONK_PALETTE:
			m_current_config[ConfigOptions::YonkPalette] = (float) reverse_lookup(palette_strings, option_name);
			break;
	}

	return FALSE;
}

BOOL ConfigDialog::pattern_fix_checked(WPARAM wparam, HWND checkbox) {
	bool checked = Button_GetCheck(checkbox) == BST_CHECKED;
	m_current_config[ConfigOptions::IsPatternFixed] = checked ? 1.0f : 0.0f;
	refresh();
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
	HWND version_label = GetDlgItem(m_dialog, IDC_VERSION_LABEL);
	std::wstring version_label_text = L"yokscr version " + YOKSCR_VERSION;
	SendMessage(version_label, WM_SETTEXT, NULL, (LPARAM) version_label_text.c_str());

	for (const ConfigOption &opt : ConfigOptions::All) {
		HWND slider = GetDlgItem(m_dialog, opt.dialog_control_id);

		SendMessage(slider, TBM_SETRANGEMIN, TRUE, encodef(opt.range.first));
		SendMessage(slider, TBM_SETRANGEMAX, TRUE, encodef(opt.range.second));
		SendMessage(slider, TBM_SETPOS, TRUE, encodef(m_current_config.at(opt)));
	}

	// these should probably be put somewhere else at some point
	if (m_current_config.at(ConfigOptions::YonkPattern) < 0 
	 || m_current_config.at(ConfigOptions::YonkPattern) >= (int) _PATTERN_COUNT) 
	{
		m_current_config.at(ConfigOptions::YonkPattern) = 0;
	}
	if (m_current_config.at(ConfigOptions::YonkPalette) < 0 
	 || m_current_config.at(ConfigOptions::YonkPalette) >= (int) PaletteGroup::_PALETTE_OPTION_COUNT) 
	{
		m_current_config.at(ConfigOptions::YonkPalette) = 0;
	}

	HWND pattern_box = GetDlgItem(m_dialog, IDC_YONK_PATTERN);
	ComboBox_SelectString(pattern_box, -1, pattern_strings.at(
		(PatternName) m_current_config.at(ConfigOptions::YonkPattern)).c_str()
	);

	HWND palette_box = GetDlgItem(m_dialog, IDC_YONK_PALETTE);
	ComboBox_SelectString(palette_box, -1, palette_strings.at(
		(PaletteGroup) m_current_config.at(ConfigOptions::YonkPalette)).c_str()
	);

	bool is_pattern_fixed = m_current_config.at(ConfigOptions::IsPatternFixed) == 1.0f;
	HWND pattern_interval_slider = GetDlgItem(m_dialog, IDC_PATTERN_CHANGE_INTERVAL);
	HWND pattern_fixed_check = GetDlgItem(m_dialog, IDC_PATTERN_FIX);
	Button_SetCheck(pattern_fixed_check, is_pattern_fixed);
	EnableWindow(pattern_interval_slider, !is_pattern_fixed);
}