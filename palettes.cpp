#include <map>
#include <algorithm>

#include "palettes.h"
#include "common.h"

Palette::Palette(const std::array<Color, _PALETTE_SIZE> &colors) {
	std::copy(colors.begin(), colors.end(), begin());
}

Palette::Palette(const std::initializer_list<Color> &i_list) {
	std::copy(i_list.begin(), i_list.end(), begin());
}

const Palette &RandomPalettes::random(int rng_token) {
	static std::map<int, Palette *> generated_palettes;

	auto result = generated_palettes.find(rng_token);

	if (result == generated_palettes.end()) {
		generated_palettes[rng_token] = new_random_palette();
	}

	return *generated_palettes.at(rng_token);
}

Palette *RandomPalettes::new_random_palette() {
	std::array<Color, _PALETTE_SIZE> colors;

	colors[PI_TRANSPARENT] = { 0, 0, 0, 0 };

	colors[PI_SCALES] = random_color();
	colors[PI_HORNS] = random_color();
	colors[PI_EYE] = random_color();
	colors[PI_WHITES] = { 240, 240, 240, 255 };

	colors[PI_SCALES_HIGHLIGHT] = lighten_color(colors[PI_SCALES]);
	colors[PI_SCALES_SHADOW] = darken_color(colors[PI_SCALES]);

	colors[PI_HORNS_SHADOW] = darken_color(colors[PI_HORNS]);

	return new Palette(colors);
}

Color RandomPalettes::random_color() {
	return { rand() % 255, rand() % 255, rand() % 255, 255 };
}

Color RandomPalettes::darken_color(const Color &color) {
	int new_red = std::get<RED>(color) / 2;
	int new_green = std::get<GREEN>(color) / 2;
	int new_blue = std::get<BLUE>(color) / 1.5; 

	return {
		std::clamp(new_red, 0, 255),
		std::clamp(new_green, 0, 255),
		std::clamp(new_blue, 0, 255),
		255,
	};
}

Color RandomPalettes::lighten_color(const Color &color) {
	int new_red = std::get<RED>(color) + 50;
	int new_green = std::get<GREEN>(color) + 50;
	int new_blue = std::get<BLUE>(color) + 50; 

	return {
		std::clamp(new_red, 0, 255),
		std::clamp(new_green, 0, 255),
		std::clamp(new_blue, 0, 255),
		255,
	};
}
