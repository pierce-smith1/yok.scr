#include <Windows.h>

#include <vector>
#include <utility>

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
		L"Software\\doughbyte\\yokscr", 
		0, 
		NULL,
		REG_OPTION_NON_VOLATILE, 
		KEY_WRITE | KEY_READ, 
		NULL, 
		&m_reg_key, 
		NULL
	);
}

Config Registry::get_config() {
	std::vector<std::pair<ConfigOption, float>> keys;

	for (int i = _CONFIG_OPTIONS_START + 1; i < _CONFIG_OPTIONS_END; i++) {
		ConfigOption opt = (ConfigOption) i;
		keys.push_back(std::make_pair(opt, get(opt, cfg_defaults.at(opt))));
	}

	return Config(keys.begin(), keys.end());
}

float Registry::get(ConfigOption opt, float default_) {
	std::wstring option_name = std::to_wstring(opt);

	wchar_t result[1 << 6] {};
	DWORD result_type;
	DWORD result_size = 1 << 6;

	LSTATUS status = RegGetValue(
		m_reg_key,
		NULL,
		std::to_wstring(opt).c_str(),
		RRF_RT_REG_SZ,
		&result_type,
		result,
		&result_size
	);

	if (status == ERROR_FILE_NOT_FOUND) {
		return default_;
	}

	return std::stof(result);
}

void Registry::write(ConfigOption opt, float value) {
	RegSetValueEx(
		m_reg_key,
		std::to_wstring(opt).c_str(),
		0,
		REG_SZ,
		(BYTE *) std::to_wstring(value).c_str(),
		(std::to_wstring(value).size() + 1) * 2
	);
}