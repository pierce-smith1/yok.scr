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
	const Id m_id;
};

using Color = std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>;
enum Channel {
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	ALPHA = 3
};
