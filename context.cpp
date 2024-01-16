#include "context.h"
#include "config.h"

Context::Context(HWND window) : m_window(window), m_frame_count(0) {
	PIXELFORMATDESCRIPTOR pfd{};
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
	return m_frame_count / cfg.at(Opts::TimeDivisor);
}