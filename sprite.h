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

class Sprite : public Identifiable<Empty> {
public:
	Sprite(const Texture *texture, const Point &home);

	void change_texture(const Texture *texture);
	virtual void draw(Context &ctx);
	virtual void update(Context &ctx);

	template <int C> float final() const {
		return std::get<C>(m_home) + std::get<C>(m_relpos);
	}

protected:
	void transform();

	const Texture *m_texture;
	Point m_relpos;
	Point m_home;
	GLfloat m_size;

	friend class SpriteChoreographer;
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
	const Bitmap &bitmap_for_current_emotion(Context &ctx) const;
	EmotionVector emotion_vector(Context &ctx) const;

	EmotionVector m_emotion_vector;
};

class Impostor : public Sprite {
public:
	Impostor(PaletteName palette, const Point &home);

protected:
	static BitmapName random_bitmap();
};

class Yoy : public Sprite {
public:
	Yoy(PaletteName palette, const Point& home);

protected:
	static BitmapName random_bitmap();
};
