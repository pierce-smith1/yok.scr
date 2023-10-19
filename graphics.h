#pragma once

#include <array>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <utility>
#include <map>
#include <string>

#include "context.h"

enum PaletteName;
enum BitmapName;

constexpr static unsigned int BITMAP_WH = 128;

using Color = std::tuple<BYTE, BYTE, BYTE, BYTE>;
enum Channel {
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	ALPHA = 3
};

using Id = unsigned int;
static Id running_id = 0;

class Empty {};
template <typename Base> class Identifiable : public Base {
public:
	Identifiable() : m_id(running_id++) {}

	Id id() const {
		return m_id;
	}

private:
	const Id m_id;
};

enum PaletteIndex {
	PI_TRANSPARENT = 0,      
	PI_SCALES = 1,          
	PI_SCALES_HIGHLIGHT = 2,  
	PI_SCALES_SHADOW = 3,     
	PI_HORNS = 4,
	PI_EYE = 5,
	PI_WHITES = 6,
	PI_HORNS_SHADOW = 7,
	_PALETTE_SIZE
};
class Palette : public Identifiable<std::array<Color, _PALETTE_SIZE>> {
public:
	Palette(const std::initializer_list<Color> &i_list);
};

class Bitmap : public Identifiable<std::array<GLubyte, BITMAP_WH * BITMAP_WH>> {
public:
	Bitmap(const std::initializer_list<GLubyte> &i_list);
	Bitmap(const GLubyte *data);
};

class Texture {
public:
	static const Texture *get(const Palette &palette, const Bitmap &bitmap);
	static const Texture *of(PaletteName palette_name, BitmapName bitmap_name);

	const Palette &palette() const;

	void apply() const;

private:
	Texture(const Palette &palette, const Bitmap &bitmap);
	Texture(const Texture &texture) = delete;
	Texture &operator=(const Texture &texture) = delete;

	GLubyte *data();

	static std::map<std::pair<Id, Id>, Texture *> texture_cache;

	unsigned int m_gl_tex_id;
	const Palette &m_palette;
	const Bitmap &m_bitmap;
};

using Point = std::pair<GLfloat, GLfloat>;
enum Coord {
	X = 0,
	Y = 1
};