#pragma once

#include <vector>

#include "context.h"
#include "sprite.h"
#include "spritecontrol.h"

class Scene {
public:
	Scene(HWND window);

	void draw();

private:
	Context m_ctx;
	Sprites m_sprites;
	SpriteChoreographer m_choreographer;
};