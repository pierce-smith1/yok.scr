#include "sprite.h"
#include "noise.h"
#include "resources.h"
#include "config.h"

#include <math.h>

float wrap(float n, float min, float max) {
	if (n < min) {
		return max - (min - n);
	} else if (n > max) {
		return min + (n - max);
	} else {
		return n;
	}
};

int clamp(int n, int min, int max) {
	return n < min ? min : (n > max) ? max : n;
}

int emotion_map_index_of(float emotion) {
	return clamp(round(emotion * cfg[YonkEmotionScale]), -1, 1) + 1;
}

Sprite::Sprite(const Texture *texture, const Point &home) 
	: m_texture(texture), 
	m_home(home), 
	m_relpos(0.0f, 0.0f), 
	m_size(cfg[SpriteSize] / 1000.0f) { }

void Sprite::change_texture(const Texture *texture) {
	m_texture = texture;
}

void Sprite::draw(Context &ctx) {
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
	m_home.x = wrap(m_home.x, -1.0f - cfg[YonkHomeDrift], 1.0f + cfg[YonkHomeDrift]);
	m_home.y = wrap(m_home.y, -1.0f - cfg[YonkHomeDrift], 1.0f + cfg[YonkHomeDrift]);
}

float Sprite::final_x() const {
	return m_home.x + m_relpos.x;
}

float Sprite::final_y() const {
	return m_home.y + m_relpos.y;
}

void Sprite::transform() {
	glTranslatef(final_x(), final_y(), 0.0f);
	glScalef(m_size, m_size, 1.0f);
}

Yonker::Yonker(const Texture *texture, const Point &home) 
	: Sprite(texture, home) { }

void Yonker::update(Context &ctx) {
	m_emotion_vector = emotion_vector(ctx);

	float emotion_magnitude = 0.0f;
	for (EmotionVector::iterator it = m_emotion_vector.begin(); it != m_emotion_vector.end(); ++it) {
		emotion_magnitude += fabs(*it);
	}

	// In little steps up and down they'll roam,
	// But never too far outside their home.
	m_relpos.x = Noise::wiggle(
		m_relpos.x,
		-cfg[YonkHomeDrift],
		cfg[YonkHomeDrift],
		cfg[YonkStepSize] * (emotion_magnitude * cfg[YonkShakeFactor])
	);

	m_relpos.y = Noise::wiggle(
		m_relpos.y,
		-cfg[YonkHomeDrift],
		cfg[YonkHomeDrift],
		cfg[YonkStepSize] * (emotion_magnitude * cfg[YonkShakeFactor])
	);
	
	Sprite::update(ctx);

	change_texture(Texture::get(m_texture->palette(), bitmap_for_current_emotion(ctx)));
}

const Bitmap &Yonker::bitmap_for_current_emotion(Context &ctx) const {
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

EmotionVector Yonker::emotion_vector(Context &ctx) const {
	// Continous noise will be perfect for this;
	// Nearby to those pissed will also be pissed.
	EmotionVector emotions;

	emotions.push_back(PerlinNoise::get(final_x() + ctx.t(), final_y() + ctx.t(), ctx.t()));
	emotions.push_back(PerlinNoise::get(final_x() - ctx.t(), final_y() + ctx.t(), ctx.t()));
	emotions.push_back(PerlinNoise::get(final_x() + ctx.t(), final_y() - ctx.t(), ctx.t()));

	return emotions;
}

// A strange sillouette appears in the dark...
// That's no Llokin! That's something sinistrous!
// The temper of character just misses the mark...
// Emergency meeting! That's awfully suspicious!
Impostor::Impostor(PaletteName palette, const Point &home)
	: Sprite(Texture::of(palette, random_bitmap()), home) { }

BitmapName Impostor::random_bitmap() {
	static std::vector<BitmapName> impostors; 
	
	if (impostors.empty()) {
		impostors.push_back(cvjoy);
		impostors.push_back(nx);
		impostors.push_back(vx); 
		impostors.push_back(lkmoyai); 
		impostors.push_back(fn); 
		impostors.push_back(fnplead);
	}

	return impostors[(int) (Noise::random() * impostors.size())];
}
