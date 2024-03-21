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
	long long encodef(double value);
	double decodef(long long value);
	void refresh();
	
	template <typename K, typename V> K reverse_lookup(const std::map<K, V> &map, V value) {
		return std::find_if(map.begin(), map.end(), [=](const auto &entry) {
			return entry.second == value;
		})->first;
	}

	Registry m_registry;
	HWND m_dialog;
	Config m_current_config;
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