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

const static double M_PI = std::acos(-1);

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
Point operator+(const Point &l, const Point &r);
Point &operator+=(Point &l, const Point &r);
Point operator-(const Point &l, const Point &r);
Point &operator-=(Point &l, const Point &r);
Point operator*(const Point &l, const GLdouble &r);
Point &operator*=(Point &l, const GLdouble &r);
Point operator/(const Point &l, const GLdouble &r);
Point &operator/=(Point &l, const GLdouble &r);

namespace MathUtils {
	// Gives a random value between (1 - negative_variation; 1 + positive_variation).
	// Exponent affects the bias of the curve towards 1 before rapidly diverging at the edges.
	// Slope affects how linear the curve is. Slope = 1 behaves like exponent = 1.
	// Variation shouldn't result in a number below 0. Exponent must be larger than 0. Slope should be from 0 to 1.
	double random_curve(double negative_variation, double positive_variation, double exponent, double slope);

	// Converts a vector to an angle in the range (-M_PI, M_PI].
	double get_angle(const Point &vector);

	// Wraps angles to be inside the range (-M_PI, M_PI]
	double wrap_angle(double angle);

	double get_magnitude(const Point &vector);

	Point normalize(const Point &vector);
}

enum Coord {
	X = 0,
	Y = 1
};
