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

	HWND m_dialog;
	Config m_current_config;
};

class PaletteCustomizeDialog {
public:
	PaletteCustomizeDialog(HWND dialog);

	BOOL command(WPARAM wparam, LPARAM lparam);
	HBRUSH handle_color_button_message(WPARAM wparam, LPARAM lparam);

	std::wstring export_palettes();
  
	static void apply_palette_to_preview(HWND dialog, HANDLE preview_bitmap, int preview_control_id, const PaletteData &palette);

	const static inline size_t MinPaletteNameSize = 2;
	const static inline size_t MaxPaletteNameSize = 64;
	const static inline Palettes::Definition DefaultPalette = Palettes::Friend;
	const static inline Palettes::Definition DisabledPalette = {
		.name = L"disabled",
		.data = new PaletteData {{
			"#aaaaaa",
			"#aaaaaa",
			"#aaaaaa",
			"#aaaaaa",
			"#aaaaaa",
			"#aaaaaa",
			"#aaaaaa",
		}},
	};

	struct CurrentPalette {
		PaletteData data;
		std::wstring name;
	};

private:
	void refresh();
	void refresh_palette_list();

	void update_current_palette();
	void save_current_palette();
	void save_current_palette_as_new();
	void delete_current_palette();

	int palette_index_for_control(int color_button_control_id);
	void get_and_save_color(int palette_index);

	std::optional<CurrentPalette> m_current_palette;

	HWND m_dialog;
	HANDLE m_preview_bitmap;
	PaletteRepository m_palette_repo;
	PaletteGroupRepository m_group_repo;
};

class PalettesImport {
public:
	static PalettesImport *parse_palettes_string(const std::wstring &import_string);
	std::optional<std::wstring> get_error_string(size_t max_errors);
	size_t get_valid_palettes_amount();
	std::wstring get_valid_palette_names(size_t max_length);
	PaletteCustomizeDialog::CurrentPalette import_palettes();

	const static inline std::wstring palette_name_end = L"=";	// There is no name_start because that's a valid character from below
	const static inline std::wstring palette_color_start = L"#";
	const static inline std::wstring palette_color_end = L";";
	const static inline std::wstring input_terminator = L"+";

private:
	std::vector<std::wstring> palettes_name;
	std::vector<std::vector<std::wstring>> palettes_raw_colors;
	std::vector<unsigned int> palettes_has_errors;
	bool import_string_had_terminator;

	const static inline std::wstring valid_chars = L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789-_!?:()";	// Note: Spaces are valid characters but not included in this string
	const static inline std::wstring valid_hex_chars = L"0123456789abcdefABCDEF";
	const static inline size_t colors_amount = 7;

	const enum ParserErrors : unsigned int {
		error_none = 0,
		error_name_too_short = 1 << 0,
		error_name_too_long = 1 << 1,
		error_name_invalid_characters = 1 << 2,
		error_colors_too_few = 1 << 3,
		error_colors_incorrect_length = 1 << 4,
		error_colors_invalid_characters = 1 << 5,
		error_palette_contains_newlines = 1 << 6,
	};

	PalettesImport(std::vector<std::wstring> palettes_name,
				   std::vector<std::vector<std::wstring>> palettes_raw_colors,
				   std::vector<unsigned int> palettes_has_errors,
				   bool import_string_had_terminator);
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

LRESULT CALLBACK ScreenSaverPaletteCustomizeDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK ScreenSaverNewCustomPaletteDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK CustomColorDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK ScreenSaverImportExportPalettesDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK AddPredefinedPaletteDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam);

