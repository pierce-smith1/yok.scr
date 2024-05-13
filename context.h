#pragma once

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

const static int ANIM_TIMER_ID = 1;

// 17 is close enough to 16.66666...
const static int SIXTY_FPS = 33;

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
	unsigned int m_frame_count;
};