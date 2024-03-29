#pragma once

using Id = unsigned int;
static Id running_id = 0;

class Empty { };
template <typename Base> class Identifiable : public Base {
public:
	Identifiable() : m_id(running_id++) { }

	Id id() const {
		return m_id;
	}

private:
	Id m_id;
};

using Color = std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>;
enum Channel {
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	ALPHA = 3
};


template <typename String> std::vector<String> split(const String &string, const String &delimiter) {
	if (string.empty()) {
		return {};
	}

	std::vector<String> parts;

	size_t prev_index = 0;
	size_t next_index = String::npos;
	while ((next_index = string.find(delimiter, prev_index)) != String::npos) {
		parts.push_back(string.substr(prev_index, next_index - prev_index));
		prev_index = next_index + 1;
	}

	parts.push_back(string.substr(prev_index, next_index - prev_index));

	return parts;
}

template <typename String, typename Container> String join(const Container &strings, const String &delimiter) {
	String joined;

	for (const auto &string : strings) {
		joined += string + delimiter;
	}

	return joined.substr(0, joined.size() - delimiter.size());
}
  
template <typename To, typename From> To cast(From value) {
	return static_cast<To>(value);
}
