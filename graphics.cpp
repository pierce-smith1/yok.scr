#include "graphics.h"
#include "resources.h"

// Here, textures are born not by loading from disk...
Palette::Palette(const std::initializer_list<Color> &i_list) {
	std::copy(i_list.begin(), i_list.end(), begin());
}

// But by our own hands, with ingredients.
Bitmap::Bitmap(const std::initializer_list<GLubyte> &i_list) {
	std::copy(i_list.begin(), i_list.end(), begin());
}

// Lazily baked, not wasting our time...
const Texture *Texture::get(const Palette &palette, const Bitmap &bitmap) {
	auto ids = std::make_pair(palette.id(), bitmap.id());
	auto result = texture_cache.find(ids);

	if (result == texture_cache.end()) {
		texture_cache[ids] = new Texture(palette, bitmap);
	}

	return texture_cache.at(ids);
}

// And make calling convenient, 'fore I wish any crimes.
const Texture *Texture::of(PaletteName palette_name, BitmapName bitmap_name) {
	return get(PALETTES.at(palette_name), *load_bitmap(bitmap_name));
}

Bitmap::Bitmap(const GLubyte *data) {
	std::copy(data, data + size(), begin());
}

void Texture::apply() const {
	glBindTexture(GL_TEXTURE_2D, m_gl_tex_id);
}

const Palette &Texture::palette() const {
	return m_palette;
}

std::map<std::pair<Id, Id>, Texture *> Texture::texture_cache{};

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
	GLubyte *texture_data = new GLubyte[m_bitmap.size() * 4];

	// The bitmap and palette are fused into one!
	// One turns to four and the process is done.
	for (int i = 0; i < m_bitmap.size(); i++) {
		Color palette_color = m_palette[m_bitmap.data()[i]];

		texture_data[i * 4 + RED] = std::get<RED>(palette_color);
		texture_data[i * 4 + GREEN] = std::get<GREEN>(palette_color);
		texture_data[i * 4 + BLUE] = std::get<BLUE>(palette_color);
		texture_data[i * 4 + ALPHA] = std::get<ALPHA>(palette_color);
	}

	return texture_data;
}