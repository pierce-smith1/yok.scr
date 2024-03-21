#include <Windows.h>

#include <vector>
#include <utility>
#include <shlobj_core.h>

#include "config.h"

Config::Config() 
	: m_store(Cfg::All.size())
{
	for (const auto &def : Cfg::All) {
		(*this)[def] = def.default_;
	}
}

float &Config::operator[](const Cfg::Definition &opt) {
	return m_store[opt.index];
}

float Config::operator[](const Cfg::Definition &opt) const {
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
	bool is_registry_migrated = get(is_registry_migrated_name, 0.0f) != 0.0f;

	for (const auto &opt : Cfg::All) {
		bool has_no_legacy = opt.legacy_id == 0;
		if (is_registry_migrated || has_no_legacy) {
			config[opt] = get(opt.name, opt.default_);
		} else {
			config[opt] = get(std::to_wstring(opt.legacy_id), opt.default_);
			write(opt.name, config[opt]);
			remove(std::to_wstring(opt.legacy_id));
		}
	}

	if (!is_registry_migrated) {
		write(is_registry_migrated_name, 1.0f);
	}

	return config;
}

float Registry::get(const std::wstring &opt, float default_) {
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

void Registry::write(const std::wstring &opt, float value) {
	write_string(opt, std::to_wstring(value));
}

void Registry::write_string(const std::wstring &opt, const std::wstring &value) {
	RegSetValueEx(
		m_reg_key,
		opt.c_str(),
		0,
		REG_SZ,
		(BYTE *) value.c_str(),
		(value.size() + 1) * 2
	);
}

void Registry::remove(const std::wstring &opt) {
	RegDeleteValue(
		m_reg_key,
		opt.c_str()
	);
}

DataStore::DataStore() {
	wchar_t *raw_appdata_path = nullptr;
	auto code = SHGetKnownFolderPath(
		FOLDERID_RoamingAppData, 
		KF_FLAG_CREATE, 
		NULL, 
		&raw_appdata_path
	);
	
	m_data_folder_path = raw_appdata_path;
	m_data_folder_path += L"\\yokscr";

	CreateDirectory(m_data_folder_path.c_str(), NULL);
}

ConfigStore DataStore::get_store() {
	ConfigStore store;

	for (const auto &key : Storage::All) {
		store[key.name] = read(key.name);
	}

	return store;
}

std::wstring DataStore::read(const std::wstring &filename) {
	HANDLE file = CreateFile(
		(m_data_folder_path + L"\\" + filename).c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	// Yes I went there, no I no longer care.
	// The brave may dare to live on a prayer.
	const size_t buffer_size = 1 << 20;
	wchar_t *buffer = new wchar_t[buffer_size];
	DWORD bytes_read;
	bool read_file_ok = ReadFile(file, buffer, buffer_size, &bytes_read, NULL);

	buffer[(bytes_read / 2)] = L'\0';
	std::wstring data = buffer;

	CloseHandle(file);
	delete[] buffer;

	return data;
}

void DataStore::write(const std::wstring &filename, const std::wstring &contents) {
	HANDLE file = CreateFile(
		(m_data_folder_path + L"\\" + filename).c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	WriteFile(
		file,
		contents.c_str(),
		contents.size() * 2,
		NULL,
		NULL
	);
}
