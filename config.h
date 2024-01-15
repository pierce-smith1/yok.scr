#pragma once

#include <Windows.h>

#include <map>
#include <string>

enum ConfigOption {
	_CONFIG_OPTIONS_START = 1000,
	YonkStepSize,
	YonkHomeDrift,
	YonkEmotionScale,
	TimeDivisor,
	MaxColors,
	SpriteCount,
	SpriteSize,
	YonkShakeFactor,
	YonkPattern,
	PatternChangeInterval,
	IsPatternFixed,
	ImpostorChance,
	YonkPalette,
	PlayOverDesktop,
	_CONFIG_OPTIONS_END
};

const static std::map<int, std::wstring> config_names = {
	{ YonkStepSize, L"YonkStepSize" },
	{ YonkHomeDrift, L"YonkHomeDrift" },
	{ YonkEmotionScale, L"YonkEmotionScale" },
	{ TimeDivisor, L"TimeDivisor" },
	{ MaxColors, L"MaxColors" },
	{ SpriteCount, L"SpriteCount" },
	{ SpriteSize, L"SpriteSize" },
	{ YonkShakeFactor, L"YonkShakeFactor" },
	{ YonkPattern, L"YonkPattern" },
	{ PatternChangeInterval, L"PatternChangeInterval" },
	{ IsPatternFixed, L"IsPatternFixed" },
	{ ImpostorChance, L"ImpostorChance" },
	{ YonkPalette, L"YonkPalette" },
	{ PlayOverDesktop, L"PlayOverDesktop" },
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

const static Config cfg_defaults = {
	{ YonkStepSize, 0.005f },
	{ YonkHomeDrift, 0.3f },
	{ YonkEmotionScale, 5.0f },
	{ TimeDivisor, 180.0f },
	{ MaxColors, 5.0f },
	{ SpriteCount, 80.0f },
	{ SpriteSize, 50.0f },
	{ YonkShakeFactor, 2.0f },
	{ YonkPattern, 0.0f },
	{ PatternChangeInterval, 60.0f * 15.0f },
	{ IsPatternFixed, 0.0f },
	{ ImpostorChance, (float)pow(0.002f, 1.0f/3.0f) },	// i'm sorry
	{ YonkPalette, 0.0f },
	{ PlayOverDesktop, 0.0f },
};

const static Config cfg = Registry().get_config();
