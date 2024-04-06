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
	: m_dialog(dialog),
	// We use lksix because it shows off every color in the palette.
	m_preview_bitmap(Bitmaps::load_raw_resource(Bitmaps::Lksix.resource_id)),
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
	switch (LOWORD(wparam)) {
		case IDOK: {
			save_current_palette();
			EndDialog(m_dialog, true);
			return TRUE;
		}
		case IDCANCEL: {
			EndDialog(m_dialog, true);
			return TRUE;
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
	};

	auto all_palettes = m_palette_repo.get_all_custom_palettes();
	for (HWND control : controls_to_disable) {
		EnableWindow(control, !all_palettes.empty());
	}

	if (m_current_palette) {
		apply_palette_to_preview(m_dialog, m_preview_bitmap, IDC_PALDLG_PREVIEW, m_current_palette->data);
	} else {
		apply_palette_to_preview(m_dialog, m_preview_bitmap, IDC_PALDLG_PREVIEW, *DisabledPalette.data);
	}

	// Force the color buttons to re-paint, they won't on their own
	InvalidateRect(m_dialog, NULL, TRUE);
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

	m_palette_repo.remove_palette(m_current_palette->name);

	auto all_palettes = m_palette_repo.get_all_custom_palettes();
	if (!all_palettes.empty()) {
		m_current_palette = {
			.data = *all_palettes[0].data,
			.name = all_palettes[0].name,
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
		.Flags = CC_FULLOPEN | CC_ENABLETEMPLATE | CC_ENABLEHOOK,
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
							const static std::wstring valid_chars = L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789 -_!?:";

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
				case IDC_COLORDLG_HEX_CODE: {
					switch (HIWORD(wparam)) {
						case EN_CHANGE: {
							HWND hex_input = GetDlgItem(dialog, IDC_COLORDLG_HEX_CODE);
							std::wstring hex_code = string_from_buffer<wchar_t, max_hex_code_length>([&](wchar_t *buffer, size_t size) {
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
