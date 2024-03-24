#pragma once

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

			PaletteRepository palette_repo;
			palette_repo.set_palette(L"alpha", *Palettes::Aemil.data);
			palette_repo.set_palette(L"zeta", *Palettes::Zoog.data);
			palette_repo.set_palette(L"lambda", *Palettes::Loxxe.data);
			palette_repo.set_palette(L"luna", *Palettes::Ascent.data);
			palette_repo.set_palette(L"fall", *Palettes::Autumn.data);

			PaletteGroupRepository group_repo;
			auto greek = group_repo.get_group(L"greek");
			greek.add_palette(L"alpha");
			greek.add_palette(L"zeta");
			greek.add_palette(L"lambda");

			auto wp = group_repo.get_group(L"wallpapers");
			wp.add_palette(L"luna");
			wp.add_palette(L"fall");

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

BOOL WINAPI RegisterDialogClasses(HANDLE instance) {
	return TRUE;
}
