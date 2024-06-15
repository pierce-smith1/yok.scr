#include <algorithm>

#include "sprite.h"
#include "noise.h"
#include "bitmaps.h"
#include "config.h"
#include "common.h"

using std::get;

Sprite::Sprite(const Texture *texture, const Point &home, const bool has_trail)
	: m_texture(texture),
	m_home(home),
	m_relpos(0.0, 0.0),
	m_size(cfg[Cfg::SpriteSize] / 1000.0f),
	m_trail_start_index(0),
	m_relpos_tendency(0.0, 0.0),
	m_frames_when_tendency_changes(0),
	m_tendency_distance_from_home_bias(default_tendency_distance_from_home_bias.first),
	m_wiggle_amount_bias(default_wiggle_amount_bias.first),
	m_distance_from_tendency_bias(default_distance_from_tendency_bias.first)
{
	if (has_trail) {
		for (int i = 0; i < TrailSprite::get_trail_length(); i++) {
			m_trail.emplace_back(texture, home);
		}
	}
}

void Sprite::change_texture(const Texture *texture) {
	m_texture = texture;
}

void Sprite::draw(Context &ctx) {
	draw_trail(ctx);

	m_texture->apply();

	glColor4d(1.0, 1.0, 1.0, 1.0);

	glPushMatrix();
	transform();
	glBegin(GL_QUADS);

	// Reality lives in a box that is square;
	// But plastered on a rectangular screen.
	// Here we adjust so the ratio's fair
	// And our wandering Llokin are properly seen.
	double squarifiy_offset = (double) ((long long) ctx.rect().right - ctx.rect().bottom) / ctx.rect().right;

	glTexCoord2d(1.0, 0.0); glVertex2d(1.0 - squarifiy_offset, -1.0);
	glTexCoord2d(1.0, 1.0); glVertex2d(1.0 - squarifiy_offset, 1.0);
	glTexCoord2d(0.0, 1.0); glVertex2d(-1.0 + squarifiy_offset, 1.0);
	glTexCoord2d(0.0, 0.0); glVertex2d(-1.0 + squarifiy_offset, -1.0);

	glEnd();
	glPopMatrix();
}

void Sprite::update(Context &ctx) {
	auto wrap = [](double home, double total, double min, double max) -> double {
		if (total < min) {
			return home + (max - min);
		} else if (total > max) {
			return home + (min - max);
		} else {
			return home;
		}
	};

	auto keep_in_bounds = [](double home, double total, double min, double max) -> double {
		if (total < min) {
			return home + (total - min);
		} else if (total > max) {
			return home + (max - total);
		} else {
			return home;
		}
	};

	double edge_boundary = 0.15 + m_size / 1.1;
	double horizontal_correction = max((double) ctx.rect().right / (double) ctx.rect().bottom, 1.0);
	double vertical_correction = max((double) ctx.rect().bottom / (double) ctx.rect().right, 1.0);
	if (allow_screen_wrapping) {
		get<X>(m_home) = wrap(get<X>(m_home), final<X>(), -1.0 - (edge_boundary / horizontal_correction), 1.0 + (edge_boundary / horizontal_correction));
		get<Y>(m_home) = wrap(get<Y>(m_home), final<Y>(), -1.0 - (edge_boundary / vertical_correction), 1.0 + (edge_boundary / vertical_correction));
	}
	else {
		// Still keep them within bounds if wrapping is not allowed. Should help prevent teleporting on screen when the pattern changes.
		// Some patterns will be fighting against this, but since it's happening off screen, it shouldn't matter.
		get<X>(m_home) = keep_in_bounds(get<X>(m_home), final<X>(), -1.0 - (edge_boundary / horizontal_correction), 1.0 + (edge_boundary / horizontal_correction));
		get<Y>(m_home) = keep_in_bounds(get<Y>(m_home), final<Y>(), -1.0 - (edge_boundary / vertical_correction), 1.0 + (edge_boundary / vertical_correction));
	}
}

Point &Sprite::home() {
	return m_home;
}

void Sprite::transform() {
	glTranslated(final<X>(), final<Y>(), 0.0);
	glScaled(m_size, m_size, 1.0);
}

