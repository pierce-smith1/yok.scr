#pragma once

#include <algorithm>
#include <iterator>
#include <utility>
#include <map>
#include <string>
#include <vector>

#include "context.h"

enum PaletteName;
enum BitmapName;

static unsigned int BITMAP_WH = 128;

//using Color = std::tuple<BYTE, BYTE, BYTE, BYTE>;
struct Color {
	Color() 
		: red(0), green(0), blue(0), alpha(0) {}

	Color(BYTE red, BYTE green, BYTE blue, BYTE alpha)
		: red(red), green(green), blue(blue), alpha(alpha) {}

	BYTE red;
	BYTE green;
	BYTE blue;
	BYTE alpha;
};

bool operator<(const Color &a, const Color &b);
bool operator==(const Color &a, const Color &b);

typedef unsigned int Id;
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

class Palette : public Identifiable<std::vector<Color> > {
public:
	Palette() {}
	Palette(
		const Color &transparent, 
		const Color &scales, 
		const Color &scales_highlight, 
		const Color &scales_shadow, 
		const Color &horns, 
		const Color &eye, 
		const Color &whites,
		const Color &horns_shadow
	);
	Palette &operator=(const Palette &other);
};

struct Bitmap : public Identifiable<Empty> {
	Bitmap(const GLubyte *data) : data(data) {}

	GLubyte operator[](size_t index) const;

	const GLubyte *data;
};

class Texture;
typedef std::map<std::pair<Id, Id>, Texture *> TextureCache;
class Texture {
public:
	static const Texture *get(const Palette &palette, const Bitmap &bitmap);
	static const Texture *of(PaletteName palette_name, BitmapName bitmap_name);

	const Palette &palette() const;

	void apply() const;

private:
	Texture(const Palette &palette, const Bitmap &bitmap);

	GLubyte *data();

	static TextureCache texture_cache;

	unsigned int m_gl_tex_id;
	const Palette &m_palette;
	const Bitmap &m_bitmap;
};

struct Point {
	Point(float x, float y) : x(x), y(y) {}

	float x;
	float y;
};
