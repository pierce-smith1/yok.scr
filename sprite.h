#pragma once

#include <array>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <utility>
#include <map>
#include <vector>
#include <set>
#include <numeric>
#include <chrono>

#include "context.h"
#include "graphics.h"

class Trail;

class Sprite : public Identifiable<Empty> {
public:
	Sprite(const Texture *texture, const Point &home, const bool has_trail = true);

	std::vector<Trail> trail;

	void change_texture(const Texture *texture);
	virtual void draw(Context &ctx);
	virtual void update(Context &ctx);

	template <int C> float final() const {
		return std::get<C>(m_home) + std::get<C>(m_relpos);
	}

	Point &home();

protected:
	void transform();
	void update_trail();

	const Texture *m_texture;
	Point m_relpos;
	Point m_home;
	GLfloat m_size;
};

class Yonker : public Sprite {
public:
	enum Emotion {
		OPTIMISM = 0,
		EMPATHY = 1,
		AMBITION = 2,
		_EMOTIONS_COUNT
	};
	using EmotionVector = std::array<float, _EMOTIONS_COUNT>;

	Yonker(const Texture *texture, const Point &home);

	virtual void update(Context &ctx) override;

protected:
	const BitmapData &bitmap_for_current_emotion(Context &ctx) const;
	EmotionVector emotion_vector(Context &ctx) const;

	EmotionVector m_emotion_vector;
};

class Impostor : public Sprite {
public:
	Impostor(const PaletteData *palette, const Point &home);

	virtual void update(Context &ctx) override;

protected:
	static Bitmaps::Definition &random_bitmap();
};

class Trail : public Sprite {
public:
	Trail(const Texture *palette, const Point &home);

	virtual void update(Context &ctx) override;
};
