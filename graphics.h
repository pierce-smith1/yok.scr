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
#include "common.h"

enum BitmapName;

constexpr static unsigned int BITMAP_WH = 128;

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