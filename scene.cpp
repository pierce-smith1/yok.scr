#include "scene.h"
#include "bitmaps.h"
#include "config.h"
#include "spritecontrol.h"

Scene::Scene(HWND window)
// It's of utmost importance the context comes first!
// Else reality cursed, at the seams it will burst!!!
	: m_ctx(window),
	  m_sprites(SpriteGenerator().make(cast<unsigned int>(cfg[Cfg::SpriteCount]))),
	  m_choreographer((PatternName) cfg[Cfg::Pattern], &m_sprites, &m_ctx) { }

void Scene::draw() {
	glViewport(0, 0, m_ctx.rect().right, m_ctx.rect().bottom);
	gluPerspective(45, 1.0 * m_ctx.rect().right / m_ctx.rect().bottom, 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (cfg[Cfg::PlayOverDesktop]) {
		draw_background();
	}

	m_choreographer.update();

	for (Sprite *sprite : m_sprites) {
		sprite->draw(m_ctx);
	}

	glFlush();
	SwapBuffers(m_ctx.device());

	m_ctx.frame_count()++;
}

GLuint Scene::background_tex_id = 0;

void Scene::draw_background() {
	static BYTE *background_rgba = get_background_rgba();

	// The MSDN OpenGL docs don't really say if 0 is a valid texture id,
	// so... I'm just gonna hope it isn't.
	if (background_tex_id == 0) {
		glGenTextures(1, &background_tex_id);
	}

	glBindTexture(GL_TEXTURE_2D, background_tex_id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ctx.rect().right, m_ctx.rect().bottom, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, background_rgba);

	glBegin(GL_QUADS);

	glTexCoord2f(1.0, 0.0); glVertex2f(1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex2f(1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, -1.0);

	glEnd();
}

BYTE *Scene::get_background_rgba() {
	LONG screen_width = m_ctx.rect().right;
	LONG screen_height = m_ctx.rect().bottom;

	HDC screen_dc = GetDC(NULL);
	HDC target_dc = CreateCompatibleDC(screen_dc);
	HBITMAP target_bmp = CreateCompatibleBitmap(screen_dc, screen_width, screen_height);

	SelectObject(target_dc, target_bmp);

	int screen_x = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int screen_y = GetSystemMetrics(SM_YVIRTUALSCREEN);
	BitBlt(
		target_dc,
		0,
		0,
		screen_width,
		screen_height,
		screen_dc,
		screen_x,
		screen_y,
		SRCCOPY | CAPTUREBLT
	);

	BITMAP screenshot_bmp;
	GetObject(target_bmp, sizeof(BITMAP), &screenshot_bmp);

	BITMAPINFOHEADER bitmap_header = { 0 };
	bitmap_header.biSize = sizeof(BITMAPINFOHEADER);
	bitmap_header.biWidth = screenshot_bmp.bmWidth;
	bitmap_header.biHeight = screenshot_bmp.bmHeight;
	bitmap_header.biPlanes = 1;
	bitmap_header.biBitCount = 32;
	bitmap_header.biCompression = BI_RGB;

	LONG bitmap_data_size = screenshot_bmp.bmWidth * screenshot_bmp.bmHeight * 4;
	BYTE *bitmap_data = new BYTE[bitmap_data_size];
	GetDIBits(
		target_dc,
		target_bmp,
		0,
		screenshot_bmp.bmHeight,
		bitmap_data,
		(BITMAPINFO *) &bitmap_header,
		DIB_RGB_COLORS
	);

	return bitmap_data;
}