void Sprite::update_trail() {
	if (TrailSprite::get_trail_length() < 1) {
		return;
	}

	get_trail() = TrailSprite(m_texture, Point(final<X>(), final<Y>()));
	increment_trail_index();
}

TrailSprite& Sprite::get_trail(const size_t index) {
	return m_trail.at((m_trail_start_index + index) % TrailSprite::get_trail_length());
}

void Sprite::increment_trail_index(const size_t amount) {
	m_trail_start_index += amount;
	m_trail_start_index %= TrailSprite::get_trail_length();
}

void Sprite::draw_trail(Context &ctx) {
	for (size_t i = 0; i < TrailSprite::get_trail_length(); i += TrailSprite::get_trail_space()) {
		get_trail(i).draw(ctx);
	}
}

void Sprite::randomize_tendency() {
	m_frames_when_tendency_changes += (size_t) round(randomize_tendency_variable(default_frames_between_tendency_changes)
		* (1 + cfg[Cfg::HomeDrift] / home_drift_divisor) / (1 + cfg[Cfg::ShakeFactor] / shake_divisor / 2));
	m_tendency_distance_from_home_bias = randomize_tendency_variable(default_tendency_distance_from_home_bias);
	m_wiggle_amount_bias = randomize_tendency_variable(default_wiggle_amount_bias);
	m_distance_from_tendency_bias = randomize_tendency_variable(default_distance_from_tendency_bias);

	get<X>(m_relpos_tendency) = Noise::wiggle(
		0.0,
		0.0,
		-cfg[Cfg::HomeDrift],
		cfg[Cfg::HomeDrift],
		cfg[Cfg::HomeDrift],
		m_tendency_distance_from_home_bias
	);
	get<Y>(m_relpos_tendency) = Noise::wiggle(
		0.0,
		0.0,
		-cfg[Cfg::HomeDrift],
		cfg[Cfg::HomeDrift],
		cfg[Cfg::HomeDrift],
		m_tendency_distance_from_home_bias
	);
}

template<typename T>
double Sprite::randomize_tendency_variable(pair_randomness<T> tendency_pair) {
	double tendency;

	bool increase = Noise::random() < 0.5;
	if (increase) {
		tendency = tendency_pair.first * (1 + tendency_pair.second * Noise::random());
	} else {
		tendency = tendency_pair.first / (1 + tendency_pair.second * Noise::random());
	}

	return tendency;
}

Yonker::Yonker(const Texture *texture, const Point &home)
	: Sprite(texture, home), m_emotion_vector({ 0.0, 0.0, 0.0 }) { }

void Yonker::update(Context &ctx) {
	m_emotion_vector = emotion_vector(ctx);

	auto abs_plus = [](double a, double b) -> double {
		return a + abs(b);
	};

	double emotion_magnitude =
		std::accumulate(m_emotion_vector.begin(), m_emotion_vector.end(), 0.0, abs_plus);

	// In little steps up and down they'll roam,
	// But never too far outside their home.
	if (cfg[Cfg::HomeDrift] >= 0.000001) {
		if (ctx.frame_count() >= m_frames_when_tendency_changes) {
			randomize_tendency();
		}

		get<X>(m_relpos) = Noise::wiggle(
			get<X>(m_relpos),
			get<X>(m_relpos_tendency),
			-cfg[Cfg::HomeDrift] / home_drift_divisor,
			cfg[Cfg::HomeDrift] / home_drift_divisor,
			cfg[Cfg::StepSize] * (emotion_magnitude * cfg[Cfg::ShakeFactor] / shake_divisor),
			m_wiggle_amount_bias,
			1.0 / m_distance_from_tendency_bias
		);

		get<Y>(m_relpos) = Noise::wiggle(
			get<Y>(m_relpos),
			get<Y>(m_relpos_tendency),
			-cfg[Cfg::HomeDrift] / home_drift_divisor,
			cfg[Cfg::HomeDrift] / home_drift_divisor,
			cfg[Cfg::StepSize] * (emotion_magnitude * cfg[Cfg::ShakeFactor] / shake_divisor),
			m_wiggle_amount_bias,
			1.0 / m_distance_from_tendency_bias
		);
	}

	Sprite::update(ctx);

	change_texture(Texture::get(m_texture->palette(), bitmap_for_current_emotion(ctx)));

	update_trail();
}

