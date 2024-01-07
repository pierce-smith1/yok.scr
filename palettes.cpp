#include <map>
#include <algorithm>
#include <vector>

#include "palettes.h"
#include "common.h"

Palette::Palette(const std::array<Color, _PALETTE_SIZE> &colors) {
	std::copy(colors.begin(), colors.end(), begin());
}

Palette::Palette(const std::initializer_list<Color> &i_list) {
	std::copy(i_list.begin(), i_list.end(), begin());
}

const Palette *RandomPalettes::random(int rng_token) {
	static std::map<int, Palette *> generated_palettes;

	auto result = generated_palettes.find(rng_token);

	if (result == generated_palettes.end()) {
		generated_palettes[rng_token] = new_random_palette();
	}

	return generated_palettes.at(rng_token);
}

Palette *RandomPalettes::new_random_palette() {
	std::array<Color, _PALETTE_SIZE> colors;

	static float red_weight = rand() % 100 / 200.0f + 0.6f;
	static float green_weight = rand() % 100 / 200.0f + 0.35f;
	static float blue_weight = rand() % 100 / 200.0f + 0.5f;

	colors[PI_SCALES] = random_color(600, red_weight, green_weight, blue_weight);
	colors[PI_HORNS] = random_gray();
	colors[PI_EYE] = random_color(200, red_weight, green_weight, blue_weight);
	colors[PI_WHITES] = { 240, 240, 240, 255 };

	for (auto &color : colors) {
		color = noisify(color, 15.0f);
	}

	auto traits = random_traits();

	if (traits.find(GenerationTraits::ColorfulHorns) != traits.end()) {
		colors[PI_HORNS] = random_color(500, red_weight, green_weight, blue_weight);
	}

	if (traits.find(GenerationTraits::SwapHornsAndScales) != traits.end()) {
		std::swap(colors[PI_HORNS], colors[PI_SCALES]);
	}

	if (traits.find(GenerationTraits::BlackEyes) != traits.end()) {
		colors[PI_WHITES] = random_color(50, red_weight, green_weight, blue_weight);
		colors[PI_EYE] = random_color(700, red_weight, green_weight, blue_weight);
	}

	colors[PI_SCALES_HIGHLIGHT] = lighten_color(colors[PI_SCALES]);
	colors[PI_SCALES_SHADOW] = darken_color(colors[PI_SCALES]);

	colors[PI_HORNS_SHADOW] = darken_color(colors[PI_HORNS]);

	colors[PI_TRANSPARENT] = { 0, 0, 0, 0 };

	return new Palette(colors);
}

Color RandomPalettes::random_color(int budget, float red_weight, float green_weight, float blue_weight) {
	int max_budget = 4.0f / 5.0f * budget;

	int red = 0;
	int green = 0;
	int blue = 0;

	while (budget > 0) {
		int budget_chunk = budget < 30 ? budget : 30;

		int channel = rand() % 3;

		if (channel == RED && red >= max_budget) continue;
		if (channel == GREEN && green >= max_budget) continue;
		if (channel == BLUE && blue >= max_budget) continue;

		switch (channel) {
			case RED: red += budget_chunk * red_weight; break;
			case GREEN: green += budget_chunk * green_weight; break; 
			case BLUE: blue += budget_chunk * blue_weight; break; 
		}

		budget -= budget_chunk;
	}

	return {
		std::clamp(red, 0, 255),
		std::clamp(green, 0, 255),
		std::clamp(blue, 0, 255),
		255,
	};
}

Color RandomPalettes::random_gray() {
	int luminance = rand() % 255;
	return { luminance, luminance, luminance, 255 };
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

Color RandomPalettes::noisify(const Color &color, float degree) {
	int new_red = std::get<RED>(color) + (((rand() % 100) / 100) * degree);
	int new_green = std::get<GREEN>(color) + (((rand() % 100) / 100) * degree);
	int new_blue = std::get<BLUE>(color) + (((rand() % 100) / 100) * degree);

	return {
		std::clamp(new_red, 0, 255),
		std::clamp(new_green, 0, 255),
		std::clamp(new_blue, 0, 255),
		255,
	};
}

std::set<RandomPalettes::GenerationTraits> RandomPalettes::random_traits() {
	const static std::map<GenerationTraits, float> trait_chances = {
		{ GenerationTraits::ColorfulHorns, 0.5f },
		{ GenerationTraits::SwapHornsAndScales, 0.1f },
		{ GenerationTraits::BlackEyes, 0.05f },
	};

	std::set<GenerationTraits> traits;

	for (const auto &pair : trait_chances) {
		if (rand() % 100 / 100.0f < pair.second) {
			traits.insert(pair.first);
		}
	}
	
	return traits;
}
