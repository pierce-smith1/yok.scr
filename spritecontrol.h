#pragma once

#include <vector>
#include <functional>

#include "graphics.h"
#include "sprite.h"

using Sprites = std::vector<Sprite *>;

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

enum class PaletteGroup {
	All,
	Canon,
	NonCanon,
	_PALETTE_OPTION_COUNT
};

class SpriteGenerator {
public:
	SpriteGenerator();

	Sprites make(unsigned int n) const;

private:
	const Texture *next_texture() const;
	PaletteName next_palette() const;

	std::vector<PaletteName> m_palettes;
};

class SpriteChoreographer {
public:
	SpriteChoreographer(SpritePattern pattern, Sprites sprites, Context *ctx);

	void update();

private:
	using MoveFunction = std::function<void(Sprite *, Context *, float)>;

	static float hash(unsigned int n);

	Sprites m_sprites;
	Context *m_ctx;
	SpritePattern m_pattern;
	unsigned int m_id_offset;

	static std::map<SpritePattern, MoveFunction> move_functions;
};