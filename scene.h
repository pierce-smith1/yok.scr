#pragma once

#include <vector>

#include "context.h"
#include "sprite.h"
#include "spritecontrol.h"
#include "common.h"

class Scene {
public:
	Scene(HWND window);

	void draw();

private:
	void draw_background();
	BYTE *get_background_rgba();

	static GLuint background_tex_id;

	Context m_ctx;
	Sprites m_sprites;
	SpriteChoreographer m_choreographer;
};