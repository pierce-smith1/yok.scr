#include "palettes.h"

Palette::Palette(const std::initializer_list<Color> &i_list) {
	std::copy(i_list.begin(), i_list.end(), begin());
}

const Palette &Palette::random(int rng_token) {
	static std::map<int, Palette *> generated_palettes;

	auto result = generated_palettes.find(rng_token);

	if (result == generated_palettes.end()) {
		generated_palettes[rng_token] = new_random_palette();
	}

	return *generated_palettes.at(rng_token);
}

Palette *Palette::new_random_palette() {
	
}
