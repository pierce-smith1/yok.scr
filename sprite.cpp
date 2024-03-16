#include <algorithm>

#include "sprite.h"
#include "noise.h"
#include "resources.h"
#include "config.h"
#include "common.h"

using std::get;

Sprite::Sprite(const Texture *texture, const Point &home) 
	: m_texture(texture), m_home(home), m_relpos(0.0f, 0.0f), m_size(cfg[ConfigOptions::SpriteSize] / 1000.0f) { }

void Sprite::change_texture(const Texture *texture) {
	m_texture = texture;
}

void Sprite::draw(Context &ctx) {
	update(ctx);
	m_texture->apply();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glPushMatrix();
	transform();
	glBegin(GL_QUADS);

	// Reality lives in a box that is square;
	// But plastered on a rectangular screen.
	// Here we adjust so the ratio's fair
	// And our wandering Llokin are properly seen.
	float squarifiy_offset = (float) (ctx.rect().right - ctx.rect().bottom) / ctx.rect().right;

	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f - squarifiy_offset, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f - squarifiy_offset, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f + squarifiy_offset, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f + squarifiy_offset, -1.0f);

	glEnd();
	glPopMatrix();

}

void Sprite::update(Context &ctx) {
	auto wrap = [](float home, float total, float min, float max) -> float {
		if (total < min) {
			return home + (max - min);
		} else if (total > max) {
			return home + (min - max);
		} else {
			return home;
		}
	};

	float edge_boundary = 0.15f + m_size / 1.1f;
	float horizontal_correction = max((float) ctx.rect().right / (float) ctx.rect().bottom, 1.0f);
	float vertical_correction = max((float) ctx.rect().bottom / (float) ctx.rect().right, 1.0f);
	get<X>(m_home) = wrap(get<X>(m_home), final<X>(), -1.0f - (edge_boundary / horizontal_correction), 1.0f + (edge_boundary / horizontal_correction));
	get<Y>(m_home) = wrap(get<Y>(m_home), final<Y>(), -1.0f - (edge_boundary / vertical_correction), 1.0f + (edge_boundary / vertical_correction));
}

Point &Sprite::home() {
	return m_home;
}

void Sprite::transform() {
	glTranslatef(final<X>(), final<Y>(), 0.0f);
	glScalef(m_size, m_size, 1.0f);
}

Yonker::Yonker(const Texture *texture, const Point &home) 
	: Sprite(texture, home) { }

void Yonker::update(Context &ctx) {
	m_emotion_vector = emotion_vector(ctx);

	auto abs_plus = [](float a, float b) -> float {
		return a + abs(b);
	};

	float emotion_magnitude =
		std::accumulate(m_emotion_vector.begin(), m_emotion_vector.end(), 0.0f, abs_plus);

	// In little steps up and down they'll roam,
	// But never too far outside their home.
	if (cfg[ConfigOptions::HomeDrift] >= 0.000001f) {
		get<X>(m_relpos) = Noise::wiggle(
			get<X>(m_relpos),
			-cfg[ConfigOptions::HomeDrift],
			cfg[ConfigOptions::HomeDrift],
			cfg[ConfigOptions::StepSize] * (emotion_magnitude * cfg[ConfigOptions::ShakeFactor]) / max((cfg[ConfigOptions::HomeDrift] / ConfigOptions::HomeDrift.default_), 1)
		);

		get<Y>(m_relpos) = Noise::wiggle(
			get<Y>(m_relpos),
			-cfg[ConfigOptions::HomeDrift],
			cfg[ConfigOptions::HomeDrift],
			cfg[ConfigOptions::StepSize] * (emotion_magnitude * cfg[ConfigOptions::ShakeFactor]) / max((cfg[ConfigOptions::HomeDrift] / ConfigOptions::HomeDrift.default_), 1)
		);
	}

	Sprite::update(ctx);

	change_texture(Texture::get(m_texture->palette(), bitmap_for_current_emotion(ctx)));
}

const Bitmap &Yonker::bitmap_for_current_emotion(Context &ctx) const {
	auto emotion_map_index_of = [](float emotion) -> int {
		return std::clamp((int) round(emotion * cfg[ConfigOptions::EmotionScale]), -1, 1) + 1;
	};

	int empathetic = emotion_map_index_of(m_emotion_vector[EMPATHY]);
	int optimistic = emotion_map_index_of(m_emotion_vector[OPTIMISM]);
	int ambitious = emotion_map_index_of(m_emotion_vector[AMBITION]);

	static BitmapName emotion_map[3][3][3] = {
		// Go down through the layers, and the soul empathatic,
		// Go down _within_ layers, and the heart optimistic,
		// Go off towards the right, and the head energetic.
		// The center is calm; the corners, eclectic!
		{
			{ lksix,          lkhusk,     lkhusk },
			{ lkunamused, lkunamused,      lksix },
			{ lkxd,             lkxd,      lksix },
		},
		{
			{ lkunamused, lkunamused,         lk },
			{  lkconcern,         lk,         lk },
			{  lkconcern, lkthumbsup, lkthumbsup },
		},
		{
			{ lkexhausted, lkexhausted, lkexhausted },
			{     lkthink,     lkthink,       lkjoy },
			{       lkjoy,      lkcool,       lksix },
		},
	};

	return *load_bitmap(emotion_map[empathetic][optimistic][ambitious]);
}

std::array<float, Yonker::_EMOTIONS_COUNT> Yonker::emotion_vector(Context &ctx) const {
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
Impostor::Impostor(const Palette *palette, const Point &home)
	: Sprite(Texture::of(palette, random_bitmap()), home) { }

BitmapName Impostor::random_bitmap() {
	static std::vector<BitmapName> impostors = { cvjoy, nx, vx, lkmoyai, fn, fnplead };
	static std::vector<BitmapName> yoy = { lkyoy, lkyoyapprove, fnyoy, cvyoy };

	if (Noise::random() < 0.5f) {
		return impostors[(int) (Noise::random() * impostors.size())];
	} else {
		return yoy[(int) (Noise::random() * yoy.size())];
	}
}
