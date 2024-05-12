#include "context.h"
#include "config.h"

Context::Context(HWND window) : m_window(window), m_frame_count(0) {
	DEVMODE dummy_devmode = {0};
	EnumDisplaySettings(NULL, 0, &dummy_devmode);

	DEVMODE previous_display_mode = {0};
	previous_display_mode.dmSize = sizeof(DEVMODE);
	bool success = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &previous_display_mode);

	previous_display_mode.dmBitsPerPel = 16;
	previous_display_mode.dmPelsWidth = 1024;
	previous_display_mode.dmPelsHeight = 768;
	m_previous_display_mode = previous_display_mode;

	DEVMODE new_display_mode = {0};
	new_display_mode.dmSize = sizeof(DEVMODE);
	new_display_mode.dmPelsWidth = ANIMATION_WIDTH;
	new_display_mode.dmPelsHeight = ANIMATION_HEIGHT;
	new_display_mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	ChangeDisplaySettings(&new_display_mode, CDS_FULLSCREEN);
	
	SetWindowPos(window, HWND_TOP, 0, 0, ANIMATION_WIDTH, ANIMATION_HEIGHT, SWP_NOSENDCHANGING);

	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize = sizeof pfd;
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;

	m_device = GetDC(window);
	int i = ChoosePixelFormat(m_device, & pfd);
	SetPixelFormat(m_device, i, &pfd);

	m_gl = wglCreateContext(m_device);
	wglMakeCurrent(m_device, m_gl);
	glEnable(GL_TEXTURE_2D);

	// If you don't specify these two things,
	// you won't get transparency on _anything_, no matter how hard you
	// scream the word "RGBA".
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GetClientRect(window, &m_rect);

	SetTimer(window, ANIM_TIMER_ID, SIXTY_FPS, NULL);
}

Context::~Context() {
	bool success = ChangeDisplaySettings(&m_previous_display_mode, 0);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_gl);
	ReleaseDC(m_window, m_device);

	KillTimer(m_window, ANIM_TIMER_ID);
}

HDC Context::device() {
	return m_device;
}

HGLRC Context::gl() {
	return m_gl;
}

RECT Context::rect() {
	return m_rect;
}

unsigned int &Context::frame_count() {
	return m_frame_count;
}

float Context::t() {
	return m_frame_count / cfg[TimeDivisor];
}