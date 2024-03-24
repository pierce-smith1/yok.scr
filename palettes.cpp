#include <map>
#include <algorithm>
#include <vector>
#include <random>

#include "palettes.h"
#include "common.h"
#include "config.h"
#include "configdialog.h"

PaletteData::PaletteData(const std::array<Color, _PALETTE_SIZE> &colors) {
	std::copy(colors.begin(), colors.end(), begin());
}

PaletteData::PaletteData(const std::initializer_list<Color> &i_list) {
	std::copy(i_list.begin(), i_list.end(), begin());
}

// The hex strings provided must be of the form #RRGGBB, where XX are hex numbers.
// The alpha is always assumed to be 255.
// The resulting palette will always start with { 0, 0, 0, 0 }.
PaletteData::PaletteData(const std::array<std::string, _PALETTE_SIZE - 1> &hex_strings) {
	*begin() = { 0, 0, 0, 0 };
	std::transform(hex_strings.begin(), hex_strings.end(), begin() + 1, [](const std::string &hex_string) -> Color {
		auto red = std::stoi(hex_string.substr(1, 2), nullptr, 16);
		auto green = std::stoi(hex_string.substr(3, 2), nullptr, 16);
		auto blue = std::stoi(hex_string.substr(5, 2), nullptr, 16);

		return { red, green, blue, 255 };
	});
}

const PaletteData *RandomPalettes::random(int rng_token) {
	static std::map<int, PaletteData *> generated_palettes;

	auto result = generated_palettes.find(rng_token);

	if (result == generated_palettes.end()) {
		generated_palettes[rng_token] = new_random_palette();
	}

	return generated_palettes.at(rng_token);
}

PaletteData *RandomPalettes::new_random_palette() {
	static int bias_intensity = (int) (20.0 / ((cfg[Cfg::MaxColors] + 4.0) / Cfg::MaxColors.range.second));

	static double red_bias = rand() % bias_intensity * (rand() % 2 ? -1.0 : 1.0);
	static double green_bias = rand() % bias_intensity * (rand() % 2 ? -1.0 : 1.0);
	static double blue_bias = rand() % bias_intensity * (rand() % 2 ? -1.0 : 1.0);

	std::array<Color, _PALETTE_SIZE> colors;

	colors[PI_SCALES] = random_color();
	colors[PI_HORNS] = random_gray();
	colors[PI_EYE] = darken_color(random_color());

	for (auto &color : colors) {
		color = noisify(color, 15.0f);
		color = recolorize(color, red_bias, green_bias, blue_bias);
	}

	colors[PI_WHITES] = { 240, 240, 240, 255 };

	auto traits = random_traits();

	if (traits.find(GenerationTraits::ColorfulHorns) != traits.end()) {
		colors[PI_HORNS] = random_color();
	}

	if (traits.find(GenerationTraits::PastelScales) != traits.end()) {
		colors[PI_SCALES] = lighten_color(lighten_color(colors[PI_SCALES]));
		colors[PI_WHITES] = { 255, 255, 255, 255 };
	}

	if (traits.find(GenerationTraits::SwapHornsAndScales) != traits.end()) {
		std::swap(colors[PI_HORNS], colors[PI_SCALES]);
	}

	if (traits.find(GenerationTraits::BlackEyes) != traits.end()) {
		colors[PI_WHITES] = { 0, 0, 0, 255 };
		colors[PI_EYE] = random_color();
	}

	colors[PI_SCALES_HIGHLIGHT] = lighten_color(colors[PI_SCALES]);
	colors[PI_SCALES_SHADOW] = darken_color(colors[PI_SCALES]);
	colors[PI_HORNS_SHADOW] = darken_color(colors[PI_HORNS]);

	if (traits.find(GenerationTraits::CrystalBody) != traits.end()) {
		colors[PI_SCALES_HIGHLIGHT] = darken_color(colors[PI_SCALES]);
		colors[PI_SCALES_SHADOW] = lighten_color(colors[PI_SCALES]);
		colors[PI_HORNS_SHADOW] = lighten_color(colors[PI_HORNS]);
	}

	colors[PI_TRANSPARENT] = { 0, 0, 0, 0 };

	return new PaletteData(colors);
}

