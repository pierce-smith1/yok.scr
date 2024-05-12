#include "scene.h"
#include "resources.h"
#include "config.h"
#include "spritecontrol.h"

Scene::Scene(HWND window) 
	// It's of utmost importance the context comes first!
	// Else reality cursed, at the seams it will burst!!!
	: m_ctx(window),
	  m_sprites(SpriteGenerator().make(cfg[SpriteCount])),
	  m_choreographer((SpritePattern) (int) cfg[YonkPattern], m_sprites, &m_ctx) { }

void Scene::draw() {
	// GLsizei view_width = m_ctx.rect().right;
	// GLsizei view_height = m_ctx.rect().bottom;
	GLsizei view_width = ANIMATION_WIDTH;
	GLsizei view_height = ANIMATION_HEIGHT;

	glViewport(0, 0, view_width, view_height);
	gluPerspective(45, 1.0 * view_width / view_height, 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_choreographer.update();
	for (Sprites::iterator it = m_sprites.begin(); it != m_sprites.end(); ++it) {
		(*it)->draw(m_ctx);
	}

	glFlush();
	SwapBuffers(m_ctx.device());

	m_ctx.frame_count()++;
}
