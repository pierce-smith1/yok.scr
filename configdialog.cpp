#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#include <algorithm>

#include "configdialog.h"

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
		registry.write(entry.first, entry.second);
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
			m_current_config = cfg_defaults;
			refresh();
			return FALSE;
		}
		case IDC_YONK_PATTERN: {
			if (HIWORD(wparam) == CBN_SELENDOK) {
				return combobox_changed((HWND) lparam);
			}
		}
		case IDC_YONK_PALETTE: {
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
	wchar_t str[1 << 6];
	ComboBox_GetText(combobox, str, 1 << 6);
	std::wstring pattern_name(str);
	std::wstring palette_name(str);

	m_current_config[YonkPattern] = (float) reverse_lookup(pattern_strings, pattern_name);
	m_current_config[YonkPalette] = (float) reverse_lookup(palette_strings, palette_name);
	OutputDebugString(pattern_name.c_str());
	OutputDebugString(palette_name.c_str());

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
	for (const auto &entry : config_ranges) {
		HWND slider = GetDlgItem(m_dialog, slider_ids.at(entry.first));
		SendMessage(slider, TBM_SETRANGEMIN, TRUE, encodef(entry.second.first));
		SendMessage(slider, TBM_SETRANGEMAX, TRUE, encodef(entry.second.second));
		SendMessage(slider, TBM_SETPOS, TRUE, encodef(m_current_config.at(entry.first)));
	}

	HWND pattern_box = GetDlgItem(m_dialog, IDC_YONK_PATTERN);
	ComboBox_SelectString(pattern_box, -1, pattern_strings.at(
		(SpritePattern) m_current_config.at(YonkPattern)).c_str()
	);

	HWND palette_box = GetDlgItem(m_dialog, IDC_YONK_PALETTE);
	ComboBox_SelectString(palette_box, -1, palette_strings.at(
		(PaletteGroup) m_current_config.at(YonkPalette)).c_str()
	);
}