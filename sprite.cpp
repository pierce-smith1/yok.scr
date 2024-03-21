#include <algorithm>

#include "sprite.h"
#include "noise.h"
#include "bitmaps.h"
#include "config.h"
#include "common.h"

using std::get;

Sprite::Sprite(const Texture *texture, const Point &home) 
	: m_texture(texture), m_home(home), m_relpos(0.0f, 0.0f), m_size(cfg[Cfg::SpriteSize] / 1000.0f) { }

void Sprite::change_texture(const Texture *texture) {
	m_texture = texture;
}

void Sprite::draw(Context &ctx) {
	update(ctx);
	m_texture->apply();

	glColor4d(1.0, 1.0, 1.0, 1.0);

	glPushMatrix();
	transform();
	glBegin(GL_QUADS);

	// Reality lives in a box that is square;
	// But plastered on a rectangular screen.
	// Here we adjust so the ratio's fair
	// And our wandering Llokin are properly seen.
	double squarifiy_offset = (double) (ctx.rect().right - ctx.rect().bottom) / ctx.rect().right;

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

	double edge_boundary = 0.15f + m_size / 1.1f;
	double horizontal_correction = max((double) ctx.rect().right / (double) ctx.rect().bottom, 1.0f);
	double vertical_correction = max((double) ctx.rect().bottom / (double) ctx.rect().right, 1.0f);
	get<X>(m_home) = wrap(get<X>(m_home), final<X>(), -1.0f - (edge_boundary / horizontal_correction), 1.0f + (edge_boundary / horizontal_correction));
	get<Y>(m_home) = wrap(get<Y>(m_home), final<Y>(), -1.0f - (edge_boundary / vertical_correction), 1.0f + (edge_boundary / vertical_correction));
}

Point &Sprite::home() {
	return m_home;
}

void Sprite::transform() {
	glTranslated(final<X>(), final<Y>(), 0.0f);
	glScaled(m_size, m_size, 1.0f);
}

Yonker::Yonker(const Texture *texture, const Point &home) 
	: Sprite(texture, home) { }

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
		get<X>(m_relpos) = Noise::wiggle(
			get<X>(m_relpos),
			-cfg[Cfg::HomeDrift],
			cfg[Cfg::HomeDrift],
			cfg[Cfg::StepSize] * (emotion_magnitude * cfg[Cfg::ShakeFactor]) / max((cfg[Cfg::HomeDrift] / Cfg::HomeDrift.default_), 1)
		);

		get<Y>(m_relpos) = Noise::wiggle(
			get<Y>(m_relpos),
			-cfg[Cfg::HomeDrift],
			cfg[Cfg::HomeDrift],
			cfg[Cfg::StepSize] * (emotion_magnitude * cfg[Cfg::ShakeFactor]) / max((cfg[Cfg::HomeDrift] / Cfg::HomeDrift.default_), 1)
		);
	}

	Sprite::update(ctx);

	change_texture(Texture::get(m_texture->palette(), bitmap_for_current_emotion(ctx)));
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

Bitmaps::Definition &Impostor::random_bitmap() {
	static auto impostors = Bitmaps::bitmaps_of_group(BitmapGroup::Impostor);
	static auto yoy = Bitmaps::bitmaps_of_group(BitmapGroup::YoyImpostor);

	if (Noise::random() < 0.5) {
		return impostors[(int) (Noise::random() * impostors.size())];
	} else {
		return yoy[(int) (Noise::random() * yoy.size())];
	}
}
