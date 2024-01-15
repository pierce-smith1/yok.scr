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

BOOL WINAPI RegisterDialogClasses(HANDLE instance) {
	return TRUE;
}
