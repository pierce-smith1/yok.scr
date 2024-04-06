#pragma once

#include <array>
#include <set>
#include <initializer_list>
#include <vector>
#include <variant>
#include <string>
#include <stdexcept>


#include "common.h"
#include "config.h"

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
	PaletteData(const std::vector<std::wstring> &hex_strings);
};

enum class PaletteGroup {
	All,
	Canon,
	NonCanon,
	RandomlyGenerated,
	Custom,
	_PALETTE_OPTION_COUNT
};

struct Palettes {
	struct Definition {
		auto operator<=>(const Definition &other) const = default;

		std::wstring name;
		PaletteGroup group;
		PaletteData *data;
	};

	inline const static Definition Aemil = {
		.name = L"aemil",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#56eb8e",
			L"#84f5c3",
			L"#1d9550",
			L"#e29a56",
			L"#e16a72",
			L"#dff9eb",
			L"#966336",
		}},
	};

	inline const static Definition Autumn = {
		.name = L"autumn",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#db8313",
			L"#be9275",
			L"#904a08",
			L"#574d3c",
			L"#904a08",
			L"#f3e6e9",
			L"#000000",
		}},
	};

	inline const static Definition Ascent = {
		.name = L"ascent",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#1963c4",
			L"#1963c4",
			L"#0b407d",
			L"#8c96dd",
			L"#8c96dd",
			L"#ffffff",
			L"#506bbc",
		}},
	};

	inline const static Definition Azul = {
		.name = L"azul",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#1aadd9",
			L"#a1d0e5",
			L"#296d9e",
			L"#4d8db9",
			L"#296d9e",
			L"#ffffff",
			L"#103050",
		}},
	};

	inline const static Definition Bliss = {
		.name = L"bliss",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#73981e",
			L"#73981e",
			L"#3d5317",
			L"#6a96f2",
			L"#282438",
			L"#eaf2ff",
			L"#3b73ee",
		}},
	};

	inline const static Definition Chasnah = {
		.name = L"chasnah",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#6f31dd",
			L"#932de3",
			L"#3a12a2",
			L"#e30efe",
			L"#e227ff",
			L"#f6f5f4",
			L"#e959f5",
		}},
	};

	inline const static Definition Crystal = {
		.name = L"crystal",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#38399e",
			L"#51b7cf",
			L"#1c63d8",
			L"#434ad9",
			L"#0a1a4a",
			L"#eaf2ff",
			L"#0a1a4a",
		}},
	};

	inline const static Definition Dejil = {
		.name = L"dejil",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#f0f0f0",
			L"#ffffff",
			L"#bebebe",
			L"#c6bfa0",
			L"#f0f0f0",
			L"#f0f0f0",
			L"#ab925e",
		}},
	};

	inline const static Definition Dzune = {
		.name = L"dzune",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#e56161",
			L"#c75653",
			L"#ab3232",
			L"#4b1111",
			L"#e38663",
			L"#faecec",
			L"#370808",
		}},
	};

	inline const static Definition Ellai = {
		.name = L"ellai",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#97cc72",
			L"#caecb1",
			L"#338527",
			L"#72482d",
			L"#f04f2a",
			L"#ffffff",
			L"#45160e",
		}},
	};

	inline const static Definition Evjar = {
		.name = L"evjar",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#6c2717",
			L"#8b4231",
			L"#2f1c31",
			L"#220e32",
			L"#f04f2a",
			L"#ffffff",
			L"#0e0019",
		}},
	};

	inline const static Definition Follow = {
		.name = L"follow",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#de5a29",
			L"#d0742d",
			L"#0a1a4a",
			L"#c0982b",
			L"#0a1a4a",
			L"#eaf2ff",
			L"#9f894e",
		}},
	};

	inline const static Definition Friend = {
		.name = L"friend",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#a08d77",
			L"#bcab96",
			L"#433a2d",
			L"#433a2d",
			L"#191713",
			L"#e3ddd5",
			L"#191713",
		}},
	};

	inline const static Definition Fruit = {
		.name = L"fruit",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#f05f7c",
			L"#ed7890",
			L"#d3405e",
			L"#4da796",
			L"#f7e570",
			L"#dead5b",
			L"#ebde6e",
		}},
	};

	inline const static Definition Gimeljoy = {
		.name = L"gimeljoy",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#ffdec4",
			L"#fac499",
			L"#ffb376",
			L"#e7b085",
			L"#000000",
			L"#ffa154",
			L"#ce9161",
		}},
	};

	inline const static Definition Gimelsad = {
		.name = L"gimelsad",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#c4e7ff",
			L"#81c0ec",
			L"#5fa7da",
			L"#63a0cb",
			L"#000000",
			L"#37abfc",
			L"#3d80af",
		}},
	};

	inline const static Definition GimelYOO = {
		.name = L"gimelYOO",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#f9c4ff",
			L"#f492ff",
			L"#f06cff",
			L"#dd84e7",
			L"#000000",
			L"#ed44ff",
			L"#c767d1",
		}},
	};

	inline const static Definition Home = {
		.name = L"home",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#a26177",
			L"#7d5a7c",
			L"#593b42",
			L"#7e3e4c",
			L"#4149c6",
			L"#ffe1e7",
			L"#593b42",
		}},
	};

	inline const static Definition Jaela = {
		.name = L"jaela",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#f8b765",
			L"#f6d58e",
			L"#a95a2e",
			L"#f6585b",
			L"#e85533",
			L"#ffffff",
			L"#b5282b",
		}},
	};

	inline const static Definition Jergh = {
		.name = L"jergh",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#e7e7e7",
			L"#f3f3eb",
			L"#b4bcd1",
			L"#79c569",
			L"#d08ab7",
			L"#f0f0f0",
			L"#cf90b8",
		}},
	};

	inline const static Definition Kirii = {
		.name = L"kirii",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#4ad4bf",
			L"#44e0d1",
			L"#2c9197",
			L"#77807f",
			L"#6483bc",
			L"#d8fff9",
			L"#525353",
		}},
	};

	inline const static Definition Kraza = {
		.name = L"kraza",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#593c2b",
			L"#775336",
			L"#3b2620",
			L"#ff6365",
			L"#fffff7",
			L"#81ce3a",
			L"#b93a72",
		}},
	};

	inline const static Definition Llema = {
		.name = L"llema",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#4eae8b",
			L"#84c19c",
			L"#3d9177",
			L"#1c3443",
			L"#f2a27f",
			L"#ffefe1",
			L"#162432",
		}},
	};

	inline const static Definition Lotus = {
		.name = L"lotus",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#f02555",
			L"#f25278",
			L"#7c1b3d",
			L"#f49f50",
			L"#e46d33",
			L"#f3e6e9",
			L"#ab5e39",
		}},
	};

	inline const static Definition Loxxe = {
		.name = L"loxxe",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#243966",
			L"#31487a",
			L"#16274d",
			L"#122240",
			L"#546e78",
			L"#a4b2b6",
			L"#0b162c",
		}},
	};

	inline const static Definition Meazs = {
		.name = L"meazs",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#ea8241",
			L"#f3ab58",
			L"#9e3b1f",
			L"#eab1e2",
			L"#cd40dd",
			L"#ffffff",
			L"#b96ac2",
		}},
	};

	inline const static Definition Metis = {
		.name = L"metis",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#50c169",
			L"#72d387",
			L"#1b703f",
			L"#742a7d",
			L"#f04f2a",
			L"#ffffff",
			L"#29063b",
		}},
	};

	inline const static Definition Moonflower = {
		.name = L"moonflower",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#ea4a71",
			L"#f664cc",
			L"#b41d3d",
			L"#ffffff",
			L"#f664cc",
			L"#ffffff",
			L"#f9b70c",
		}},
	};

	inline const static Definition Nachi = {
		.name = L"nachi",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#ff5454",
			L"#d63838",
			L"#00ffff",
			L"#00ffff",
			L"#ba0000",
			L"#ffffff",
			L"#00c9c9",
		}},
	};

	inline const static Definition Oom = {
		.name = L"oom",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#060084",
			L"#0000ff",
			L"#04004d",
			L"#ffffff",
			L"#f40006",
			L"#ffffff",
			L"#0000ff",
		}},
	};

	inline const static Definition Peace = {
		.name = L"peace",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#ddfdfe",
			L"#ffffff",
			L"#9bdbde",
			L"#baf1f0",
			L"#afdce9",
			L"#ddfdfe",
			L"#9bdbde",
		}},
	};

	inline const static Definition Power = {
		.name = L"power",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#fea0f8",
			L"#ffc3c8",
			L"#fe0ab4",
			L"#fe0ab4",
			L"#ff00c5",
			L"#ffc3c8",
			L"#fea0f8",
		}},
	};

	inline const static Definition Purpleflower = {
		.name = L"purpleflower",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#794b7c",
			L"#a27cb8",
			L"#461f3e",
			L"#2c9ead",
			L"#146a72",
			L"#dbffff",
			L"#146a72",
		}},
	};

	inline const static Definition Radiance = {
		.name = L"radiance",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#372c24",
			L"#6c665c",
			L"#000000",
			L"#b5a692",
			L"#372c24",
			L"#dac3b1",
			L"#92897a",
		}},
	};

	inline const static Definition Redmoondesert = {
		.name = L"redmoondesert",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#89280b",
			L"#cd5329",
			L"#3e1109",
			L"#5e8bdc",
			L"#cd5329",
			L"#cedbff",
			L"#15367a",
		}},
	};

	inline const static Definition Ripple = {
		.name = L"ripple",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#0aa4f5",
			L"#00bdf7",
			L"#0057da",
			L"#004da3",
			L"#00bdf7",
			L"#ccccff",
			L"#00315f",
		}},
	};

	inline const static Definition Romal = {
		.name = L"romal",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#5a3178",
			L"#825197",
			L"#3f1f56",
			L"#acacac",
			L"#785ae1",
			L"#ffffff",
			L"#655b6b",
		}},
	};

	inline const static Definition Sillh = {
		.name = L"sillh",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#f8ddd1",
			L"#ffeee7",
			L"#bf927f",
			L"#f36f59",
			L"#d74e37",
			L"#ffffff",
			L"#c63f3b",
		}},
	};

	inline const static Definition Stonehenge = {
		.name = L"stonehenge",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#627ec4",
			L"#a6b5e2",
			L"#43589a",
			L"#434d35",
			L"#948e85",
			L"#eeeeff",
			L"#182b11",
		}},
	};

	inline const static Definition Tulips = {
		.name = L"tulips",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#d39e0e",
			L"#b7c0e6",
			L"#7b5505",
			L"#272e12",
			L"#516bdc",
			L"#ffffee",
			L"#1c2328",
		}},
	};

	inline const static Definition Vette = {
		.name = L"vette",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#aab0cf",
			L"#707fca",
			L"#747ca8",
			L"#7b7d89",
			L"#798adb",
			L"#d5dcff",
			L"#4d5064",
		}},
	};

	inline const static Definition Vortecspace = {
		.name = L"vortecspace",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#029cfe",
			L"#0240de",
			L"#0c1f88",
			L"#029cfe",
			L"#029cfe",
			L"#0c142a",
			L"#0c1f88",
		}},
	};

	inline const static Definition Wind = {
		.name = L"wind",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#cb7d54",
			L"#f38b41",
			L"#6b4c30",
			L"#ac8a9d",
			L"#f38b41",
			L"#ffeedd",
			L"#5569ad",
		}},
	};

	inline const static Definition Windowsxp = {
		.name = L"windowsxp",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
			L"#679d4a",
			L"#5ba631",
			L"#446333",
			L"#427725",
			L"#7b96f9",
			L"#ffeeff",
			L"#446333",
		}},
	};

	inline const static Definition Yette = {
		.name = L"yette",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#45b0ee",
			L"#60c1f9",
			L"#238cc9",
			L"#c3c4e7",
			L"#44a7d8",
			L"#e8f9ff",
			L"#8587c1",
		}},
	};

	inline const static Definition Zehal = {
		.name = L"zehal",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#eab375",
			L"#e8c685",
			L"#ad7455",
			L"#724731",
			L"#f3a296",
			L"#fbfbd7",
			L"#4f2d1b",
		}},
	};

	inline const static Definition Zoog = {
		.name = L"zoog",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
			L"#9f2936",
			L"#c74545",
			L"#851a26",
			L"#2a0808",
			L"#a20814",
			L"#000000",
			L"#1c0407",
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

class RandomPalettes {
public:
	static Palettes::Definition random(int rng_token);

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
	static Color noisify(const Color &color, double degree = 1.0);
	static Color recolorize(const Color &color, double red_weight, double green_weight, double blue_weight);
	static std::set<GenerationTraits> random_traits();
};

struct PaletteGroups {
private:
	static std::vector<Palettes::Definition> palettes_of_group(PaletteGroup group);
	
public:
	struct Definition {
		std::wstring name;
		PaletteGroup group;
		std::vector<Palettes::Definition> members;
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
			default: throw std::domain_error("PaletteGroup " + std::to_string((int) group) + " is not valid.");
		}
	}
};

// This is 100% registry-backed storage. Do not access it in hot paths.
class PaletteRepository {
public:
	PaletteRepository();

	void set_palette(const std::wstring &name, const PaletteData &data);
	void remove_palette(const std::wstring &name);
	std::optional<Palettes::Definition> get_palette(const std::wstring &name);
	std::vector<Palettes::Definition> get_all_custom_palettes();

	std::wstring serialize(const PaletteData &palette);
	PaletteData deserialize(const std::wstring &serialized);

private:
	RegistryBackedMap m_map;
};

class PaletteGroupRepository {
public:
	PaletteGroupRepository();

	class Group {
	public:
		Group(const std::wstring &key, const RegistryBackedMap &repo_map);

		void add_palette(const std::wstring &name);
		void remove_palette(const std::wstring &name);
		std::vector<std::wstring> get_all_palettes();

		std::wstring name();
		
	private:
		const static inline std::wstring ListDelimiter = L",";

		std::wstring m_key;
		RegistryBackedMap m_repo_map;
	};

	Group get_group(const std::wstring &name);
	std::optional<size_t> get_group_index(const std::wstring &group_name);
	void remove_group(const std::wstring &name);
	std::vector<Group> get_all_groups();

private:
	RegistryBackedMap m_map;
};

