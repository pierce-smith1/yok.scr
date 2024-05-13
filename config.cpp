#include <Windows.h>
#include <winreg.h>

#include <vector>
#include <utility>
#include <stdio.h>

#include "config.h"

Registry::Registry() 
	: m_reg_key(NULL)
{
	// Faith is a strength of all engineers:
	// My heart may cry but my eyes have no tears.
	// "It'll work, I promise, 
	//  neglect all of your fears."
	RegCreateKeyEx(
		HKEY_CURRENT_USER, 
		"Software\\doughbyte\\yokscr", 
		0, 
		NULL,
		REG_OPTION_NON_VOLATILE, 
		KEY_WRITE | KEY_READ, 
		NULL, 
		&m_reg_key, 
		NULL
	);
}

Config Registry::get_default_config() {
	Config cfg = new float[_CONFIG_OPTIONS_SIZE];
	
	cfg[YonkStepSize] = 0.005f;
	cfg[YonkHomeDrift] = 0.3f;
	cfg[YonkEmotionScale] = 5.0f;
	cfg[TimeDivisor] = 100.0f;
	cfg[MaxColors] = 5.0f;
	cfg[SpriteCount] = 22.0f;
	cfg[SpriteSize] = 70.0f;
	cfg[YonkShakeFactor] = 2.0f;
	cfg[YonkPattern] = 0.0f;
	cfg[PatternChangeInterval] = 60.0f * 7.0f;

	return cfg;
}

Config Registry::get_config() {
	Config cfg = new float[_CONFIG_OPTIONS_SIZE];

	for (int i = _CONFIG_OPTIONS_START + 1; i < _CONFIG_OPTIONS_SIZE; i++) {
		ConfigOption opt = (ConfigOption) i;
		float default_ = cfg_defaults[opt];
		cfg[opt] = get(opt, default_);
	}

	return cfg;
}

float Registry::get(ConfigOption opt, float default_) {
	char option_name[1 << 10] = {0};
	sprintf(option_name, "%d", opt);

	unsigned char result[1 << 10] = {0};
	DWORD result_type = REG_SZ;
	DWORD result_size = 1 << 10;

	LONG status = RegQueryValueEx(
		m_reg_key,
		option_name,
		NULL,
		&result_type,
		result,
		&result_size
	);

	if (status == ERROR_FILE_NOT_FOUND) {
		return default_;
	}

	return atof((const char *) result);
}

void Registry::write(ConfigOption opt, float value) {
	char value_name[1 << 10] = {0};
	sprintf(value_name, "%f", value);

	size_t value_size = strlen(value_name) + 1;

	char option_name[1 << 10] = {0};
	sprintf(option_name, "%d", opt);

	RegSetValueEx(
		m_reg_key,
		option_name,
		0,
		REG_SZ,
		(const unsigned char *) value_name,
		value_size
	);
}