#include "scene.h"
#include "resources.h"
#include "config.h"
#include "spritecontrol.h"

Scene::Scene(HWND window) 
	// It's of utmost importance the context comes first!
	// Else reality cursed, at the seams it will burst!!!
	: m_ctx(window),
	  m_sprites(SpriteGenerator().make(cfg.at(SpriteCount))),
	  m_choreographer((PatternName) cfg.at(YonkPattern), &m_sprites, &m_ctx) { }

void Scene::draw() {
	glViewport(0, 0, m_ctx.rect().right, m_ctx.rect().bottom);
	gluPerspective(45, 1.0 * m_ctx.rect().right / m_ctx.rect().bottom, 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint bg_tex_id;

	glGenTextures(1, &bg_tex_id);
	glBindTexture(GL_TEXTURE_2D, bg_tex_id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, get_background_rgba().data());

	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);

	glEnd();

	m_choreographer.update();
	for (Sprite *sprite : m_sprites) {
		sprite->draw(m_ctx);
	}

	glFlush();
	SwapBuffers(m_ctx.device());

	m_ctx.frame_count()++;
}

std::vector<BYTE> Scene::get_background_rgba() {
	BYTE bits[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 143, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 0, 103, 92, 89, 122, 102, 92, 89, 145, 117, 102, 98, 170, 103, 92, 89, 255, 102, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 105, 101, 50, 108, 95, 92, 255, 108, 95, 92, 255, 108, 95, 92, 255, 108, 96, 92, 255, 102, 91, 88, 255, 115, 101, 97, 255, 120, 104, 100, 255, 121, 105, 101, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 171, 103, 92, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 172, 103, 92, 89, 245, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 89, 255, 119, 103, 99, 255, 118, 103, 99, 255, 103, 92, 89, 255, 103, 92, 89, 255, 120, 104, 100, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 135, 103, 92, 89, 255, 116, 101, 97, 255, 105, 93, 90, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 217, 103, 92, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 246, 112, 99, 95, 255, 112, 98, 95, 255, 102, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 114, 103, 92, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 0, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 91, 88, 64, 77, 61, 56, 255, 95, 82, 79, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 78, 64, 59, 255, 75, 61, 56, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 20, 77, 61, 56, 243, 77, 61, 56, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 75, 62, 56, 255, 58, 43, 36, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 244, 103, 92, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 24, 77, 61, 56, 255, 76, 59, 54, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 75, 62, 56, 255, 58, 43, 36, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 98, 87, 84, 255, 28, 18, 15, 255, 28, 18, 15, 100, 28, 18, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 255, 77, 61, 56, 14, 103, 92, 89, 181, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 96, 83, 80, 255, 103, 92, 89, 255, 75, 62, 56, 255, 58, 43, 36, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 26, 16, 13, 255, 28, 18, 15, 255, 28, 18, 15, 255, 28, 18, 15, 255, 28, 18, 15, 159, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 0, 0, 0, 0, 0, 103, 92, 89, 181, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 81, 66, 62, 255, 103, 92, 89, 255, 101, 89, 86, 255, 57, 42, 35, 255, 103, 92, 89, 255, 102, 92, 88, 255, 197, 99, 128, 255, 213, 100, 134, 255, 210, 100, 133, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 28, 18, 15, 255, 28, 18, 15, 255, 32, 25, 23, 255, 28, 18, 15, 255, 28, 18, 15, 159, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 95, 82, 79, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 112, 93, 93, 255, 212, 100, 134, 255, 212, 100, 134, 255, 202, 99, 130, 255, 105, 92, 90, 255, 143, 95, 106, 255, 99, 92, 87, 255, 103, 92, 89, 255, 104, 93, 90, 255, 28, 18, 15, 255, 28, 18, 15, 255, 28, 18, 15, 255, 28, 18, 15, 255, 28, 18, 15, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 255, 103, 92, 89, 255, 113, 99, 96, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 96, 84, 80, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 103, 92, 89, 255, 117, 93, 95, 255, 103, 92, 89, 255, 94, 82, 78, 255, 103, 92, 89, 255, 99, 92, 88, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 28, 18, 15, 255, 31, 24, 22, 255, 28, 18, 15, 255, 28, 18, 15, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 255, 111, 98, 94, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 81, 66, 62, 255, 103, 92, 89, 255, 86, 71, 67, 255, 77, 61, 56, 255, 77, 61, 56, 255, 78, 62, 57, 255, 77, 61, 56, 255, 79, 64, 59, 255, 79, 64, 59, 255, 77, 61, 56, 255, 78, 62, 57, 255, 103, 92, 89, 255, 104, 93, 90, 255, 28, 18, 15, 255, 28, 18, 15, 255, 28, 18, 15, 255, 28, 18, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 9, 102, 92, 89, 255, 115, 100, 97, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 81, 66, 62, 255, 103, 92, 89, 255, 90, 76, 72, 255, 103, 92, 89, 255, 98, 86, 83, 255, 98, 86, 82, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 90, 255, 77, 61, 56, 255, 99, 88, 84, 255, 28, 18, 15, 255, 28, 18, 15, 255, 28, 18, 15, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 245, 121, 105, 101, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 81, 66, 62, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 103, 92, 89, 119, 103, 92, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 0, 103, 92, 89, 117, 77, 61, 56, 0, 28, 18, 15, 128, 28, 18, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 13, 120, 104, 100, 255, 103, 92, 89, 255, 120, 104, 100, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 255, 103, 92, 89, 255, 120, 104, 100, 255, 113, 99, 95, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 103, 92, 89, 31, 103, 92, 89, 240, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 76, 60, 55, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 240, 77, 61, 56, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 100, 103, 92, 89, 255, 120, 104, 100, 255, 121, 105, 100, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 91, 78, 74, 255, 77, 61, 56, 255, 77, 61, 56, 255, 88, 74, 70, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 195, 113, 99, 96, 255, 120, 104, 100, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 76, 60, 55, 255, 77, 61, 56, 255, 103, 92, 89, 255, 121, 105, 101, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 137, 83, 68, 63, 0, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 104, 93, 90, 255, 120, 104, 100, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 104, 93, 90, 255, 77, 61, 56, 255, 103, 92, 89, 255, 120, 104, 100, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 34, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 0, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 0, 103, 92, 89, 2, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 77, 61, 56, 255, 88, 74, 69, 255, 120, 104, 100, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 0, 0, 0, 0, 0, 77, 61, 56, 0, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 7, 103, 92, 89, 241, 103, 92, 89, 255, 105, 94, 90, 255, 110, 97, 93, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 102, 91, 88, 255, 77, 61, 56, 255, 108, 95, 92, 255, 102, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 0, 0, 0, 0, 0, 77, 61, 56, 88, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 224, 103, 92, 89, 255, 106, 94, 91, 255, 116, 101, 97, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 78, 62, 57, 255, 77, 61, 56, 255, 111, 97, 94, 255, 114, 100, 96, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 204, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 53, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 224, 103, 92, 89, 255, 121, 104, 100, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 251, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 27, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 3, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 0, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 79, 63, 58, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 109, 96, 93, 255, 105, 94, 90, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 254, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 0, 0, 0, 0, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 142, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 242, 77, 61, 56, 216, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 103, 92, 89, 255, 109, 96, 93, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 42, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 0, 103, 92, 89, 66, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 0, 0, 0, 0, 0, 77, 61, 56, 0, 77, 61, 56, 227, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 40, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 0, 0, 0, 0, 0, 0, 0, 0, 77, 61, 56, 0, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 3, 103, 92, 89, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103, 92, 89, 120, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 255, 103, 92, 89, 0, 0, 0, 0, 0, 77, 61, 56, 0, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 255, 77, 61, 56, 90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::vector<BYTE> data(32 * 32 * 4, 0);

	for (size_t y = 0; y < 32; y++) {
		for (size_t x = 0; x < 32; x++) {
			size_t i_source = y * (32 * 4) + (x * 4);
			size_t i_target = (y * 4) + 32 * (x * 4);
	
			data[i_target] = bits[i_source];
			data[i_target + 1] = bits[i_source + 1];
			data[i_target + 2] = bits[i_source + 2];
			data[i_target + 3] = bits[i_source + 3];
		}
	}	 

	return data;
}
