#include "graphics.h"
#include "resources.h"

bool operator<(const Color &a, const Color &b) {
	return a.red < b.red; // does it really matter???
}

bool operator==(const Color &a, const Color &b) {
	return a.red == b.red; // again i ask though, why???
}

Palette::Palette(
	const Color &transparent, 
	const Color &scales, 
	const Color &scales_highlight, 
	const Color &scales_shadow, 
	const Color &horns, 
	const Color &eye, 
	const Color &whites,
	const Color &horns_shadow
) {
	push_back(transparent);
	push_back(scales);
	push_back(scales_highlight);
	push_back(scales_shadow);
	push_back(horns);
	push_back(eye);
	push_back(whites);
	push_back(horns_shadow);
}

Palette &Palette::operator=(const Palette &other) {
	clear();

	push_back(other[0]);
	push_back(other[1]);
	push_back(other[2]);
	push_back(other[3]);
	push_back(other[4]);
	push_back(other[5]);
	push_back(other[6]);
	push_back(other[7]);

	return *this;
}

GLubyte Bitmap::operator[](size_t index) const {
	return data[index];
}

const Texture *Texture::get(const Palette &palette, const Bitmap &bitmap) {
	std::pair<Id, Id> ids = std::make_pair(palette.id(), bitmap.id());
	TextureCache::iterator result = texture_cache.find(ids);

	if (result == texture_cache.end()) {
		texture_cache[ids] = new Texture(palette, bitmap);
	}

	return texture_cache[ids];
}

const Texture *Texture::of(PaletteName palette_name, BitmapName bitmap_name) {
	return get(PALETTES[palette_name], *load_bitmap(bitmap_name));
}

void Texture::apply() const {
	glBindTexture(GL_TEXTURE_2D, m_gl_tex_id);
}

const Palette &Texture::palette() const {
	return m_palette;
}

TextureCache Texture::texture_cache;

Texture::Texture(const Palette &palette, const Bitmap &bitmap)
	: m_palette(palette), m_bitmap(bitmap)
{
	glGenTextures(1, &m_gl_tex_id);
	glBindTexture(GL_TEXTURE_2D, m_gl_tex_id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// The documents make me a promise in writing:
	// "I'll map colors for you, waste not implementing!"
	// But I sit here defeated, my soul slowly dying,
	// As MSDN was just fucking lying.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, BITMAP_WH, BITMAP_WH, 0, GL_RGBA, GL_UNSIGNED_BYTE, data());
}

GLubyte *Texture::data() {
	GLubyte *texture_data = new GLubyte[BITMAP_WH * BITMAP_WH * 4];

	// The bitmap and palette are fused into one!
	// One turns to four and the process is done.
	for (int i = 0; i < BITMAP_WH * BITMAP_WH; i++) {
		Color palette_color = m_palette[m_bitmap[i]];

		texture_data[i * 4 + 0] = palette_color.red;
		texture_data[i * 4 + 1] = palette_color.green;
		texture_data[i * 4 + 2] = palette_color.blue;
		texture_data[i * 4 + 3] = palette_color.alpha;
	}

	return texture_data;
}