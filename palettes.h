#pragma once

#include <array>
#include <set>
#include <initializer_list>
#include <vector>

#include "common.h"

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
class PaletteData : public Identifiable<std::array<Color, _PALETTE_SIZE>> {
public:
	PaletteData(const std::array<Color, _PALETTE_SIZE> &colors);
	PaletteData(const std::initializer_list<Color> &i_list);
	PaletteData(const std::array<std::string, _PALETTE_SIZE - 1> &hex_strings);
};

enum class PaletteGroup {
	All,
	Canon,
	NonCanon,
	RandomlyGenerated,
	_PALETTE_OPTION_COUNT
};

class RandomPalettes {
public:
	static const PaletteData *random(int rng_token);

private:
	enum class GenerationTraits {
		ColorfulHorns,
		SwapHornsAndScales,
		BlackEyes,
		PastelScales,
		CrystalBody,
	};

	static PaletteData *new_random_palette();
	static Color random_color();
	static Color random_gray();
	static Color darken_color(const Color &color);
	static Color lighten_color(const Color &color);
	static Color noisify(const Color &color, float degree = 1.0f);
	static Color recolorize(const Color &color, float red_weight, float green_weight, float blue_weight);
	static std::set<GenerationTraits> random_traits();
};

struct Palettes {
	struct Definition {
		auto operator<=>(const Definition &other) const = default;

		const size_t index;
		const std::wstring name;
		const PaletteGroup group;
		const PaletteData palette;
	};

