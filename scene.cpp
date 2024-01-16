#include "scene.h"
#include "resources.h"
#include "config.h"
#include "spritecontrol.h"

Scene::Scene(HWND window) 
	// It's of utmost importance the context comes first!
	// Else reality cursed, at the seams it will burst!!!
	: m_ctx(window),
	  m_sprites(SpriteGenerator().make(cfg.at(Opts::SpriteCount))),
	  m_choreographer((PatternName) cfg.at(Opts::Pattern), &m_sprites, &m_ctx) { }

void Scene::draw() {
	glViewport(0, 0, m_ctx.rect().right, m_ctx.rect().bottom);
	gluPerspective(45, 1.0 * m_ctx.rect().right / m_ctx.rect().bottom, 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_choreographer.update();
	for (Sprite *sprite : m_sprites) {
		sprite->draw(m_ctx);
	}

	glFlush();
	SwapBuffers(m_ctx.device());

	m_ctx.frame_count()++;
}
