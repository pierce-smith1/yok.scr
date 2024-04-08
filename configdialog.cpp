#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <gdiplus.h>
#include <ShObjIdl.h>

#include <algorithm>

#include "configdialog.h"
#include "yokscr.h"
#include "resourcew.h"
#include "graphics.h"
#include "noise.h"

ConfigDialog::ConfigDialog(HWND dialog)
	: m_dialog(dialog), m_current_config(Registry::get_config()) {
	HWND pattern_box = GetDlgItem(m_dialog, IDC_YONK_PATTERN);
	for (const auto &entry : pattern_strings) {
		// They saw in the API great awkwardness...
		// So they solved the problem by - well, get this -
		// Wrapping the function they could have better designed
		// In a _macro_ that hides all its dubious lies...
		ComboBox_AddString(pattern_box, entry.second.c_str());
	}

	HWND palette_box = GetDlgItem(m_dialog, IDC_YONK_PALETTE);
	for (const auto &entry : palette_strings) {
		ComboBox_AddString(palette_box, entry.second.c_str());
	}

	refresh();
}

void ConfigDialog::save() {
	Registry registry;

	for (const auto &opt : Cfg::All) {
		registry.write(opt.name, m_current_config[opt]);
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
			for (const auto &opt : Cfg::All) {
				m_current_config[opt] = opt.default_;
			}
			refresh();
			return FALSE;
		}
		case IDC_PALETTE_CUSTOMIZE: {
			DialogBox(
				NULL,
				MAKEINTRESOURCE(DLG_PALETTE_CUSTOMIZER),
				m_dialog,
				(DLGPROC) ScreenSaverPaletteCustomizeDialog
			);
			break;
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
				return checkbox_checked(wparam, (HWND) lparam, Cfg::IsPatternFixed);
			}
			break;
		}
		case IDC_PLAY_OVER_DESKTOP: {
			if (HIWORD(wparam) == BN_CLICKED) {
				return checkbox_checked(wparam, (HWND) lparam, Cfg::PlayOverDesktop);
			}
			break;
		}
		case IDC_CUSTOM_PALETTES_CHECK: {
			if (HIWORD(wparam) == BN_CLICKED) {
				return checkbox_checked(wparam, (HWND) lparam, Cfg::UseCustomPalettes);
			}
			break;
		}
		case IDC_TRAILS_ENABLED: {
			if (HIWORD(wparam) == BN_CLICKED) {
				return checkbox_checked(wparam, (HWND) lparam, Cfg::TrailsEnabled);
			}
			break;
		}
	}

	return FALSE;
}

