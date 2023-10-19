#include "sprite.h"
#include "noise.h"
#include "resources.h"
#include "config.h"

using std::get;

Sprite::Sprite(const Texture *texture, const Point &home) 
	: m_texture(texture), m_home(home), m_relpos(0.0f, 0.0f), m_size(cfg.at(SpriteSize) / 1000.0f) { }

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

	glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f + squarifiy_offset, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f - squarifiy_offset, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(1.0f - squarifiy_offset, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f + squarifiy_offset, 1.0f);

	glEnd();
	glPopMatrix();
}

void Sprite::update(Context &ctx) {
	auto wrap = [](float n, float min, float max) -> float {
		if (n < min) {
			return max - (min - n);
		} else if (n > max) {
			return min + (n - max);
		} else {
			return n;
		}
	};

	get<X>(m_home) = wrap(get<X>(m_home), -1.0f - cfg.at(YonkHomeDrift), 1.0f + cfg.at(YonkHomeDrift));
	get<Y>(m_home) = wrap(get<Y>(m_home), -1.0f - cfg.at(YonkHomeDrift), 1.0f + cfg.at(YonkHomeDrift));
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
	get<X>(m_relpos) = Noise::wiggle(
		get<X>(m_relpos),
		-cfg.at(YonkHomeDrift),
		cfg.at(YonkHomeDrift),
		cfg.at(YonkStepSize) * (emotion_magnitude * cfg.at(YonkShakeFactor))
	);

	get<Y>(m_relpos) = Noise::wiggle(
		get<Y>(m_relpos),
		-cfg.at(YonkHomeDrift),
		cfg.at(YonkHomeDrift),
		cfg.at(YonkStepSize) * (emotion_magnitude * cfg.at(YonkShakeFactor))
	);
	
	Sprite::update(ctx);

	change_texture(Texture::get(m_texture->palette(), bitmap_for_current_emotion(ctx)));
}

const Bitmap &Yonker::bitmap_for_current_emotion(Context &ctx) const {
	auto clamp = [](int n, int min, int max) -> int {
		return n < min ? min : (n > max) ? max : n;
	};

	auto emotion_map_index_of = [clamp](float emotion) -> int {
		return clamp(round(emotion * cfg.at(YonkEmotionScale)), -1, 1) + 1;
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
Impostor::Impostor(PaletteName palette, const Point &home)
	: Sprite(Texture::of(palette, random_bitmap()), home) { }

BitmapName Impostor::random_bitmap() {
	static std::vector<BitmapName> impostors = { cvjoy, nx, vx, lkmoyai, fn, fnplead };
	return impostors[(int) (Noise::random() * impostors.size())];
}
