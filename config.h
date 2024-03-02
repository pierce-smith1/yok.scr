#pragma once

#include <Windows.h>

#include <map>
#include <string>
#include <optional>
#include <vector>
#include "resourcew.h"

struct ConfigOption {
	auto operator <=> (const ConfigOption &other) const = default;

	const std::wstring name;
	const int legacy_id;
	const float default_;
	const std::pair<float, float> range;
	const int dialog_control_id;
};

struct Opts {
	inline const static ConfigOption StepSize = { 
		.name = L"YonkStepSize", 
		.legacy_id = 1001,
		.default_ = 0.005f, 
	};

	inline const static ConfigOption HomeDrift = { 
		.name = L"YonkHomeDrift", 
		.legacy_id = 1002,
		.default_= 0.3f, 
		.range = { 0.0f, 5.0f },
		.dialog_control_id = IDC_YONK_HOME_DRIFT,
	};

	inline const static ConfigOption EmotionScale = { 
		.name = L"YonkEmotionScale", 
		.legacy_id = 1003,
		.default_= 5.0f, 
		.range = { 0.0f, 10.0f },
		.dialog_control_id = IDC_YONK_EMOTION_SCALE,
	};

	inline const static ConfigOption TimeDivisor = { 
		.name = L"TimeDivisor", 
		.legacy_id = 1004,
		.default_= 180.0f, 
		.range = { 10.0f, 300.0f },
		.dialog_control_id = IDC_TIME_DIVISOR,
	};

	inline const static ConfigOption MaxColors = { 
		.name = L"MaxColors", 
		.legacy_id = 1005,
		.default_= 5.0f, 
		.range = { 2.0f, 47.0f },
		.dialog_control_id = IDC_MAX_COLORS,
	};

	inline const static ConfigOption SpriteCount = {
		.name = L"SpriteCount",
		.legacy_id = 1006,
		.default_ = 80.0f,
		.range = { 1.0f, 200.0f },
		.dialog_control_id = IDC_SPRITE_COUNT,
	};

	inline const static ConfigOption SpriteSize = {
		.name = L"SpriteSize",
		.legacy_id = 1007,
		.default_ = 50.0f,
		.range = { 10.0f, 200.0f },
		.dialog_control_id = IDC_SPRITE_SIZE,
	};

	inline const static ConfigOption ShakeFactor = {
		.name = L"YonkShakeFactor",
		.legacy_id = 1008,
		.default_ = 2.0f,
		.range = { 0.0f, 5.0f },
		.dialog_control_id = IDC_YONK_SHAKE_FACTOR,
	};

	inline const static ConfigOption Pattern = {
		.name = L"YonkPattern",
		.legacy_id = 1009,
		.default_ = 0.0f,
		.dialog_control_id = IDC_YONK_PATTERN,
	};

	inline const static ConfigOption PatternChangeInterval = {
		.name = L"PatternChangeInterval",
		.legacy_id = 1010,
		.default_ = 60.0f * 15.0f,
		.range = { 30.0f, 60.0f * 30.0f },
		.dialog_control_id = IDC_PATTERN_CHANGE_INTERVAL,
	};

	inline const static ConfigOption IsPatternFixed = {
		.name = L"IsPatternFixed",
		.legacy_id = 1011,
		.default_ = 0.0f,
		.dialog_control_id = IDC_PATTERN_FIX,
	};

	inline const static ConfigOption ImpostorChance = {
		.name = L"ImpostorChance",
		.legacy_id = 1012,
		.default_ = powf(0.002f, 1.0f / 3.0f),
		.range = { 0.0f, 1.0f },
		.dialog_control_id = IDC_IMPOSTOR_CHANCE,
	};

	inline const static ConfigOption Palette = {
		.name = L"YonkPalette",
		.legacy_id = 1013,
		.default_ = 0.0f,
		.dialog_control_id = IDC_YONK_PALETTE,
	};

	inline const static ConfigOption PlayOverDesktop = {
		.name = L"PlayOverDesktop",
		.legacy_id = 1014,
		.default_ = 0.0f,
		.dialog_control_id = IDC_PLAY_OVER_DESKTOP,
	};

	inline const static std::vector<ConfigOption> All = {
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

using Config = std::map<ConfigOption, float>;

class Registry {
public:
	Registry();

	Config get_config();
	float get(const std::wstring &opt, float default_);
	void write(const std::wstring &opt, float value);
	void remove(const std::wstring &opt);

private:
	HKEY m_reg_key;
};

const static Config cfg = Registry().get_config();
