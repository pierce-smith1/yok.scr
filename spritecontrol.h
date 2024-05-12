#pragma once

#include <vector>
#include <functional>

#include "graphics.h"
#include "sprite.h"

bool signbit(float x);

typedef std::vector<Sprite *> Sprites;

enum SpritePattern {
	Roamers,
	Waves,
	Square,
	Bouncy,
	Lissajous,
	Rose,
	Lattice,
	_PATTERN_COUNT
};

class SpriteGenerator {
public:
	SpriteGenerator();

	Sprites make(unsigned int n);

private:
	const Texture *next_texture();
	PaletteName next_palette();

	std::vector<PaletteName> m_palettes;
};


//typedef std::function<void(Sprite *, Context *, float)> MoveFunction;
typedef void (*MoveFunction) (Sprite *, Context *, float);

class SpriteChoreographer {
public:
	SpriteChoreographer(SpritePattern pattern, Sprites sprites, Context *ctx);

	void update();
	void do_move_function(SpritePattern pattern, Sprite *sprite, Context *ctx, float offset);

private:
	static float hash(unsigned int n);

	Sprites m_sprites;
	Context *m_ctx;
	SpritePattern m_pattern;
	unsigned int m_id_offset;
};