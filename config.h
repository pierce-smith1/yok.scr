#pragma once

#include <Windows.h>

#include <map>
#include <string>

enum ConfigOption {
	_CONFIG_OPTIONS_START = 0,
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
	_CONFIG_OPTIONS_SIZE
};

//using Config = std::map<ConfigOption, float>;
typedef float *Config;

class Registry {
public:
	Registry();

	static Config get_default_config();
	Config get_config();
	float get(ConfigOption opt, float default_);
	void write(ConfigOption opt, float value);

private:
	HKEY m_reg_key;
};


static Config cfg_defaults = Registry::get_default_config();
static Config cfg = Registry().get_config();