#include <Windows.h>

#include <vector>
#include <utility>

#include "config.h"
#include "common.h"

Config::Config() 
	: m_store(Cfg::All.size())
{
	for (const auto &def : Cfg::All) {
		(*this)[def] = def.default_;
	}
}

double &Config::operator[](const Cfg::Definition &opt) {
	return m_store[opt.index];
}

double Config::operator[](const Cfg::Definition &opt) const {
	return m_store[opt.index];
}

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
	Config config;

	const std::wstring is_registry_migrated_name = L"IsRegistryMigrated";
	bool is_registry_migrated = get(is_registry_migrated_name, 0.0) != 0.0;

	for (const auto &opt : Cfg::All) {
		if (is_registry_migrated) {
			config[opt] = get(opt.name, opt.default_);
		} else {
			config[opt] = get(std::to_wstring(opt.legacy_id), opt.default_);
			write(opt.name, config[opt]);
			remove(std::to_wstring(opt.legacy_id));
		}
	}

	if (!is_registry_migrated) {
		write(is_registry_migrated_name, 1.0);
	}

	return config;
}

double Registry::get(const std::wstring &opt, double default_) {
	wchar_t result[1 << 6] {};
	DWORD result_type;
	DWORD result_size = 1 << 6;

	LSTATUS status = RegGetValue(
		m_reg_key,
		NULL,
		opt.c_str(),
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

void Registry::write(const std::wstring &opt, double value) {
	auto string_value = std::to_wstring(value);
	auto data_size = cast<DWORD>((string_value.size() + 1) * 2);

	RegSetValueEx(
		m_reg_key,
		opt.c_str(),
		0,
		REG_SZ,
		(BYTE *) string_value.c_str(),
		data_size
	);
}

void Registry::remove(const std::wstring &opt) {
	RegDeleteValue(
		m_reg_key,
		opt.c_str()
	);
}