	inline const static Definition Aemil = {
		.index = __COUNTER__,
		.name = L"aemil",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#56eb8e",
			"#84f5c3",
			"#1d9550",
			"#e29a56",
			"#e16a72",
			"#dff9eb",
			"#966336",
		}},
	};

	inline const static Definition Autumn = {
		.index = __COUNTER__,
		.name = L"autumn",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#db8313",
			"#be9275",
			"#904a08",
			"#574d3c",
			"#904a08",
			"#f3e6e9",
			"#000000",
		}},
	};

	inline const static Definition Ascent = {
		.index = __COUNTER__,
		.name = L"ascent",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#1963c4",
			"#1963c4",
			"#0b407d",
			"#8c96dd",
			"#8c96dd",
			"#ffffff",
			"#506bbc",
		}},
	};

	inline const static Definition Azul = {
		.index = __COUNTER__,
		.name = L"azul",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#1aadd9",
			"#a1d0e5",
			"#296d9e",
			"#4d8db9",
			"#296d9e",
			"#ffffff",
			"#103050",
		}},
	};

	inline const static Definition Bliss = {
		.index = __COUNTER__,
		.name = L"bliss",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#73981e",
			"#73981e",
			"#3d5317",
			"#6a96f2",
			"#282438",
			"#eaf2ff",
			"#3b73ee",
		}},
	};

	inline const static Definition Chasnah = {
		.index = __COUNTER__,
		.name = L"chasnah",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#6f31dd",
			"#932de3",
			"#3a12a2",
			"#e30efe",
			"#e227ff",
			"#f6f5f4",
			"#e959f5",
		}},
	};

	inline const static Definition Crystal = {
		.index = __COUNTER__,
		.name = L"crystal",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#38399e",
			"#51b7cf",
			"#1c63d8",
			"#434ad9",
			"#0a1a4a",
			"#eaf2ff",
			"#0a1a4a",
		}},
	};

	inline const static Definition Dejil = {
		.index = __COUNTER__,
		.name = L"dejil",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#f0f0f0",
			"#ffffff",
			"#bebebe",
			"#c6bfa0",
			"#f0f0f0",
			"#f0f0f0",
			"#ab925e",
		}},
	};

	inline const static Definition Dzune = {
		.index = __COUNTER__,
		.name = L"dzune",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#e56161",
			"#c75653",
			"#ab3232",
			"#4b1111",
			"#e38663",
			"#faecec",
			"#370808",
		}},
	};

	inline const static Definition Ellai = {
		.index = __COUNTER__,
		.name = L"ellai",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#97cc72",
			"#caecb1",
			"#338527",
			"#72482d",
			"#f04f2a",
			"#ffffff",
			"#45160e",
		}},
	};

	inline const static Definition Evjar = {
		.index = __COUNTER__,
		.name = L"evjar",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#6c2717",
			"#8b4231",
			"#2f1c31",
			"#220e32",
			"#f04f2a",
			"#ffffff",
			"#0e0019",
		}},
	};

	inline const static Definition Follow = {
		.index = __COUNTER__,
		.name = L"follow",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#de5a29",
			"#d0742d",
			"#0a1a4a",
			"#c0982b",
			"#0a1a4a",
			"#eaf2ff",
			"#9f894e",
		}},
	};

	inline const static Definition Friend = {
		.index = __COUNTER__,
		.name = L"friend",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#a08d77",
			"#bcab96",
			"#433a2d",
			"#433a2d",
			"#191713",
			"#e3ddd5",
			"#191713",
		}},
	};

	inline const static Definition Fruit = {
		.index = __COUNTER__,
		.name = L"fruit",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#f05f7c",
			"#ed7890",
			"#d3405e",
			"#4da796",
			"#f7e570",
			"#dead5b",
			"#ebde6e",
		}},
	};

	inline const static Definition Gimeljoy = {
		.index = __COUNTER__,
		.name = L"gimeljoy",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#ffdec4",
			"#fac499",
			"#ffb376",
			"#e7b085",
			"#000000",
			"#ffa154",
			"#ce9161",
		}},
	};

	inline const static Definition Gimelsad = {
		.index = __COUNTER__,
		.name = L"gimelsad",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#c4e7ff",
			"#81c0ec",
			"#5fa7da",
			"#63a0cb",
			"#000000",
			"#37abfc",
			"#3d80af",
		}},
	};

	inline const static Definition GimelYOO = {
		.index = __COUNTER__,
		.name = L"gimelYOO",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#f9c4ff",
			"#f492ff",
			"#f06cff",
			"#dd84e7",
			"#000000",
			"#ed44ff",
			"#c767d1",
		}},
	};

	inline const static Definition Home = {
		.index = __COUNTER__,
		.name = L"home",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#a26177",
			"#7d5a7c",
			"#593b42",
			"#7e3e4c",
			"#4149c6",
			"#ffe1e7",
			"#593b42",
		}},
	};

	inline const static Definition Jaela = {
		.index = __COUNTER__,
		.name = L"jaela",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#f8b765",
			"#f6d58e",
			"#a95a2e",
			"#f6585b",
			"#e85533",
			"#ffffff",
			"#b5282b",
		}},
	};

	inline const static Definition Jergh = {
		.index = __COUNTER__,
		.name = L"jergh",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#e7e7e7",
			"#f3f3eb",
			"#b4bcd1",
			"#79c569",
			"#d08ab7",
			"#f0f0f0",
			"#cf90b8",
		}},
	};

	inline const static Definition Kirii = {
		.index = __COUNTER__,
		.name = L"kirii",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#4ad4bf",
			"#44e0d1",
			"#2c9197",
			"#77807f",
			"#6483bc",
			"#d8fff9",
			"#525353",
		}},
	};

	inline const static Definition Kraza = {
		.index = __COUNTER__,
		.name = L"kraza",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#593c2b",
			"#775336",
			"#3b2620",
			"#ff6365",
			"#fffff7",
			"#81ce3a",
			"#b93a72",
		}},
	};

	inline const static Definition Llema = {
		.index = __COUNTER__,
		.name = L"llema",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#4eae8b",
			"#84c19c",
			"#3d9177",
			"#1c3443",
			"#f2a27f",
			"#ffefe1",
			"#162432",
		}},
	};

	inline const static Definition Lotus = {
		.index = __COUNTER__,
		.name = L"lotus",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#f02555",
			"#f25278",
			"#7c1b3d",
			"#f49f50",
			"#e46d33",
			"#f3e6e9",
			"#ab5e39",
		}},
	};

	inline const static Definition Loxxe = {
		.index = __COUNTER__,
		.name = L"loxxe",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#243966",
			"#31487a",
			"#16274d",
			"#122240",
			"#546e78",
			"#a4b2b6",
			"#0b162c",
		}},
	};

	inline const static Definition Meazs = {
		.index = __COUNTER__,
		.name = L"meazs",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#ea8241",
			"#f3ab58",
			"#9e3b1f",
			"#eab1e2",
			"#cd40dd",
			"#ffffff",
			"#b96ac2",
		}},
	};

	inline const static Definition Metis = {
		.index = __COUNTER__,
		.name = L"metis",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#50c169",
			"#72d387",
			"#1b703f",
			"#742a7d",
			"#f04f2a",
			"#ffffff",
			"#29063b",
		}},
	};

	inline const static Definition Moonflower = {
		.index = __COUNTER__,
		.name = L"moonflower",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#ea4a71",
			"#f664cc",
			"#b41d3d",
			"#ffffff",
			"#f664cc",
			"#ffffff",
			"#f9b70c",
		}},
	};

	inline const static Definition Nachi = {
		.index = __COUNTER__,
		.name = L"nachi",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#ff5454",
			"#d63838",
			"#00ffff",
			"#00ffff",
			"#ba0000",
			"#ffffff",
			"#00c9c9",
		}},
	};

	inline const static Definition Oom = {
		.index = __COUNTER__,
		.name = L"oom",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#060084",
			"#0000ff",
			"#04004d",
			"#ffffff",
			"#f40006",
			"#ffffff",
			"#0000ff",
		}},
	};

	inline const static Definition Peace = {
		.index = __COUNTER__,
		.name = L"peace",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#ddfdfe",
			"#ffffff",
			"#9bdbde",
			"#baf1f0",
			"#afdce9",
			"#ddfdfe",
			"#9bdbde",
		}},
	};

	inline const static Definition Power = {
		.index = __COUNTER__,
		.name = L"power",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#fea0f8",
			"#ffc3c8",
			"#fe0ab4",
			"#fe0ab4",
			"#ff00c5",
			"#ffc3c8",
			"#fea0f8",
		}},
	};

	inline const static Definition Purpleflower = {
		.index = __COUNTER__,
		.name = L"purpleflower",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#794b7c",
			"#a27cb8",
			"#461f3e",
			"#2c9ead",
			"#146a72",
			"#dbffff",
			"#146a72",
		}},
	};

	inline const static Definition Radiance = {
		.index = __COUNTER__,
		.name = L"radiance",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#372c24",
			"#6c665c",
			"#000000",
			"#b5a692",
			"#372c24",
			"#dac3b1",
			"#92897a",
		}},
	};

	inline const static Definition Redmoondesert = {
		.index = __COUNTER__,
		.name = L"redmoondesert",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#89280b",
			"#cd5329",
			"#3e1109",
			"#5e8bdc",
			"#cd5329",
			"#cedbff",
			"#15367a",
		}},
	};

	inline const static Definition Ripple = {
		.index = __COUNTER__,
		.name = L"ripple",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#0aa4f5",
			"#00bdf7",
			"#0057da",
			"#004da3",
			"#00bdf7",
			"#ccccff",
			"#00315f",
		}},
	};

	inline const static Definition Romal = {
		.index = __COUNTER__,
		.name = L"romal",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#5a3178",
			"#825197",
			"#3f1f56",
			"#acacac",
			"#785ae1",
			"#ffffff",
			"#655b6b",
		}},
	};

	inline const static Definition Sillh = {
		.index = __COUNTER__,
		.name = L"sillh",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#f8ddd1",
			"#ffeee7",
			"#bf927f",
			"#f36f59",
			"#d74e37",
			"#ffffff",
			"#c63f3b",
		}},
	};

	inline const static Definition Stonehenge = {
		.index = __COUNTER__,
		.name = L"stonehenge",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#627ec4",
			"#a6b5e2",
			"#43589a",
			"#434d35",
			"#948e85",
			"#eeeeff",
			"#182b11",
		}},
	};

	inline const static Definition Tulips = {
		.index = __COUNTER__,
		.name = L"tulips",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#d39e0e",
			"#b7c0e6",
			"#7b5505",
			"#272e12",
			"#516bdc",
			"#ffffee",
			"#1c2328",
		}},
	};

	inline const static Definition Vette = {
		.index = __COUNTER__,
		.name = L"vette",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#aab0cf",
			"#707fca",
			"#747ca8",
			"#7b7d89",
			"#798adb",
			"#d5dcff",
			"#4d5064",
		}},
	};

	inline const static Definition Vortecspace = {
		.index = __COUNTER__,
		.name = L"vortecspace",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#029cfe",
			"#0240de",
			"#0c1f88",
			"#029cfe",
			"#029cfe",
			"#0c142a",
			"#0c1f88",
		}},
	};

	inline const static Definition Wind = {
		.index = __COUNTER__,
		.name = L"wind",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#cb7d54",
			"#f38b41",
			"#6b4c30",
			"#ac8a9d",
			"#f38b41",
			"#ffeedd",
			"#5569ad",
		}},
	};

	inline const static Definition Windowsxp = {
		.index = __COUNTER__,
		.name = L"windowsxp",
		.group = PaletteGroup::NonCanon,
		.palette = {{
			"#679d4a",
			"#5ba631",
			"#446333",
			"#427725",
			"#7b96f9",
			"#ffeeff",
			"#446333",
		}},
	};

	inline const static Definition Yette = {
		.index = __COUNTER__,
		.name = L"yette",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#45b0ee",
			"#60c1f9",
			"#238cc9",
			"#c3c4e7",
			"#44a7d8",
			"#e8f9ff",
			"#8587c1",
		}},
	};

	inline const static Definition Zehal = {
		.index = __COUNTER__,
		.name = L"zehal",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#eab375",
			"#e8c685",
			"#ad7455",
			"#724731",
			"#f3a296",
			"#fbfbd7",
			"#4f2d1b",
		}},
	};

	inline const static Definition Zoog = {
		.index = __COUNTER__,
		.name = L"zoog",
		.group = PaletteGroup::Canon,
		.palette = {{
			"#9f2936",
			"#c74545",
			"#851a26",
			"#2a0808",
			"#a20814",
			"#000000",
			"#1c0407",
		}},
	};

	inline const static std::set<Definition> All = {
		Aemil,
		Dzune,
		Ellai,
		Evjar,
		Gimeljoy,
		Gimelsad,
		GimelYOO,
		Jaela,
		Jergh,
		Kirii,
		Kraza,
		Llema,
		Lotus,
		Loxxe,
		Meazs,
		Metis,
		Romal,
		Sillh,
		Vette,
		Zoog,
		Autumn,
		Ascent,
		Azul,
		Bliss,
		Chasnah,
		Crystal,
		Dejil,
		Follow,
		Friend,
		Fruit,
		Home,
		Moonflower,
		Nachi,
		Oom,
		Peace,
		Power,
		Purpleflower,
		Radiance,
		Redmoondesert,
		Ripple,
		Stonehenge,
		Tulips,
		Vortecspace,
		Wind,
		Windowsxp,
		Yette,
		Zehal,
	};
};

struct PaletteGroups {
private:
	inline static std::vector<const Palettes::Definition *> palettes_of_group(PaletteGroup group) {
		std::vector<const Palettes::Definition *> members;

		for (const auto &palette : Palettes::All) {
			if (group == PaletteGroup::All || palette.group == group) {
				members.push_back(&palette);
			}
		}

		return members;
	}
	
public:
	struct Definition {
		const std::wstring name;
		const PaletteGroup group;
		const std::vector<const Palettes::Definition *> members;
	};

	inline const static Definition Canon = {
		.name = L"Canon",
		.group = PaletteGroup::Canon,
		.members = palettes_of_group(PaletteGroup::Canon),
	};

	inline const static Definition NonCanon = {
		.name = L"NonCanon",
		.group = PaletteGroup::NonCanon,
		.members = palettes_of_group(PaletteGroup::NonCanon),
	};

	inline const static Definition All = {
		.name = L"All",
		.group = PaletteGroup::All,
		.members = palettes_of_group(PaletteGroup::All),
	};

	inline static Definition get(PaletteGroup group) {
		switch (group) {
			case PaletteGroup::Canon: return Canon;
			case PaletteGroup::NonCanon: return NonCanon;
			case PaletteGroup::All: return All;
		}
	}
};