BOOL ConfigDialog::slider_changed(WPARAM wparam, HWND slider) {
	auto value = SendMessage(slider, TBM_GETPOS, 0, 0);

	auto opt = *std::find_if(Cfg::All.begin(), Cfg::All.end(), [&](const auto &opt) {
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
			m_current_config[Cfg::Pattern] = (double) reverse_lookup(pattern_strings, option_name);
			break;
		case IDC_YONK_PALETTE:
			m_current_config[Cfg::Palette] = (double) reverse_lookup(palette_strings, option_name);
			break;
	}

	return FALSE;
}

BOOL ConfigDialog::checkbox_checked(WPARAM wparam, HWND checkbox, const Cfg::Definition &option) {
	bool checked = Button_GetCheck(checkbox) == BST_CHECKED;
	m_current_config[option] = checked ? 1.0 : 0.0;
	refresh();
	return FALSE;
}

// Continuous sliders use integers only...
// What Redmond designer responds to this folly?!
long long ConfigDialog::encodef(double value) {
	return cast<long long>(value * 8.0);
}

double ConfigDialog::decodef(long long value) {
	return value / 8.0;
}

void ConfigDialog::refresh() {
	HWND version_label = GetDlgItem(m_dialog, IDC_VERSION_LABEL);
	std::wstring version_label_text = L"yokscr version " + YOKSCR_VERSION;
	SendMessage(version_label, WM_SETTEXT, NULL, (LPARAM) version_label_text.c_str());

	for (const auto &opt : Cfg::All) {
		HWND slider = GetDlgItem(m_dialog, opt.dialog_control_id);

		SendMessage(slider, TBM_SETRANGEMIN, TRUE, encodef(opt.range.first));
		SendMessage(slider, TBM_SETRANGEMAX, TRUE, encodef(opt.range.second));
		SendMessage(slider, TBM_SETPOS, TRUE, encodef(m_current_config[opt]));
	}

	// these should probably be put somewhere else at some point
	if (m_current_config[Cfg::Pattern] < 0
	 || m_current_config[Cfg::Pattern] >= (int) _PATTERN_COUNT) {
		m_current_config[Cfg::Pattern] = 0;
	}
	if (m_current_config[Cfg::Palette] < 0
	 || m_current_config[Cfg::Palette] >= (int) PaletteGroup::_PALETTE_OPTION_COUNT) {
		m_current_config[Cfg::Palette] = 0;
	}

	HWND pattern_box = GetDlgItem(m_dialog, IDC_YONK_PATTERN);
	ComboBox_SelectString(pattern_box, -1, pattern_strings.at(
		(PatternName) m_current_config[Cfg::Pattern]).c_str()
	);

	HWND palette_box = GetDlgItem(m_dialog, IDC_YONK_PALETTE);
	ComboBox_SelectString(palette_box, -1, palette_strings.at(
		(PaletteGroup) m_current_config[Cfg::Palette]).c_str()
	);

	bool is_pattern_fixed = m_current_config[Cfg::IsPatternFixed] == 1.0;
	HWND pattern_interval_slider = GetDlgItem(m_dialog, IDC_PATTERN_CHANGE_INTERVAL);
	HWND pattern_fixed_check = GetDlgItem(m_dialog, IDC_PATTERN_FIX);
	Button_SetCheck(pattern_fixed_check, is_pattern_fixed);
	EnableWindow(pattern_interval_slider, !is_pattern_fixed);

	bool is_playing_over_desktop = m_current_config[Cfg::PlayOverDesktop] == 1.0;
	HWND play_over_desktop_check = GetDlgItem(m_dialog, IDC_PLAY_OVER_DESKTOP);
	Button_SetCheck(play_over_desktop_check, is_playing_over_desktop);

	bool are_trails_enabled = m_current_config[Cfg::TrailsEnabled] == 1.0;
	HWND trail_length_slider = GetDlgItem(m_dialog, IDC_TRAIL_LENGTH);
	HWND trail_space_slider = GetDlgItem(m_dialog, IDC_TRAIL_SPACE);
	HWND trails_enabled_check = GetDlgItem(m_dialog, IDC_TRAILS_ENABLED);
	Button_SetCheck(trails_enabled_check, are_trails_enabled);
	EnableWindow(trail_length_slider, are_trails_enabled);
	EnableWindow(trail_space_slider, are_trails_enabled);

	bool using_custom_palettes = m_current_config[Cfg::UseCustomPalettes] == 1.0;
	HWND palette_selector = GetDlgItem(m_dialog, IDC_YONK_PALETTE);
	HWND palette_customize_button = GetDlgItem(m_dialog, IDC_PALETTE_CUSTOMIZE);
	HWND custom_palettes_check = GetDlgItem(m_dialog, IDC_CUSTOM_PALETTES_CHECK);
	EnableWindow(palette_selector, !using_custom_palettes);
	EnableWindow(palette_customize_button, using_custom_palettes);
	Button_SetCheck(custom_palettes_check, using_custom_palettes);
}

PaletteCustomizeDialog::PaletteCustomizeDialog(HWND dialog)
	: m_current_preview_bitmap(Bitmaps::Lksix),
	m_dialog(dialog),
	// We use lksix because it shows off every color in the palette.
	m_preview_bitmap(Bitmaps::load_raw_resource(m_current_preview_bitmap.resource_id)),
	// The "Friend" palette makes a good, netural-toned default.
	m_current_palette(*Palettes::Friend.data)
{
	auto all_palettes = m_palette_repo.get_all_custom_palettes();
	if (!all_palettes.empty()) {
		m_current_palette = {
			.data = *all_palettes[0].data,
			.name = all_palettes[0].name
		};
	} else {
		m_current_palette = {};
	}

	refresh();
	refresh_palette_list();
}

BOOL PaletteCustomizeDialog::command(WPARAM wparam, LPARAM lparam) {
	auto control_id = LOWORD(wparam);
	switch (control_id) {
		case IDOK: {
			save_current_palette();
			EndDialog(m_dialog, true);
			return TRUE;
		}
		case IDCANCEL: {
			EndDialog(m_dialog, true);
			return TRUE;
		}
		case IDC_PALDLG_PREV_BITMAP:
		case IDC_PALDLG_NEXT_BITMAP: {
			auto current_bitmap_position = Bitmaps::All.find(m_current_preview_bitmap);

			auto bitmaps_start = Bitmaps::All.begin();
			auto bitmaps_last = --Bitmaps::All.end();

			if (control_id == IDC_PALDLG_PREV_BITMAP && current_bitmap_position == bitmaps_start) {
				m_current_preview_bitmap = *bitmaps_last;
			} else if (control_id == IDC_PALDLG_NEXT_BITMAP && current_bitmap_position == bitmaps_last) {
				m_current_preview_bitmap = *bitmaps_start;
			} else if (control_id == IDC_PALDLG_PREV_BITMAP) {
				m_current_preview_bitmap = *--current_bitmap_position;
			} else {
				m_current_preview_bitmap = *++current_bitmap_position;
			}

			m_preview_bitmap = Bitmaps::load_raw_resource(m_current_preview_bitmap.resource_id);
			refresh();

			break;
		}
		case IDC_PALDLG_SCALE_COLOR:
		case IDC_PALDLG_SCALE_HIGHLIGHT_COLOR:
		case IDC_PALDLG_SCALE_SHADOW_COLOR:
		case IDC_PALDLG_HORN_COLOR:
		case IDC_PALDLG_HORN_SHADOW_COLOR:
		case IDC_PALDLG_EYE_COLOR:
		case IDC_PALDLG_WHITES_COLOR: {
			int palette_index = palette_index_for_control(LOWORD(wparam));
			get_and_save_color(palette_index);
			break;
		}
		case IDC_PALDLG_PALETTE_LIST: {
			switch (HIWORD(wparam)) {
				case LBN_SELCHANGE: {
					update_current_palette();
					refresh();
					break;
				}
			}
			break;
		}
		case IDC_PALDLG_DUPE_PALETTE:
		case IDC_PALDLG_NEW_PALETTE: {
			std::wstring *name = (std::wstring *) DialogBox(
				NULL,
				MAKEINTRESOURCE(DLG_NEW_CUSTOM_PALETTE),
				m_dialog,
				(DLGPROC) ScreenSaverNewCustomPaletteDialog
			);

			if (name == nullptr) {
				break;
			}

			auto new_data = LOWORD(wparam) == IDC_PALDLG_DUPE_PALETTE
				? m_current_palette->data
				: *DefaultPalette.data;

			m_current_palette = {
				.data = new_data,
				.name = *name
			};

			save_current_palette_as_new();
			refresh_palette_list();

			delete name;

			break;
		}
		case IDC_PALDLG_INSERT_PREDEFINED: {
			std::wstring *predefined_palette_name = (std::wstring *) DialogBox(
				NULL,
				MAKEINTRESOURCE(DLG_INSERT_PREDEFINED),
				m_dialog,
				(DLGPROC) AddPredefinedPaletteDialog
			);

			if (predefined_palette_name == nullptr) {
				return FALSE;
			}

			auto palette = *std::find_if(Palettes::All.begin(), Palettes::All.end(), [&](const Palettes::Definition &palette) {
				return *predefined_palette_name == palette.name;
			});

			m_current_palette = {
				.data = *palette.data,
				.name = palette.name
			};

			save_current_palette_as_new();
			refresh_palette_list();

			break;
		}
		case IDC_PALDLG_DELETE_PALETTE: {
			delete_current_palette();
			refresh_palette_list();
			break;
		}
		case IDC_PALDLG_PNG_EXPORT: {
			if (m_current_palette) {
				IFileDialog *file_dialog;
				CoCreateInstance(
					CLSID_FileOpenDialog,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_PPV_ARGS(&file_dialog)
				);

				DWORD flags;
				file_dialog->GetOptions(&flags);
				file_dialog->SetOptions(flags | FOS_PICKFOLDERS);
				file_dialog->SetTitle(L"Choose where to save the ygklokin images");
				file_dialog->SetOkButtonLabel(L"Save Yonklins Here");

				auto result = file_dialog->Show(m_dialog);
				if (result == HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
					break;
				}

				IShellItem *selected_folder;
				file_dialog->GetResult(&selected_folder);

				wchar_t *folder_name = nullptr;
				selected_folder->GetDisplayName(SIGDN_FILESYSPATH, &folder_name);
				
				if (folder_name != nullptr) {
					auto export_path = get_png_export_path(folder_name, m_current_palette->name);
					do_png_export(export_path, *m_current_palette);

					ShellExecute(NULL, L"explore", export_path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
					CoTaskMemFree(folder_name);
				}

				selected_folder->Release();
			}
      break;
    }
		case IDC_PALDLG_IMPORT_EXPORT_PALETTES: {
			std::wstring *exported_palettes = new std::wstring(export_palettes());
			PalettesImport *imported_palettes = (PalettesImport *) DialogBoxParam(
				NULL,
				MAKEINTRESOURCE(DLG_IMPORT_EXPORT_PALETTES),
				m_dialog,
				(DLGPROC) ScreenSaverImportExportPalettesDialog,
				(LPARAM) exported_palettes
			);

			if (imported_palettes == nullptr) {
				break;
			}

			m_current_palette = imported_palettes->import_palettes();
			
			refresh_palette_list();

			delete imported_palettes;
			break;
		}
	}

	return FALSE;
}

HBRUSH PaletteCustomizeDialog::handle_color_button_message(WPARAM wparam, LPARAM lparam) {
	HWND button = (HWND) lparam;
	int button_id = GetDlgCtrlID(button);

	if ((button_id != IDC_PALDLG_SCALE_COLOR
		&& button_id != IDC_PALDLG_SCALE_HIGHLIGHT_COLOR
		&& button_id != IDC_PALDLG_SCALE_SHADOW_COLOR
		&& button_id != IDC_PALDLG_HORN_COLOR
		&& button_id != IDC_PALDLG_HORN_SHADOW_COLOR
		&& button_id != IDC_PALDLG_EYE_COLOR
		&& button_id != IDC_PALDLG_WHITES_COLOR)
	 || !m_current_palette
	) {
		return FALSE;
	}

	int palette_index = palette_index_for_control(button_id);
	Color color = m_current_palette->data[palette_index];

	HBRUSH brush = CreateSolidBrush(RGB(
		std::get<RED>(color),
		std::get<GREEN>(color),
		std::get<BLUE>(color)
	));

	// This may be a resource leak because brushes are supposed to be freed
	// after their use, but I have no idea how to get a reference to this brush
	// back after it's been used. So, just let it leak?
	return brush;
}

void PaletteCustomizeDialog::refresh() {
	HWND not_all_colors_warning = GetDlgItem(m_dialog, IDC_PALDLG_NOT_ALL_COLORS_WARNING);
	const static std::set<Bitmaps::Definition> bitmaps_with_all_colors = {
		Bitmaps::Lksix,
		Bitmaps::Lkconcern,
	};

	if (bitmaps_with_all_colors.find(m_current_preview_bitmap) == bitmaps_with_all_colors.end()) {
		ShowWindow(not_all_colors_warning, SW_SHOW);
	} else {
		ShowWindow(not_all_colors_warning, SW_HIDE);
	}

	std::vector<HWND> controls_to_disable = {
		GetDlgItem(m_dialog, IDC_PALDLG_DELETE_PALETTE),
		GetDlgItem(m_dialog, IDC_PALDLG_SCALE_COLOR),
		GetDlgItem(m_dialog, IDC_PALDLG_SCALE_HIGHLIGHT_COLOR),
		GetDlgItem(m_dialog, IDC_PALDLG_SCALE_SHADOW_COLOR),
		GetDlgItem(m_dialog, IDC_PALDLG_HORN_COLOR),
		GetDlgItem(m_dialog, IDC_PALDLG_HORN_SHADOW_COLOR),
		GetDlgItem(m_dialog, IDC_PALDLG_EYE_COLOR),
		GetDlgItem(m_dialog, IDC_PALDLG_WHITES_COLOR),
		GetDlgItem(m_dialog, IDC_PALDLG_DUPE_PALETTE),
		GetDlgItem(m_dialog, IDC_PALDLG_PNG_EXPORT),
		GetDlgItem(m_dialog, IDC_PALDLG_NEXT_BITMAP),
		GetDlgItem(m_dialog, IDC_PALDLG_PREV_BITMAP),
	};

	auto all_palettes = m_palette_repo.get_all_custom_palettes();
	if (all_palettes.empty()) {
		for (HWND control : controls_to_disable) {
			ShowWindow(not_all_colors_warning, SW_HIDE);
			EnableWindow(control, false);
		}
	} else {
		for (HWND control : controls_to_disable) {
			EnableWindow(control, true);
		}
	}

	if (m_current_palette) {
		apply_palette_to_preview(m_dialog, m_preview_bitmap, IDC_PALDLG_PREVIEW, m_current_palette->data);
	} else {
		apply_palette_to_preview(m_dialog, m_preview_bitmap, IDC_PALDLG_PREVIEW, *DisabledPalette.data);
	}

	// Force the color buttons to re-paint, they won't on their own
	RECT dialog_rect = { 0 };
	GetWindowRect(m_dialog, &dialog_rect);

	// In Windows Vista+, calling GetWindowRect on a dialog does not 
	// just return the bounds of the dialog;
	// it gives the bounds of the dialog _including the drop shadow!_
	// There is a function for getting the size without the drop shadow,
	// but it does NOT respect DPI settings, so it's effectively useless.
	// (and it requires linking against dwmapi... a whole dependency just for this!?)
	// Fuck it! I measured roughly how big the drop shadow is on my
	// machine and offset the rectangle by that.
	// Why do you hate me, billy?
	const static int drop_shadow_size = 12;
	dialog_rect.left += drop_shadow_size;
	dialog_rect.right -= drop_shadow_size;
	dialog_rect.top += drop_shadow_size;
	dialog_rect.bottom -= drop_shadow_size;

	HWND color_buttons_region = GetDlgItem(m_dialog, IDC_PALDLG_COLOR_BUTTONS_REGION);
	RECT colors_rect = { 0 };
	GetWindowRect(color_buttons_region, &colors_rect);

	RECT colors_client_rect = {
		.left = colors_rect.left - dialog_rect.left,
		.top = colors_rect.top - dialog_rect.top,
		.right = colors_rect.right - dialog_rect.left,
		.bottom = colors_rect.bottom - dialog_rect.top,
	};

	InvalidateRect(m_dialog, &colors_client_rect, TRUE);
}

void PaletteCustomizeDialog::refresh_palette_list() {
	HWND palette_list = GetDlgItem(m_dialog, IDC_PALDLG_PALETTE_LIST);
	ListBox_ResetContent(palette_list);

	auto all_palettes = m_palette_repo.get_all_custom_palettes();
	for (const auto &palette : all_palettes) {
		ListBox_AddString(palette_list, palette.name.c_str());
	}

	if (m_current_palette) {
		ListBox_SelectString(palette_list, -1, m_current_palette->name.c_str());
	}

	refresh();
}

void PaletteCustomizeDialog::update_current_palette() {
	save_current_palette();

	auto all_palettes = m_palette_repo.get_all_custom_palettes();
	size_t palette_index = SendDlgItemMessage(
		m_dialog,
		IDC_PALDLG_PALETTE_LIST,
		LB_GETCURSEL,
		0,
		0
	);

	std::wstring selected = string_from_buffer<wchar_t>([&](wchar_t *buffer, size_t size) {
		SendDlgItemMessage(
			m_dialog,
			IDC_PALDLG_PALETTE_LIST,
			LB_GETTEXT,
			palette_index,
			(LPARAM) buffer
		);
	});

	m_current_palette = {
		.data = *std::find_if(all_palettes.begin(), all_palettes.end(), [&](const Palettes::Definition &palette) {
			return palette.name == selected;
		})->data,
		.name = selected,
	};
}

void PaletteCustomizeDialog::save_current_palette() {
	if (m_current_palette) {
		m_palette_repo.set_palette(m_current_palette->name, m_current_palette->data);
	}
}

void PaletteCustomizeDialog::save_current_palette_as_new() {
	static auto get_copy_name = [](const std::wstring &name, int i) -> std::wstring {
		return std::format(L"{} {}", name, i);
	};

	if (m_current_palette) {
		// If we try to save a palette whose name is already taken, we want to
		// instead save it with the name "palette 2", then "palette 3", and so on.
		// (like Windows's own "Copy Of Copy Of New Folder (2) (3)", but fewer words)
		// This "is_nth_copy" variable gets set to 1 if it is the first copy and needs
		// no name change, and anything > 1 if it is a copy and does need a name change.
		int is_nth_copy = [&]() -> int {
			if (!m_palette_repo.get_palette(m_current_palette->name)) {
				return 1;
			}

			for (int i = 2; true; i++) {
				std::wstring copy_name = get_copy_name(m_current_palette->name, i);
				if (!m_palette_repo.get_palette(copy_name)) {
					return i;
				}
			}
		}();

		if (is_nth_copy == 1) {
			m_palette_repo.set_palette(m_current_palette->name, m_current_palette->data);
		} else {
			auto copy_name = get_copy_name(m_current_palette->name, is_nth_copy);
			m_palette_repo.set_palette(copy_name, m_current_palette->data);
			m_current_palette->name = copy_name;
		}
	}
}

void PaletteCustomizeDialog::delete_current_palette() {
	if (!m_current_palette) {
		return;
	}

	auto all_palettes = m_palette_repo.get_all_custom_palettes();
	size_t next_index_to_select = 0;
	if (all_palettes.size() > 1) {
		for (size_t i = 0; i < all_palettes.size(); i++) {
			if (all_palettes[i].name == m_current_palette->name) {
				next_index_to_select = (i > 0 ? i - 1 : i + 1);
				break;
			}
		}
	}

	m_palette_repo.remove_palette(m_current_palette->name);

	if (all_palettes.size() > 1) {
		m_current_palette = {
			.data = *all_palettes[next_index_to_select].data,
			.name = all_palettes[next_index_to_select].name,
		};
	} else {
		m_current_palette = {};
	}
}

std::wstring PaletteCustomizeDialog::get_png_export_path(const std::wstring &base_path, const std::wstring &palette_name) {
	return std::format(L"{}\\yokins-{}-{}", base_path, palette_name, cast<int>(Noise::random() * 10000.0));
}

void PaletteCustomizeDialog::do_png_export(const std::wstring &path, const CurrentPalette &palette) {
	Gdiplus::GdiplusStartupInput gdiStartup;
	ULONG_PTR gdiplus_token;
	Gdiplus::GdiplusStartup(&gdiplus_token, &gdiStartup, NULL);

	CreateDirectory(path.c_str(), NULL);

	for (const auto &bitmap_def : Bitmaps::All) {
		// The most reliable way to get the data for the bitmap to GDI+ is to
		// use the expanded data in its full, unindexed, 32 bits per pixel glory.
		// A lot like how it is for OpenGL!
		auto texture = Texture::of(&palette.data, bitmap_def);
		GLubyte *texture_data = texture->data();

		// We store texture data as RGBA, but GDI+ wants them as BGRA.
		// So, swap the texture's red and blue channels.
		for (int i = 0; i < BITMAP_WH * BITMAP_WH; i++) {
			std::swap(texture_data[i * 4 + BLUE], texture_data[i * 4 + RED]);
		}

		// Almost there, but GDI+'s coordinate system is also upside down 
		// relative to how we store the pixels.
		// So, flip the image across the y.
		for (int y = 0; y < BITMAP_WH / 2; y++) {
			int y_prime = BITMAP_WH - 1 - y;

			for (int x = 0; x < BITMAP_WH; x++) {
				int i = x + y * BITMAP_WH;
				int i_prime = x + y_prime * BITMAP_WH;

				std::swap(texture_data[i * 4 + RED], texture_data[i_prime * 4 + RED]);
				std::swap(texture_data[i * 4 + GREEN], texture_data[i_prime * 4 + GREEN]);
				std::swap(texture_data[i * 4 + BLUE], texture_data[i_prime * 4 + BLUE]);
				std::swap(texture_data[i * 4 + ALPHA], texture_data[i_prime * 4 + ALPHA]);
			}
		}

		Gdiplus::Bitmap bitmap(
			BITMAP_WH, 
			BITMAP_WH, 
			BITMAP_WH * 4, 
			PixelFormat32bppARGB,
			texture_data
		);

		// This is kind of gross, but the alternative is a 20-something line function
		// with calls to GetImageEncoders and other crap... just to get the ID of the
		// PNG encoder... seriously?
		// I have no good reason to believe this ID will be different between machines
		// or that any given machine will not have this encoder. So, blah.
		struct __declspec(uuid("{557cf406-1a04-11d3-9a73-0000f81ef32e}")) PngEncoder;
		auto png_encoder_sclid = __uuidof(PngEncoder);

		const auto save_path = std::format(L"{}\\{}.png", path, bitmap_def.name);
		bitmap.Save(save_path.c_str(), &png_encoder_sclid, NULL);

		delete[] texture_data;
	}

	Gdiplus::GdiplusShutdown(gdiplus_token);
}

void PaletteCustomizeDialog::apply_palette_to_preview(HWND dialog, HANDLE preview_bitmap, int preview_control_id, const PaletteData &palette) {
	HDC memory_context = CreateCompatibleDC(GetDC(dialog));

	SelectBitmap(memory_context, preview_bitmap);
	RGBQUAD colors[_PALETTE_SIZE];

	// I REALLY don't want to go down a rabbit hole of figuring out how to make this bitmap
	// actually transparent (it's an absolute miracle any of this works _at all_!), so we
	// just guess what the base color of the dialog is to fake transparency.
	// We could get the actual color from a system brush, but that sounds like effort!
	colors[PI_TRANSPARENT] = {
		.rgbBlue = 240,
		.rgbGreen = 240,
		.rgbRed = 240
	};

	for (int i = 1; i < _PALETTE_SIZE; i++) {
		colors[i] = {
			.rgbBlue = std::get<BLUE>(palette[i]),
			.rgbGreen = std::get<GREEN>(palette[i]),
			.rgbRed = std::get<RED>(palette[i]),
		};
	}

	SetDIBColorTable(
		memory_context,
		0,
		_PALETTE_SIZE,
		colors
	);

	DeleteDC(memory_context);

	SendDlgItemMessage(
		dialog,
		preview_control_id,
		STM_SETIMAGE,
		IMAGE_BITMAP,
		(LPARAM) preview_bitmap
	);
}

int PaletteCustomizeDialog::palette_index_for_control(int color_button_control_id) {
	switch (color_button_control_id) {
		default:
		case IDC_PALDLG_SCALE_COLOR: return PI_SCALES;
		case IDC_PALDLG_SCALE_HIGHLIGHT_COLOR: return PI_SCALES_HIGHLIGHT;
		case IDC_PALDLG_SCALE_SHADOW_COLOR: return PI_SCALES_SHADOW;
		case IDC_PALDLG_HORN_COLOR: return PI_HORNS;
		case IDC_PALDLG_HORN_SHADOW_COLOR: return PI_HORNS_SHADOW;
		case IDC_PALDLG_EYE_COLOR: return PI_EYE;
		case IDC_PALDLG_WHITES_COLOR: return PI_WHITES;
	}
}

void PaletteCustomizeDialog::get_and_save_color(int palette_index) {
	if (!m_current_palette) {
		return;
	}

	static COLORREF custom_colors[16];

	Color current_color = m_current_palette->data[palette_index];
	CHOOSECOLOR color_options = {
		.lStructSize = sizeof(CHOOSECOLOR),
		.hwndOwner = m_dialog,
		.hInstance = NULL,
		.rgbResult = RGB(std::get<RED>(current_color), std::get<GREEN>(current_color), std::get<BLUE>(current_color)),
		.lpCustColors = custom_colors,
		.Flags = CC_FULLOPEN | CC_ENABLETEMPLATE | CC_ENABLEHOOK | CC_RGBINIT,
		.lCustData = 0,
		.lpfnHook = (LPCCHOOKPROC) CustomColorDialog,
		.lpTemplateName = MAKEINTRESOURCE(DLG_CUSTOM_CHOOSECOLOR),
	};

	bool color_choice_successful = ChooseColor(&color_options);
	if (color_choice_successful) {
		m_current_palette->data[palette_index] = {
			GetRValue(color_options.rgbResult),
			GetGValue(color_options.rgbResult),
			GetBValue(color_options.rgbResult),
			255
		};

		// fuck it
		save_current_palette();
		refresh();
	}
}

std::wstring PaletteCustomizeDialog::export_palettes() {
	auto all_palettes = m_palette_repo.get_all_custom_palettes();
	std::wstring palettes_string = L"";

	for (const auto &palette : all_palettes) {
		auto name = palette.name;
		auto *data = palette.data;

		palettes_string.append(palette.name + L" " + PalettesImport::palette_name_end + L" " + m_palette_repo.serialize(*data) + PalettesImport::palette_color_end + L"\r\n");
	}
	palettes_string.append(PalettesImport::input_terminator);

	return palettes_string;
}

PalettesImport::PalettesImport(std::vector<std::wstring> palettes_name,
							   std::vector<std::vector<std::wstring>> palettes_raw_colors,
							   std::vector<unsigned int> palettes_has_errors,
							   bool import_string_had_terminator)
	: palettes_name(palettes_name), palettes_raw_colors(palettes_raw_colors), palettes_has_errors(palettes_has_errors), import_string_had_terminator(import_string_had_terminator) {
}

// Parses an import string and returns a PalettesImport object that can then be converted into palettes
PalettesImport *PalettesImport::parse_palettes_string(const std::wstring &import_string) {

	size_t import_string_current_index = 0;
	bool has_terminator_at_end = false;

	std::vector<std::wstring> parsed_palette_names;
	std::vector<std::vector<std::wstring>> parsed_palette_colors;
	std::vector<unsigned int> parsed_palette_has_errors;

	while (import_string.find_first_of(valid_chars, import_string_current_index) != std::wstring::npos
		   && import_string.find_first_of(palette_name_end, import_string_current_index) != std::wstring::npos) {
		has_terminator_at_end = (import_string.find_first_of(input_terminator, import_string_current_index) != std::wstring::npos);

		std::wstring current_name = L"";
		std::vector<std::wstring> current_colors;
		for (size_t i = 0; i < colors_amount; i++) {
			current_colors.push_back(palette_color_end);		// Used to differentiate between an empty color and a missing color
		}
		unsigned int current_has_errors = 0;

		// Read the palette name (From first valid character (except space) to ':')
		import_string_current_index = import_string.find_first_of(valid_chars, import_string_current_index);
		current_name = import_string.substr(import_string_current_index, import_string.find_first_of(palette_name_end, import_string_current_index) - import_string_current_index);
		current_name = current_name.substr(0, current_name.find_last_not_of(L" ") + 1);
		if (current_name.length() < PaletteCustomizeDialog::MinPaletteNameSize) {
			current_has_errors |= error_name_too_short;
		}
		if (current_name.length() >= PaletteCustomizeDialog::MaxPaletteNameSize) {
			current_has_errors |= error_name_too_long;
		}
		if (current_name.find_first_not_of(valid_chars + L" \r\n") != std::wstring::npos) {
			current_has_errors |= error_name_invalid_characters;
		}
		if (current_name.find_first_of(L"\r\n") != std::wstring::npos) {
			current_has_errors |= error_palette_contains_newlines;
		}

		import_string_current_index = import_string.find_first_of(palette_name_end, import_string_current_index) + 1;

		// Looking for exactly 7 colors, each color starts with # and ends with ; and must contain exactly 6 base digits in base 16
		for (int i = 0; i < colors_amount; i++) {
			if (import_string.find_first_of(palette_name_end, import_string_current_index) < import_string.find_first_of(palette_color_start, import_string_current_index)) {
				current_has_errors |= error_colors_too_few;
				break;
			}

			if (import_string.find_first_of(palette_color_start, import_string_current_index) == std::wstring::npos) {
				current_has_errors |= error_colors_too_few;
				break;
			}
			import_string_current_index = import_string.find_first_of(palette_color_start, import_string_current_index) + 1;
			current_colors[i] = import_string.substr(import_string_current_index, import_string.find_first_of(palette_color_end, import_string_current_index) - import_string_current_index);
			if (import_string.find_first_of(palette_color_end, import_string_current_index) == std::wstring::npos) {
				current_has_errors |= error_colors_too_few;
				break;
			}
			import_string_current_index = import_string.find_first_of(palette_color_end, import_string_current_index) + 1;

			if (current_colors[i].length() != 6) {
				current_has_errors |= error_colors_incorrect_length;
			}
			if (current_colors[i].find_first_not_of(valid_hex_chars + L"\r\n") != std::wstring::npos) {
				current_has_errors |= error_colors_invalid_characters;
			}
			if (current_colors[i].find_first_of(L"\r\n") != std::wstring::npos) {
				current_has_errors |= error_palette_contains_newlines;
			}
		}

		parsed_palette_names.push_back(current_name);
		parsed_palette_colors.push_back(current_colors);
		parsed_palette_has_errors.push_back(current_has_errors);

		// It's likely not reasonable for me to individually find and catch every single possible npos error,
		// and since there is no other reason the index should be 0 here, this should hopefully catch any that I've missed.
		if (import_string_current_index == 0) {
			break;
		}
	}

	return new PalettesImport(parsed_palette_names, parsed_palette_colors, parsed_palette_has_errors, has_terminator_at_end);
}

std::optional<std::wstring> PalettesImport::get_error_string(size_t max_errors) {
	if (palettes_name.size() == 0) {
		return std::nullopt;
	}

	size_t palette_count = palettes_name.size();
	size_t invalid_palette_count = 0;
	for (unsigned int has_errors : palettes_has_errors) {
		if (has_errors != error_none) {
			invalid_palette_count++;
		}
	}

	if (!import_string_had_terminator && invalid_palette_count <= 1 && (invalid_palette_count == 0 || *palettes_has_errors.rbegin() != error_none)) {
		std::wstring error_string = L"No terminator (" + input_terminator + L") found at the end of the input. This may be caused by the inputted text being too long.";
		if (palettes_name.size() > 1 || invalid_palette_count == 0) {
			auto palette_name = palettes_name.rbegin();
			if (*palettes_has_errors.rbegin() != error_none) {
				palette_name++;
			}
			error_string.append(L"\nLast valid palette found: " + *palette_name);
		}
		return error_string;
	} else if (invalid_palette_count == 0) {
		return std::nullopt;
	}

	std::wstring error_string = L"";
	for (int i = 0, invalid_count = 0; i < palette_count; i++) {
		std::wstring palette_name = palettes_name[i];
		std::vector<std::wstring> palette_raw_colors = palettes_raw_colors[i];
		unsigned int palette_has_errors = palettes_has_errors[i];

		if (palette_has_errors == error_none) {
			continue;
		}

		invalid_count++;
		if (palette_name.find_first_of(L"\r\n") != std::wstring::npos) {
			std::wstring formatted_name = palette_name;
			while (formatted_name.find_first_of(L'\r') != std::wstring::npos) {
				formatted_name.replace(formatted_name.find_first_of(L'\r'), 1, L"\\r");
			}
			while (formatted_name.find_first_of(L'\n') != std::wstring::npos) {
				formatted_name.replace(formatted_name.find_first_of(L'\n'), 1, L"\\n");
			}
			error_string.append(formatted_name + L":");
		} else {
			error_string.append(palette_name + L":");
		}

		if ((palette_has_errors & error_name_too_short) != error_none) {
			error_string.append(L" Name is too short,");
		}
		if ((palette_has_errors & error_name_too_long) != error_none) {
			error_string.append(L" Name is too long,");
		}
		if ((palette_has_errors & error_name_invalid_characters) != error_none) {
			std::wstring invalid_chars = L"";
			while (palette_name.find_first_not_of(valid_chars + L" \r\n" + invalid_chars) != std::wstring::npos) {
				invalid_chars += palette_name[palette_name.find_first_not_of(valid_chars + L" \r\n" + invalid_chars)];
			}
			error_string.append(L" Name has invalid characters (" + invalid_chars + L"),");
		}
		if ((palette_has_errors & error_colors_too_few) != error_none) {
			int count = 0;
			for (std::wstring color : palette_raw_colors) {
				if (color.find_first_of(palette_color_end) == std::wstring::npos) {
					count++;
				}
			}
			error_string.append(L" Too few colors defined (have " + std::to_wstring(count) + L", expected " + std::to_wstring(colors_amount) + L"),");
		}
		if ((palette_has_errors & error_colors_incorrect_length) != error_none) {
			error_string.append(L" One or more colors were of incorrect length (found:");
			for (std::wstring color : palette_raw_colors) {
				if (color.find_first_of(palette_color_end) == std::wstring::npos) {
					error_string += L" " + std::to_wstring(color.length()) + L",";
				}
			}
			*error_string.rbegin() = L';';
			error_string.append(L" expected: 6),");
		}
		if ((palette_has_errors & error_colors_invalid_characters) != error_none) {
			std::wstring invalid_chars = L"";
			for (std::wstring color : palette_raw_colors) {
				while (color.find_first_not_of(valid_hex_chars + L";\r\n" + invalid_chars) != std::wstring::npos) {
					invalid_chars += color[color.find_first_not_of(valid_hex_chars + L";\r\n" + invalid_chars)];
				}
			}
			error_string.append(L" Colors have invalid characters (" + invalid_chars + L"),");
		}
		if ((palette_has_errors & error_palette_contains_newlines) != error_none) {
			error_string.append(L" Palette name or colors contain a newline,");
		}

		if (invalid_count < invalid_palette_count) {
			*error_string.rbegin() = L';';
			error_string.append(L"\n");
			if (invalid_count >= max_errors) {
				error_string.append(L"( + " + std::to_wstring(invalid_palette_count - invalid_count) + L" more)");
				break;
			}
		} else {
			error_string.pop_back();
		}
	}

	return error_string;
}

size_t PalettesImport::get_valid_palettes_amount() {
	size_t count = 0;
	for (unsigned int has_errors : palettes_has_errors) {
		if (has_errors == error_none) {
			count++;
		}
	}

	return count;
}

std::wstring PalettesImport::get_valid_palette_names(size_t max_length) {
	std::wstring palette_names = L"";
	size_t count = 0;

	for (size_t i = 0; i < palettes_name.size(); i++) {
		if (palettes_has_errors[i] != error_none) {
			continue;
		}
		if (palette_names.length() + palettes_name[i].length() > max_length) {
			palette_names.append(L"\n( + " + std::to_wstring(get_valid_palettes_amount() - count) + L" more)  ");
			return palette_names;
		}

		palette_names.append(palettes_name[i] + L", ");
		count++;
	}

	palette_names.pop_back();
	palette_names.pop_back();

	return palette_names;
}

PaletteCustomizeDialog::CurrentPalette PalettesImport::import_palettes() {
	size_t palette_count = palettes_name.size();
	PaletteRepository palette_repo = PaletteRepository();
	PaletteCustomizeDialog::CurrentPalette last_palette = {};

	for (int i = 0; i < palette_count; i++) {
		if (palettes_has_errors[i] != error_none) {
			continue;
		}

		std::wstring name = palettes_name[i];
		std::vector<Palettes::Definition> existing_palettes = palette_repo.get_all_custom_palettes();
		bool is_name_taken = false;
		for (Palettes::Definition palette : existing_palettes) {
			if (_wcsicmp(palette.name.c_str(), name.c_str()) == 0) {
				is_name_taken = true;
				break;
			}
		}
		if (is_name_taken) {
			for (size_t num_add = 2; true; num_add++) {
				is_name_taken = false;
				std::wstring name_append = L" (" + std::to_wstring(num_add) + L")";

				while (name.length() + name_append.length() >= PaletteCustomizeDialog::MaxPaletteNameSize) {
					name.pop_back();
				}

				for (Palettes::Definition palette : existing_palettes) {
					if (_wcsicmp(palette.name.c_str(), (name + name_append).c_str()) == 0) {
						is_name_taken = true;
						break;
					}
				}

				if (!is_name_taken) {
					name.append(name_append);
					break;
				}
			}
		}

		std::wstring colors = L"";
		for (std::wstring color : palettes_raw_colors[i]) {
			colors.append(palette_color_start + color + palette_color_end);
		}
		colors.pop_back();

		PaletteData data = palette_repo.deserialize(colors);

		last_palette = {
			.data = data,
			.name = name
		};

		palette_repo.set_palette(name, data);
	}

	return last_palette;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
	static ConfigDialog *cfg_dialog;

	switch (message) {
		case WM_INITDIALOG: {
			cfg_dialog = new ConfigDialog(dialog);
			return TRUE;
		}
		case WM_COMMAND: {
			return cfg_dialog->command(wparam, lparam);
		}
		case WM_HSCROLL: {
			HWND handle = (HWND) lparam;

			if (handle != NULL) {
				return cfg_dialog->slider_changed(wparam, handle);
			}
		}
	}

	return FALSE;
}

LRESULT CALLBACK ScreenSaverPaletteCustomizeDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
	static PaletteCustomizeDialog *palette_dialog;

	switch (message) {
		case WM_INITDIALOG: {
			palette_dialog = new PaletteCustomizeDialog(dialog);
			return TRUE;
		} case WM_COMMAND: {
			return palette_dialog->command(wparam, lparam);
		} case WM_CTLCOLORBTN: {
			return (LRESULT) palette_dialog->handle_color_button_message(wparam, lparam);
		}
	}

	return FALSE;
}

LRESULT CALLBACK ScreenSaverNewCustomPaletteDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
		case WM_INITDIALOG: {
			return TRUE;
		} case WM_COMMAND: {
			HWND name_input = GetDlgItem(dialog, IDC_PALDLG_NEW_PALETTE_NAME);

			const static size_t max_size = PaletteCustomizeDialog::MaxPaletteNameSize;
			std::wstring name = string_from_buffer<wchar_t, max_size>([&](wchar_t *buffer, size_t size) {
				Edit_GetText(
					name_input,
					buffer,
					cast<int>(size)
				);
			});

			// Trim any leading or trailing whitespace
			if (name.find_first_not_of(L" ") != std::wstring::npos) {
				name = name.substr(name.find_first_not_of(L" "));
				name = name.substr(0, name.find_last_not_of(L" ") + 1);
			}
			else {
				name = L"";
			}

			switch (LOWORD(wparam)) {
				case IDOK: {
					auto *returned_name = new std::wstring(name);
					EndDialog(dialog, (INT_PTR) returned_name);
					return TRUE;
				} case IDCANCEL: {
					EndDialog(dialog, 0);
					return TRUE;
				} case IDC_PALDLG_NEW_PALETTE_NAME: {
					switch (HIWORD(wparam)) {
						case EN_CHANGE: {
							// Various special characters might break the registry parsing if they are saved.
							// The only catastrophe I can directly forsee is commas, but may as well just only allow
							// alphanumerics and a few special characters to be extra safe.
							const static std::wstring valid_chars = L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789 -_!?:()";

							bool name_too_short = name.size() < PaletteCustomizeDialog::MinPaletteNameSize;
							bool name_has_invalid_chars = name.find_first_not_of(valid_chars) != std::wstring::npos;
							// There is no "name_too_long" check because names are
							// automatically truncated due to the buffer size.

							HWND ok_button = GetDlgItem(dialog, IDOK);
							EnableWindow(ok_button, !name_too_short && !name_has_invalid_chars);
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	return FALSE;
}

LRESULT CALLBACK CustomColorDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
	const static size_t max_hex_code_length = 7; // 6 digits + the pound sign (#)

	static auto color_from_hex_string = [&](const std::wstring &hex_code) -> std::optional<Color> {
		static const std::wstring valid_hex_digits = L"0123456789abcdefABCDEF";

		bool starts_with_pound = hex_code[0] == L'#';
		bool has_invalid_hex_digit = hex_code.find_first_not_of(valid_hex_digits) != std::wstring::npos;
		bool has_invalid_hex_digit_after_first = hex_code.find_first_not_of(valid_hex_digits, 1) != std::wstring::npos;

		bool is_valid_pound_code = (hex_code.size() == max_hex_code_length)
			&& (starts_with_pound && !has_invalid_hex_digit_after_first);
		bool is_valid_digits_code = (hex_code.size() == max_hex_code_length - 1)
			&& (!starts_with_pound && !has_invalid_hex_digit);

		if (!is_valid_pound_code && !is_valid_digits_code) {
			return {};
		}

		if (is_valid_pound_code) {
			auto red = std::stoi(hex_code.substr(1, 2), nullptr, 16);
			auto green = std::stoi(hex_code.substr(3, 2), nullptr, 16);
			auto blue = std::stoi(hex_code.substr(5, 2), nullptr, 16);

			return { { red, green, blue, 255 } };
		}

		if (is_valid_digits_code) {
			auto red = std::stoi(hex_code.substr(0, 2), nullptr, 16);
			auto green = std::stoi(hex_code.substr(2, 2), nullptr, 16);
			auto blue = std::stoi(hex_code.substr(4, 2), nullptr, 16);

			return { { red, green, blue, 255 } };
		}

		return {};
	};

	static UINT set_rgb_message_code = RegisterWindowMessage(SETRGBSTRING);

	// As it turns out, changing the text of a text box causes an immediate(?)
	// and unavoidable EN_CHANGE notification for the text box being edited.
	// Since our handling of EN_CHANGE for both the RGB and hex code text boxes
	// changes the other's text, these are needed to prevent a stack overflow.
	// :fnyoy:
	static int rgb_changes_to_ignore = 0;
	static int hex_changes_to_ignore = 0;

	switch (message) {
		case WM_INITDIALOG: {
			SendDlgItemMessage(
				dialog,
				IDC_COLORDLG_HEX_CODE,
				EM_SETLIMITTEXT,
				max_hex_code_length,
				0
			);
			break;
		}
		case WM_NOTIFY: {
			if (lparam != NULL) {
				NMHDR notification = *((NMHDR *) lparam);
				auto a = 0;
			}
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wparam)) {
				case COLOR_RED:
				case COLOR_GREEN:
				case COLOR_BLUE: {
					switch (HIWORD(wparam)) {
						case EN_CHANGE: {
							if (hex_changes_to_ignore > 0) {
								hex_changes_to_ignore--;
								return FALSE;
							}
							rgb_changes_to_ignore = 1;

							HWND red_input = GetDlgItem(dialog, COLOR_RED);
							HWND green_input = GetDlgItem(dialog, COLOR_GREEN);
							HWND blue_input = GetDlgItem(dialog, COLOR_BLUE);

							std::wstring red = string_from_buffer<wchar_t, 4>([&](wchar_t *buffer, size_t size) {
								Edit_GetText(
									red_input,
									buffer,
									cast<int>(size)
								);
							});
							std::wstring green = string_from_buffer<wchar_t, 4>([&](wchar_t *buffer, size_t size) {
								Edit_GetText(
									green_input,
									buffer,
									cast<int>(size)
								);
							});
							std::wstring blue = string_from_buffer<wchar_t, 4>([&](wchar_t *buffer, size_t size) {
								Edit_GetText(
									blue_input,
									buffer,
									cast<int>(size)
								);
							});

							if (red.empty() || green.empty() || blue.empty()) {
								return FALSE;
							}

							std::wstring hex_code = std::format(L"#{:02x}{:02x}{:02x}", std::stoi(red), std::stoi(green), std::stoi(blue));
							HWND hex_input = GetDlgItem(dialog, IDC_COLORDLG_HEX_CODE);
							Edit_SetText(
								hex_input,
								hex_code.c_str()
							);
						}
					}
					break;
				}
				case IDC_COLORDLG_HEX_CODE: {
					switch (HIWORD(wparam)) {
						case EN_CHANGE: {
							if (rgb_changes_to_ignore > 0) {
								rgb_changes_to_ignore--;
								return FALSE;
							}
							hex_changes_to_ignore = 3;

							HWND hex_input = GetDlgItem(dialog, IDC_COLORDLG_HEX_CODE);
							std::wstring hex_code = string_from_buffer<wchar_t, max_hex_code_length + 1>([&](wchar_t *buffer, size_t size) {
								Edit_GetText(
									hex_input,
									buffer,
									cast<int>(size)
								);
							});

							auto color = color_from_hex_string(hex_code);
							if (!color) {
								return FALSE;
							}

							SendMessage(
								dialog,
								set_rgb_message_code,
								0,
								(LPARAM) RGB(std::get<RED>(*color), std::get<GREEN>(*color), std::get<BLUE>(*color))
							);
						}
					}
				}
			}
		}
	}

	return FALSE;
}

LRESULT CALLBACK ScreenSaverImportExportPalettesDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
	HWND palettes_input = GetDlgItem(dialog, IDC_PALDLG_IMPORT_EXPORT_PALETTES);
	switch (message) {
		case WM_INITDIALOG: {
			std::wstring *exported_palettes = (std::wstring *) lparam;
			Edit_SetText(
				palettes_input,
				exported_palettes->c_str()
			);
			delete exported_palettes;
			return TRUE;
		} case WM_COMMAND: {
			wchar_t *palettes_buffer = new wchar_t[1 << 16](L'\0');
			Edit_GetText(
				palettes_input,
				palettes_buffer,
				1 << 16
			);
			palettes_buffer[(1 << 16) - 1] = L'\0';

			switch (LOWORD(wparam)) {
				case IDOK: {
					std::wstring palettes = palettes_buffer;
					PalettesImport *imported_palettes = PalettesImport::parse_palettes_string(palettes);
					delete[] palettes_buffer;

					if (imported_palettes->get_error_string(1).has_value()) {
						int import_anyway = MessageBox(
							NULL,
							(L"The inputted text contains one or more problems. Are you sure you want to import anyway?\n\nThe following "
								+ std::to_wstring(imported_palettes->get_valid_palettes_amount())
								+ L" palettes will be imported: "
								+ imported_palettes->get_valid_palette_names(1000)).c_str(),
							L"Palette Customizer",
							MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2
						);
						if (import_anyway == IDNO) {
							delete imported_palettes;
							return TRUE;
						}
					}
					EndDialog(dialog, (INT_PTR) imported_palettes);
					return TRUE;
				} case IDCANCEL: {
					EndDialog(dialog, 0);
					delete[] palettes_buffer;
					return TRUE;
				} case IDC_PALDLG_IMPORT_EXPORT_PALETTES: {
					switch (HIWORD(wparam)) {
						case EN_CHANGE: {
							std::wstring palettes = palettes_buffer;
							PalettesImport *imported_palettes = PalettesImport::parse_palettes_string(palettes);

							HWND ok_button = GetDlgItem(dialog, IDOK);
							EnableWindow(ok_button, (imported_palettes->get_valid_palettes_amount() > 0));

							HWND dialog_error_text = GetDlgItem(dialog, IDC_IMPORT_ERROR_TEXT);
							EnableWindow(dialog_error_text, false);
							std::optional<std::wstring> imported_palettes_error_text = imported_palettes->get_error_string(1);
							if (imported_palettes_error_text.has_value()) {
								Edit_SetText(
									dialog_error_text,
									(L"One or more problems were detected:\n" + imported_palettes_error_text.value()).c_str()
								);
								EnableWindow(dialog_error_text, true);
							}
							else if (imported_palettes->get_valid_palettes_amount() > 0) {
								Edit_SetText(
									dialog_error_text,
									(std::to_wstring(imported_palettes->get_valid_palettes_amount()) + L" palettes will be imported:\n" + imported_palettes->get_valid_palette_names(250)).c_str()
								);
							}
							else {
								Edit_SetText(
									dialog_error_text,
									L"Paste text to import palettes."
								);
							}
							delete imported_palettes;
							break;
						}
					}
					break;
				}
			}
			delete[] palettes_buffer; 
			break;
		}
	}

	return FALSE;
}

LRESULT CALLBACK AddPredefinedPaletteDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
	const static HANDLE preview_bitmap = Bitmaps::load_raw_resource(Bitmaps::Lk.resource_id);

	static auto current_palette = Palettes::Aemil;

	auto get_palette_control = [&]() -> HWND {
		return GetDlgItem(dialog, IDC_PALDLG_CHOOSE_PREDEFINED);
	};

	auto update_preview = [&]() {
		PaletteCustomizeDialog::apply_palette_to_preview(
			dialog,
			preview_bitmap,
			IDC_PALDLG_PREDEFINED_PREVIEW,
			*current_palette.data
		);
	};

	switch (message) {
		case WM_INITDIALOG: {
			auto palette_combobox = get_palette_control();

			for (const Palettes::Definition &palette : Palettes::All) {
				ComboBox_AddString(palette_combobox, palette.name.c_str());
			}

			ComboBox_SelectString(palette_combobox, -1, current_palette.name.c_str());
			update_preview();

			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wparam)) {
				case IDOK: {
					auto palette_combobox = get_palette_control();

					std::wstring palette_name = string_from_buffer<wchar_t>([&](wchar_t *buffer, size_t size) {
						ComboBox_GetText(
							palette_combobox,
							buffer,
							cast<int>(size)
						);
					});

					auto *returned_name = new std::wstring(palette_name);
					EndDialog(dialog, (INT_PTR) returned_name);
					return TRUE;
				}
				case IDCANCEL: {
					EndDialog(dialog, (INT_PTR) nullptr);
					return TRUE;
				}
				case IDC_PALDLG_CHOOSE_PREDEFINED: {
					switch (HIWORD(wparam)) {
						case CBN_SELCHANGE: {
							auto palette_combobox = get_palette_control();

							std::wstring palette_name = string_from_buffer<wchar_t>([&](wchar_t *buffer, size_t size) {
								ComboBox_GetText(
									palette_combobox,
									buffer,
									cast<int>(size)
								);
							});

							current_palette = *std::find_if(Palettes::All.begin(), Palettes::All.end(), [&](const Palettes::Definition &palette) {
								return palette.name == palette_name;
							});

							update_preview();
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	return FALSE;
}
