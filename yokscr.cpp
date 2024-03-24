#pragma once

#include <windowsx.h>

#include "yokscr.h"
#include "scene.h"
#include "noise.h"
#include "configdialog.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT WINAPI ScreenSaverProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	static Scene *scene;
	
	// Windows demands the most awkward of prose...
	// Endless indenting and ceaseless macros.
	// Certainly, time is deaf to these woes;
	// So I spray down the boiler with a high-powered hose
	// And the _minimum_ code by these walls will be closed.
	switch (message) {
		case WM_CREATE: {
			scene = new Scene(window);
			scene->draw_background();
			return 0;
		}
		case WM_DESTROY: {
			delete scene;
			return 0;
		}
		case WM_TIMER: {
			scene->draw();
			return 0;
		}
	}

	return DefScreenSaverProc(window, message, wparam, lparam);
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

			wchar_t name_buffer[PaletteCustomizeDialog::MaxPaletteNameSize] { L'\0' };
			Edit_GetText(
				name_input,
				name_buffer,
				PaletteCustomizeDialog::MaxPaletteNameSize
			);
			name_buffer[PaletteCustomizeDialog::MaxPaletteNameSize - 1] = L'\0';

			switch (LOWORD(wparam)) {
				case IDOK: {
					auto *name = new std::wstring(name_buffer);
					EndDialog(dialog, (INT_PTR) name);
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
							static std::wstring valid_chars = L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789 -_!?:";
							std::wstring name = name_buffer;

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

BOOL WINAPI RegisterDialogClasses(HANDLE instance) {
	return TRUE;
}
