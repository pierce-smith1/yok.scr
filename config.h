#pragma once

#include <Windows.h>

#include <map>
#include <string>
#include <optional>
#include <vector>
#include <set>

#include "resourcew.h"

struct Cfg {
	struct Definition {
		auto operator<=>(const Definition &other) const = default;

		const size_t index;
		const std::wstring name;
		const int legacy_id;
		const double default_;
		const std::pair<double, double> range;
		const int dialog_control_id;
	};

	inline const static Definition StepSize = {
		.index = __COUNTER__,
		.name = L"YonkStepSize",
		.legacy_id = 1001,
		.default_ = 0.005,
	};

	inline const static Definition HomeDrift = {
		.index = __COUNTER__,
		.name = L"YonkHomeDrift",
		.legacy_id = 1002,
		.default_= 0.1,
		.range = { 0.0, 10.0 },
		.dialog_control_id = IDC_YONK_HOME_DRIFT,
	};

	inline const static Definition EmotionScale = {
		.index = __COUNTER__,
		.name = L"YonkEmotionScale",
		.legacy_id = 1003,
		.default_= 5.0,
		.range = { 0.0, 10.0 },
		.dialog_control_id = IDC_YONK_EMOTION_SCALE,
	};

	inline const static Definition TimeDivisor = {
		.index = __COUNTER__,
		.name = L"TimeDivisor",
		.legacy_id = 1004,
		.default_= 180.0,
		.range = { 10.0, 300.0 },
		.dialog_control_id = IDC_TIME_DIVISOR,
	};

	inline const static Definition MaxColors = {
		.index = __COUNTER__,
		.name = L"MaxColors",
		.legacy_id = 1005,
		.default_= 5.0,
		.range = { 2.0, 47.0 },
		.dialog_control_id = IDC_MAX_COLORS,
	};

	inline const static Definition SpriteCount = {
		.index = __COUNTER__,
		.name = L"SpriteCount",
		.legacy_id = 1006,
		.default_ = 80.0,
		.range = { 1.0, 200.0 },
		.dialog_control_id = IDC_SPRITE_COUNT,
	};

	inline const static Definition SpriteSize = {
		.index = __COUNTER__,
		.name = L"SpriteSize",
		.legacy_id = 1007,
		.default_ = 50.0,
		.range = { 10.0, 200.0 },
		.dialog_control_id = IDC_SPRITE_SIZE,
	};

	inline const static Definition ShakeFactor = {
		.index = __COUNTER__,
		.name = L"YonkShakeFactor",
		.legacy_id = 1008,
		.default_ = 2.0,
		.range = { 0.0, 5.0 },
		.dialog_control_id = IDC_YONK_SHAKE_FACTOR,
	};

	inline const static Definition Pattern = {
		.index = __COUNTER__,
		.name = L"YonkPattern",
		.legacy_id = 1009,
		.default_ = 0.0,
		.dialog_control_id = IDC_YONK_PATTERN,
	};

	inline const static Definition PatternChangeInterval = {
		.index = __COUNTER__,
		.name = L"PatternChangeInterval",
		.legacy_id = 1010,
		.default_ = 60.0 * 15.0,
		.range = { 30.0, 60.0 * 30.0 },
		.dialog_control_id = IDC_PATTERN_CHANGE_INTERVAL,
	};

	inline const static Definition ImpostorChance = {
		.index = __COUNTER__,
		.name = L"ImpostorChance",
		.legacy_id = 1012,
		.default_ = pow(0.002, 1.0 / 3.0),
		.range = { 0.0, 1.0 },
		.dialog_control_id = IDC_IMPOSTOR_CHANCE,
	};

	inline const static Definition Palette = {
		.index = __COUNTER__,
		.name = L"YonkPalette",
		.legacy_id = 1013,
		.default_ = 0.0,
		.dialog_control_id = IDC_YONK_PALETTE,
	};

	inline const static Definition PlayOverDesktop = {
		.index = __COUNTER__,
		.name = L"PlayOverDesktop",
		.legacy_id = 1014,
		.default_ = 0.0,
		.dialog_control_id = IDC_PLAY_OVER_DESKTOP,
	};

	inline const static Definition UseCustomPalettes = {
		.index = __COUNTER__,
		.name = L"UseCustomPalettes",
		.default_ = 0.0f,
		.dialog_control_id = IDC_CUSTOM_PALETTES_CHECK,
	};

	inline const static Definition TrailLength = {
		.index = __COUNTER__,
		.name = L"TrailLength",
		.default_ = 3.0,
		.range = { 1.0, 25.0 },
		.dialog_control_id = IDC_TRAIL_LENGTH,
	};

	inline const static Definition TrailSpace = {
		.index = __COUNTER__,
		.name = L"TrailSpace",
		.default_ = 10.0,
		.range = { 1.0, 50.0 },
		.dialog_control_id = IDC_TRAIL_SPACE,
	};

	// aka: kindly, calm the fuck down mate
	inline const static Definition MaxTrailCount = {
		.index = __COUNTER__,
		.name = L"MaxTrailCount",
		.default_ = 3000.0,
	};

	inline const static Definition TrailsEnabled = {
		.index = __COUNTER__,
		.name = L"TrailsEnabled",
		.default_ = 0.0,
		.dialog_control_id = IDC_TRAILS_ENABLED,
	};

	inline const static std::set<Definition> All = {
		StepSize,
		HomeDrift,
		EmotionScale,
		TimeDivisor,
		MaxColors,
		SpriteCount,
		SpriteSize,
		ShakeFactor,
		Pattern,
		PatternChangeInterval,
		ImpostorChance,
		Palette,
		PlayOverDesktop,
		UseCustomPalettes,
		TrailLength,
		TrailSpace,
		MaxTrailCount,
		TrailsEnabled,
	};
};

class Config {
public:
	Config();

	double &operator[](const Cfg::Definition &opt);
	double operator[](const Cfg::Definition &opt) const;

private:
	std::vector<double> m_store;
};

class Registry {
public:
	Registry();
	~Registry();

	Registry(const Registry &other) = delete;
	Registry &operator=(const Registry &other) = delete;

	static Config get_config();

	double get(const std::wstring &opt, double default_);
	std::wstring get_string(const std::wstring &opt, const std::wstring &default_);

	void write(const std::wstring &opt, double value);
	void write_string(const std::wstring &opt, const std::wstring &value);

	void remove(const std::wstring &opt);

private:
	HKEY m_reg_key;
};

class RegistryBackedMap {
public:
	using Item = std::pair<std::wstring, std::wstring>;

	RegistryBackedMap(const std::wstring &prefix);

	std::wstring get(const std::wstring &key, const std::wstring &default_);
	void set(const std::wstring &key, const std::wstring &value);
	void remove(const std::wstring &key);
	std::vector<Item> items();

	std::wstring prefix_key(const std::wstring &key);

private:
	const static inline std::wstring IndexDelimiter = L",";

	std::wstring m_prefix;
	std::wstring m_index_key;

	void ensure_in_index(Registry &registry, const std::wstring &key);
	void remove_from_index(Registry &registry, const std::wstring &key);
};

const static Config cfg = Registry::get_config();
