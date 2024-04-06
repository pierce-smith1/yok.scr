#pragma once

#include <array>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <utility>
#include <map>
#include <string>

#include "context.h"
#include "palettes.h"
#include "bitmaps.h"
#include "common.h"

class Texture {
public:
	static const Texture *get(const PaletteData &palette, const BitmapData &bitmap);
	static const Texture *of(const Palettes::Definition &palette, const Bitmaps::Definition &bitmap);
	static const Texture *of(const PaletteData *palette, const Bitmaps::Definition &bitmap);

	const PaletteData &palette() const;

	void apply() const;

	GLubyte *data() const;

private:
	Texture(const PaletteData &palette, const BitmapData &bitmap);
	Texture(const Texture &texture) = delete;
	Texture &operator=(const Texture &texture) = delete;


	static std::map<std::pair<Id, Id>, Texture *> texture_cache;

	unsigned int m_gl_tex_id;
	const PaletteData &m_palette;
	const BitmapData &m_bitmap;
};

using Point = std::pair<GLdouble, GLdouble>;
enum Coord {
	X = 0,
	Y = 1
};