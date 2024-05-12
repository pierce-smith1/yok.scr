#pragma once

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

const static int ANIM_TIMER_ID = 1;

// 17 is close enough to 16.66666...
const static int SIXTY_FPS = 33;

const static int ANIMATION_WIDTH = 800;
const static int ANIMATION_HEIGHT = 600;

class Context {
public:
	Context(HWND window);
	~Context();

	HDC device();
	HGLRC gl();
	RECT rect();
	unsigned int &frame_count();

	float t();

private:
	HWND m_window;
	HDC m_device;
	HGLRC m_gl;
	RECT m_rect;
	DEVMODE m_previous_display_mode;
	unsigned int m_frame_count;
};