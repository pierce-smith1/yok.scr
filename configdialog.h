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
	BOOL pattern_fix_checked(WPARAM wparam, HWND checkbox);
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

	Registry m_registry;
	HWND m_dialog;
	Config m_current_config;
};

const static std::map<ConfigOption, std::pair<float, float>> config_ranges = {
	{ YonkHomeDrift, { 0.0f, 5.0f }},
	{ YonkEmotionScale, { 0.0f, 10.0f }},
	{ TimeDivisor, { 10.0f, 300.0f }},
	{ MaxColors, { 2.0f, 47.0f }},
	{ SpriteCount, { 10.f, 200.0f }},
	{ SpriteSize, { 10.0f, 200.0f }},
	{ YonkShakeFactor, { 0.0f, 5.0f }},
	{ PatternChangeInterval, { 30.0f, 60.0f * 30.0f }},
	{ ImpostorChance, {0.0f, 1.0f }},
};

const static std::map<ConfigOption, int> slider_ids = {
	{ YonkStepSize, 0 },
	{ YonkHomeDrift, IDC_YONK_HOME_DRIFT },
	{ YonkEmotionScale, IDC_YONK_EMOTION_SCALE },
	{ TimeDivisor, IDC_TIME_DIVISOR },
	{ MaxColors, IDC_MAX_COLORS },
	{ SpriteCount, IDC_SPRITE_COUNT },
	{ SpriteSize, IDC_SPRITE_SIZE },
	{ YonkShakeFactor, IDC_YONK_SHAKE_FACTOR },
	{ YonkPattern, IDC_YONK_PATTERN },
	{ PatternChangeInterval, IDC_PATTERN_CHANGE_INTERVAL },
	{ ImpostorChance, IDC_IMPOSTOR_CHANCE },
	{ YonkPalette, IDC_YONK_PALETTE },
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