Color RandomPalettes::random_color() {
	std::vector<int> values = { 
		rand() % 50,
		225 - rand() % 50,
		rand() % 225
	};

	std::shuffle(values.begin(), values.end(), std::mt19937(std::random_device()()));

	return {
		values[0],
		values[1],
		values[2],
		255
	};
}

Color RandomPalettes::random_gray() {
	int luminance = rand() % 255;
	return { luminance, luminance, luminance, 255 };
}

Color RandomPalettes::darken_color(const Color &color) {
	double new_red = std::get<RED>(color) / 2;
	double new_green = std::get<GREEN>(color) / 2;
	double new_blue = std::get<BLUE>(color) / 1.5; 

	return {
		std::clamp(cast<int>(new_red), 0, 255),
		std::clamp(cast<int>(new_green), 0, 255),
		std::clamp(cast<int>(new_blue), 0, 255),
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

Color RandomPalettes::noisify(const Color &color, double degree) {
	double new_red = std::get<RED>(color) + (((rand() % 100) / 100.0) * degree);
	double new_green = std::get<GREEN>(color) + (((rand() % 100) / 100.0) * degree);
	double new_blue = std::get<BLUE>(color) + (((rand() % 100) / 100.0) * degree);

	return {
		std::clamp(cast<int>(new_red), 0, 255),
		std::clamp(cast<int>(new_green), 0, 255),
		std::clamp(cast<int>(new_blue), 0, 255),
		255,
	};
}

Color RandomPalettes::recolorize(const Color &color, double red_weight, double green_weight, double blue_weight) {
	double new_red = std::get<RED>(color) + (rand() % 100 / 50.0) * red_weight;
	double new_green = std::get<GREEN>(color) + (rand() % 100 / 50.0) * green_weight;
	double new_blue = std::get<BLUE>(color) + (rand() % 100 / 50.0) * blue_weight;

	return {
		std::clamp(cast<int>(new_red), 0, 255),
		std::clamp(cast<int>(new_green), 0, 255),
		std::clamp(cast<int>(new_blue), 0, 255),
		255,
	};
}

std::set<RandomPalettes::GenerationTraits> RandomPalettes::random_traits() {
	auto random_inverse_falloff_percent = [](double scale) -> double {
		if (rand() % 50 == 0) {
			return 1.0; // Very ocassionally, a trait will have 100% chance
		}

		return 1.0 / ((rand() % 300 / 100.0 + 2) + 0.25) * scale;
	};

	const static std::map<GenerationTraits, double> trait_chances = {
		{ GenerationTraits::ColorfulHorns, random_inverse_falloff_percent(0.7) },
		{ GenerationTraits::SwapHornsAndScales, random_inverse_falloff_percent(0.4) },
		{ GenerationTraits::BlackEyes, random_inverse_falloff_percent(0.2) },
		{ GenerationTraits::PastelScales, random_inverse_falloff_percent(0.3) },
		{ GenerationTraits::CrystalBody, random_inverse_falloff_percent(0.2) },
	};

	std::set<GenerationTraits> traits;

	for (const auto &pair : trait_chances) {
		if (rand() % 100 / 100.0 < pair.second) {
			traits.insert(pair.first);
		}
	}
	
	return traits;
}

std::vector<Palettes::Definition> PaletteGroups::palettes_of_group(PaletteGroup group) {
	std::vector<Palettes::Definition> palettes;
	std::copy_if(Palettes::All.begin(), Palettes::All.end(), std::back_inserter(palettes), [&](const Palettes::Definition &bitmap) {
		return group == PaletteGroup::All || bitmap.group == group;
	});

	return palettes;
}
