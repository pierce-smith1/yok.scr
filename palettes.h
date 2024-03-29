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
	PaletteData(const std::array<std::string, _PALETTE_SIZE - 1> &hex_strings);
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
		.name = L"autumn",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"ascent",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"azul",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"bliss",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"chasnah",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"crystal",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"dejil",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"dzune",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"ellai",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"evjar",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"follow",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"friend",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"fruit",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"gimeljoy",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"gimelsad",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"gimelYOO",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"home",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"jaela",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"jergh",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"kirii",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"kraza",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"llema",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"lotus",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"loxxe",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"meazs",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"metis",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"moonflower",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"nachi",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"oom",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"peace",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"power",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"purpleflower",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"radiance",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"redmoondesert",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"ripple",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"romal",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"sillh",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"stonehenge",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"tulips",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"vette",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"vortecspace",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"wind",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"windowsxp",
		.group = PaletteGroup::NonCanon,
		.data = new PaletteData {{
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
		.name = L"yette",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"zehal",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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
		.name = L"zoog",
		.group = PaletteGroup::Canon,
		.data = new PaletteData {{
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

private:
	PaletteData deserialize(const std::wstring &serialized);

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

