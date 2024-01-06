#pragma once

#include <array>
#include <initializer_list>

#include "common.h"

enum class PaletteName;

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

	static const Palette &random(int rng_token);

private:
	static Palette *new_random_palette();
};

