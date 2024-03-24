#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN     

#include <windows.h>
#include <ScrnSave.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>

const static std::wstring YOKSCR_VERSION = L"1.1.1";

LRESULT CALLBACK ScreenSaverPaletteCustomizeDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK ScreenSaverNewCustomPaletteDialog(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam);
