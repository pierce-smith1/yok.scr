#pragma once

#include <Windows.h>

#include <map>
#include <string>
#include <optional>
#include <vector>
#include "resourcew.h"
#include <set>


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
		.default_ = 0.005f, 
	};

	inline const static Definition HomeDrift = { 
		.index = __COUNTER__,
		.name = L"YonkHomeDrift", 
		.legacy_id = 1002,
		.default_= 0.3f, 
		.range = { 0.0f, 5.0f },
		.dialog_control_id = IDC_YONK_HOME_DRIFT,
	};

	inline const static Definition EmotionScale = { 
		.index = __COUNTER__,
		.name = L"YonkEmotionScale", 
		.legacy_id = 1003,
		.default_= 5.0f, 
		.range = { 0.0f, 10.0f },
		.dialog_control_id = IDC_YONK_EMOTION_SCALE,
	};

	inline const static Definition TimeDivisor = { 
		.index = __COUNTER__,
		.name = L"TimeDivisor", 
		.legacy_id = 1004,
		.default_= 180.0f, 
		.range = { 10.0f, 300.0f },
		.dialog_control_id = IDC_TIME_DIVISOR,
	};

	inline const static Definition MaxColors = { 
		.index = __COUNTER__,
		.name = L"MaxColors", 
		.legacy_id = 1005,
		.default_= 5.0f, 
		.range = { 2.0f, 47.0f },
		.dialog_control_id = IDC_MAX_COLORS,
	};

	inline const static Definition SpriteCount = {
		.index = __COUNTER__,
		.name = L"SpriteCount",
		.legacy_id = 1006,
		.default_ = 80.0f,
		.range = { 1.0f, 200.0f },
		.dialog_control_id = IDC_SPRITE_COUNT,
	};

	inline const static Definition SpriteSize = {
		.index = __COUNTER__,
		.name = L"SpriteSize",
		.legacy_id = 1007,
		.default_ = 50.0f,
		.range = { 10.0f, 200.0f },
		.dialog_control_id = IDC_SPRITE_SIZE,
	};

	inline const static Definition ShakeFactor = {
		.index = __COUNTER__,
		.name = L"YonkShakeFactor",
		.legacy_id = 1008,
		.default_ = 2.0f,
		.range = { 0.0f, 5.0f },
		.dialog_control_id = IDC_YONK_SHAKE_FACTOR,
	};

	inline const static Definition Pattern = {
		.index = __COUNTER__,
		.name = L"YonkPattern",
		.legacy_id = 1009,
		.default_ = 0.0f,
		.dialog_control_id = IDC_YONK_PATTERN,
	};

	inline const static Definition PatternChangeInterval = {
		.index = __COUNTER__,
		.name = L"PatternChangeInterval",
		.legacy_id = 1010,
		.default_ = 60.0f * 15.0f,
		.range = { 30.0f, 60.0f * 30.0f },
		.dialog_control_id = IDC_PATTERN_CHANGE_INTERVAL,
	};

	inline const static Definition IsPatternFixed = {
		.index = __COUNTER__,
		.name = L"IsPatternFixed",
		.legacy_id = 1011,
		.default_ = 0.0f,
		.dialog_control_id = IDC_PATTERN_FIX,
	};

	inline const static Definition ImpostorChance = {
		.index = __COUNTER__,
		.name = L"ImpostorChance",
		.legacy_id = 1012,
		.default_ = powf(0.002f, 1.0f / 3.0f),
		.range = { 0.0f, 1.0f },
		.dialog_control_id = IDC_IMPOSTOR_CHANCE,
	};

	inline const static Definition Palette = {
		.index = __COUNTER__,
		.name = L"YonkPalette",
		.legacy_id = 1013,
		.default_ = 0.0f,
		.dialog_control_id = IDC_YONK_PALETTE,
	};

	inline const static Definition PlayOverDesktop = {
		.index = __COUNTER__,
		.name = L"PlayOverDesktop",
		.legacy_id = 1014,
		.default_ = 0.0f,
		.dialog_control_id = IDC_PLAY_OVER_DESKTOP,
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
		IsPatternFixed,
		ImpostorChance,
		Palette,
		PlayOverDesktop,
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

	Config get_config();
	double get(const std::wstring &opt, double default_);
	void write(const std::wstring &opt, double value);
	void remove(const std::wstring &opt);

private:
	HKEY m_reg_key;
};

const static Config cfg = Registry().get_config();