const BitmapData &Yonker::bitmap_for_current_emotion(Context &ctx) const {
	auto emotion_map_index_of = [](double emotion) -> int {
		return std::clamp((int) round(emotion * cfg[Cfg::EmotionScale]), -1, 1) + 1;
	};

	int empathetic = emotion_map_index_of(m_emotion_vector[EMPATHY]);
	int optimistic = emotion_map_index_of(m_emotion_vector[OPTIMISM]);
	int ambitious = emotion_map_index_of(m_emotion_vector[AMBITION]);

	static Bitmaps::Definition emotion_map[3][3][3] = {
		// Go down through the layers, and the soul empathatic,
		// Go down _within_ layers, and the heart optimistic,
		// Go off towards the right, and the head energetic.
		// The center is calm; the corners, eclectic!
		{
			{ Bitmaps::Lksix,      Bitmaps::Lkhusk,     Bitmaps::Lkhusk },
			{ Bitmaps::Lkunamused, Bitmaps::Lkunamused, Bitmaps::Lksix },
			{ Bitmaps::Lkxd,       Bitmaps::Lkxd,       Bitmaps::Lksix },
		},
		{
			{ Bitmaps::Lkunamused, Bitmaps::Lkunamused, Bitmaps::Lk },
			{ Bitmaps::Lkconcern,  Bitmaps::Lk,         Bitmaps::Lk },
			{ Bitmaps::Lkconcern,  Bitmaps::Lkthumbsup, Bitmaps::Lkthumbsup },
		},
		{
			{ Bitmaps::Lkexhausted, Bitmaps::Lkexhausted, Bitmaps::Lkexhausted },
			{ Bitmaps::Lkthink,     Bitmaps::Lkthink,     Bitmaps::Lkjoy },
			{ Bitmaps::Lkjoy,       Bitmaps::Lkcool,      Bitmaps::Lksix },
		},
	};

	return *emotion_map[empathetic][optimistic][ambitious].data;
}

std::array<double, Yonker::_EMOTIONS_COUNT> Yonker::emotion_vector(Context &ctx) const {
	// Continous noise will be perfect for this;
	// Nearby to those pissed will also be pissed.
	return {
		PerlinNoise::get(final<X>() + ctx.t(), final<Y>() + ctx.t(), ctx.t()),
		PerlinNoise::get(final<X>() - ctx.t(), final<Y>() + ctx.t(), ctx.t()),
		PerlinNoise::get(final<X>() + ctx.t(), final<Y>() - ctx.t(), ctx.t()),
	};
}

// A strange sillouette appears in the dark...
// That's no Llokin! That's something sinistrous!
// The temper of character just misses the mark...
// Emergency meeting! That's awfully suspicious!
Impostor::Impostor(const PaletteData *palette, const Point &home)
	: Sprite(Texture::of(palette, random_bitmap()), home) { }

void Impostor::update(Context &ctx) {
	Sprite::update(ctx);

	update_trail();
}

Bitmaps::Definition &Impostor::random_bitmap() {
	static auto impostors = Bitmaps::bitmaps_of_group(BitmapGroup::Impostor);
	static auto yoy = Bitmaps::bitmaps_of_group(BitmapGroup::YoyImpostor);

	if (Noise::random() < 0.5) {
		return impostors[(size_t) (Noise::random() * impostors.size())];
	} else {
		return yoy[(size_t) (Noise::random() * yoy.size())];
	}
}

TrailSprite::TrailSprite(const Texture *texture, const Point &home)
	: Sprite(texture, home, false) { }

void TrailSprite::update(Context &ctx) { }

void TrailSprite::draw_trail(Context &ctx) { }

int TrailSprite::get_trail_length() {
	if (cfg[Cfg::TrailsEnabled] != 1.0) {
		return 0;
	}

	int max_trail = (int) round(cfg[Cfg::MaxTrailCount] / cfg[Cfg::SpriteCount]);
	int trail_length = std::clamp((int) round(cfg[Cfg::TrailLength]) + 1, 1, max_trail) - 1;
	return trail_length * get_trail_space();
}

int TrailSprite::get_trail_space() {
	return (int) max(round(cfg[Cfg::TrailSpace]), 1);
}
