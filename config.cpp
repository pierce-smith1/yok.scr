#include <Windows.h>

#include <vector>
#include <utility>
#include <format>
#include <algorithm>

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

Registry::~Registry() {
	RegCloseKey(m_reg_key);
}

Config Registry::get_config() {
	Registry registry;
	Config config;

	const std::wstring is_registry_migrated_name = L"IsRegistryMigrated";
	bool is_registry_migrated = registry.get(is_registry_migrated_name, 0.0) != 0.0;

	for (const auto &opt : Cfg::All) {
		bool has_no_legacy = opt.legacy_id == 0;
		if (is_registry_migrated || has_no_legacy) {
			config[opt] = registry.get(opt.name, opt.default_);
		} else {
			config[opt] = registry.get(std::to_wstring(opt.legacy_id), opt.default_);
			registry.write(opt.name, config[opt]);
			registry.remove(std::to_wstring(opt.legacy_id));
		}
	}

	if (!is_registry_migrated) {
		registry.write(is_registry_migrated_name, 1.0);
	}

	return config;
}

double Registry::get(const std::wstring &opt, double default_) {
	auto value = get_string(opt, std::to_wstring(default_));
	return std::stof(value);
}

std::wstring Registry::get_string(const std::wstring &opt, const std::wstring &default_) {
	wchar_t result[1 << 12] {};
	DWORD result_type;
	DWORD result_size = 1 << 12;

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

	return result;
}

void Registry::write(const std::wstring &opt, double value) {
	write_string(opt, std::to_wstring(value));
}

void Registry::write_string(const std::wstring &opt, const std::wstring &value) {
	auto data_size = cast<DWORD>((value.size() + 1) * 2);

	RegSetValueEx(
		m_reg_key,
		opt.c_str(),
		0,
		REG_SZ,
		(BYTE *) value.c_str(),
		data_size
	);
}

void Registry::remove(const std::wstring &opt) {
	RegDeleteValue(
		m_reg_key,
		opt.c_str()
	);
}

RegistryBackedMap::RegistryBackedMap(const std::wstring &prefix)
	: m_prefix(prefix), m_index_key(std::format(L"_{}_index", prefix)) { }

std::wstring RegistryBackedMap::get(const std::wstring &key, const std::wstring &default_) {
	Registry registry;

	auto value = registry.get_string(prefix_key(key), default_);
	return value;
}

void RegistryBackedMap::set(const std::wstring &key, const std::wstring &value) {
	Registry registry;

	registry.write_string(prefix_key(key), value);
	ensure_in_index(registry, key);
}

void RegistryBackedMap::remove(const std::wstring &key) {
	Registry registry;

	registry.remove(prefix_key(key));
	remove_from_index(registry, key);
}

std::vector<std::pair<std::wstring, std::wstring>> RegistryBackedMap::items() {
	Registry registry;

	auto index_contents = registry.get_string(m_index_key, L"");
	auto keys = split<std::wstring>(index_contents, IndexDelimiter);

	std::vector<Item> items;
	std::transform(keys.begin(), keys.end(), std::back_inserter(items), [&](const std::wstring &key) -> Item {
		return { key, registry.get_string(prefix_key(key), L"") };
	});

	return items;
}

std::wstring RegistryBackedMap::prefix_key(const std::wstring &key) {
	auto prefixed_key = std::format(L"_{}:{}", m_prefix, key);
	return prefixed_key;
}

void RegistryBackedMap::ensure_in_index(Registry &registry, const std::wstring &key) {
	auto index_contents = registry.get_string(m_index_key, L"");
	auto keys = split<std::wstring>(index_contents, IndexDelimiter);

	auto index_key = std::find(keys.begin(), keys.end(), key);
	if (index_key != keys.end()) {
		return;
	}

	keys.push_back(key);
	auto new_index = join<std::wstring>(keys, IndexDelimiter);
	registry.write_string(m_index_key, new_index);
}

void RegistryBackedMap::remove_from_index(Registry &registry, const std::wstring &key) {
	auto index_contents = registry.get_string(m_index_key, L"");
	auto keys = split<std::wstring>(index_contents, IndexDelimiter);

	auto index_key = std::find(keys.begin(), keys.end(), key);
	if (index_key == keys.end()) {
		return;
	}

	keys.erase(index_key);
	auto new_index = join<std::wstring>(keys, IndexDelimiter);
	registry.write_string(m_index_key, new_index);
}
