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

class TrailSprite;

class Sprite : public Identifiable<Empty> {
public:
	Sprite(const Texture *texture, const Point &home, const bool has_trail = true);

	void change_texture(const Texture *texture);
	virtual void draw(Context &ctx);
	virtual void update(Context &ctx);

	template <int C> double final() const {
		return std::get<C>(m_home) + std::get<C>(m_relpos);
	}

	Point &home();
	static GLdouble get_size();

protected:
	void transform();
	void update_trail();
	void increment_trail_index(const size_t amount = 1);
	TrailSprite& get_trail(const size_t index = 0);
	virtual void draw_trail(Context &ctx);

	const Texture *m_texture;
	Point m_relpos;
	Point m_home;
	GLdouble m_size;
	std::vector<TrailSprite> m_trail;
	size_t m_trail_start_index;
};

class SpriteWiggler {
public:
	SpriteWiggler();
	void randomize_tendency();
	void wiggle_sprite(Context &ctx, const Point &home, Point &relpos, const double magnitude);

private:
	static double wiggle_coordinate(double base, double center, double min, double max, double step, double rand_exp = 1.0, double distance_exp = 1.0);
	template<typename T>
	double randomize_tendency_variable(std::pair<T, double> tendency_pair);

	// First number gets multiplied or divided by 1 + random(second number)
	inline const static std::pair<size_t, double> default_frames_between_tendency_changes = { 10 * 30, 0.5 };
	inline const static std::pair<double, double> default_tendency_distance_from_home_bias = { 1.5, 0.2 };
	inline const static std::pair<double, double> default_wiggle_amount_bias = { 1.0, 0.25 };
	inline const static std::pair<double, double> default_distance_from_tendency_bias = { 3.0, 7.0 / 3.0 };
	inline const static double shake_divisor = 1 / 0.3;
	inline const static double home_drift_divisor = 10.0;

	Point m_relpos_tendency;
	size_t m_frames_when_tendency_changes;
	double m_tendency_distance_from_home_bias;
	double m_wiggle_amount_bias;
	double m_distance_from_tendency_bias;
};

class Yonker : public Sprite {
public:
	enum Emotion {
		OPTIMISM = 0,
		EMPATHY = 1,
		AMBITION = 2,
		_EMOTIONS_COUNT
	};
	using EmotionVector = std::array<double, _EMOTIONS_COUNT>;

	Yonker(const Texture *texture, const Point &home);

	virtual void update(Context &ctx) override;

protected:
	const BitmapData &bitmap_for_current_emotion(Context &ctx) const;
	EmotionVector emotion_vector(Context &ctx) const;

	EmotionVector m_emotion_vector;
	SpriteWiggler m_sprite_wiggler;
};

class Impostor : public Sprite {
public:
	Impostor(const PaletteData *palette, const Point &home);

	virtual void update(Context &ctx) override;

protected:
	static Bitmaps::Definition &random_bitmap();
};

class TrailSprite : public Sprite {
public:
	TrailSprite(const Texture *palette, const Point &home);

	virtual void update(Context &ctx) override;

	static int get_trail_length();
	static int get_trail_space();

protected:
	virtual void draw_trail(Context &ctx) override;
};
