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
	BOOL combobox_changed(HWND combobox);
	
private:
	int encodef(float value);
	float decodef(int value);
	void refresh();
	
	template <typename K, typename V> K reverse_lookup(const std::map<K, V> &map, V value) {
		for (std::map<K, V>::iterator it = map.begin(); it != map.end(); ++it) {
			if (it->second == value) {
				return it->first;
			}
		}

		return K();
	}

	Registry m_registry;
	HWND m_dialog;
	Config m_current_config;
};

typedef std::map<ConfigOption, std::pair<float, float> > ConfigRanges;

ConfigRanges get_config_ranges();
std::map<ConfigOption, int> get_slider_ids();
std::map<SpritePattern, std::string> get_pattern_strings();

static ConfigRanges config_ranges = get_config_ranges();
static std::map<ConfigOption, int> slider_ids = get_slider_ids();
static std::map<SpritePattern, std::string> pattern_strings = get_pattern_strings();
