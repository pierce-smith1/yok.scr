#pragma once

#include <Windows.h>

#include "config.h"
#include "resourcew.h"
#include "spritecontrol.h"

class ConfigDialog {
public:
	ConfigDialog(HWND dialog);

	void save();
	BOOL command(WPARAM wparam, LPARAM lparam);
	BOOL slider_changed(WPARAM wparam, HWND slider);
	BOOL checkbox_checked(WPARAM wparam, HWND checkbox, const Cfg::Definition &option);
	BOOL combobox_changed(HWND combobox, int option);
	
private:
	int encodef(float value);
	float decodef(int value);
	void refresh();
	
	template <typename K, typename V> K reverse_lookup(const std::map<K, V> &map, V value) {
		return std::find_if(map.begin(), map.end(), [=](const auto &entry) {
			return entry.second == value;
		})->first;
	}

	HWND m_dialog;
	Config m_current_config;
	ConfigStore m_current_store;
};

class PaletteCustomizeDialog {
public:
	PaletteCustomizeDialog(HWND dialog);

	BOOL command(WPARAM wparam, LPARAM lparam);
	HBRUSH handle_color_button_message(WPARAM wparam, LPARAM lparam);

private:
	void refresh();
	void refresh_palette_list();

	void update_current_palette();
	void save_current_palette();

	void apply_palette_to_preview(const PaletteData &palette);
	int palette_index_for_control(int color_button_control_id);
	void get_and_save_color(int palette_index);

	std::optional<std::wstring> m_selected_group;

	HWND m_dialog;
	HANDLE m_preview_bitmap;
	PaletteData m_current_palette;
	std::wstring m_current_palette_name;
	PaletteRepository m_palette_repo;
	PaletteGroupRepository m_group_repo;
};

const static std::map<PatternName, std::wstring> pattern_strings = {
	{ Roamers, L"In the Wind" },
	{ Waves, L"Waves" },
	{ Square, L"Right Angles" },
	{ Bouncy, L"Bouncy" },
	{ Lissajous, L"Lissajous" },
	{ Rose, L"Rose" },
	{ Lattice, L"Lattice" },
	{ Bubbles, L"Bubbles" },
};

const static std::map<PaletteGroup, std::wstring> palette_strings = {
	{ PaletteGroup::All, L"All" },
	{ PaletteGroup::Canon, L"Canon" },
	{ PaletteGroup::NonCanon, L"Non-Canon" },
	{ PaletteGroup::RandomlyGenerated, L"I'm Feeling Lucky" },
};