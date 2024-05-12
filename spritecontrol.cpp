#include "spritecontrol.h"
#include "resources.h"
#include <math.h>

#include "config.h"
#include "noise.h"

bool signbit(float x) {
	return x < 0.0f ? 1 : 0;
}

SpriteGenerator::SpriteGenerator() {
	std::vector<int> bag_of_palettes;
	
	for (int i = 0; i < _PALETTE_COUNT; i++) {
		bag_of_palettes.push_back(i);
	}

	for (int ii = 0; ii < round(cfg[MaxColors]); ii++) {
		size_t random_palette_index = rand() % bag_of_palettes.size();
		m_palettes.push_back((PaletteName) bag_of_palettes[random_palette_index]);
		bag_of_palettes.erase(bag_of_palettes.begin() + random_palette_index);
	}
}

std::vector<Sprite *> SpriteGenerator::make(unsigned int n) {
	Sprites sprites;

	for (float y = -1.2f; y < 1.2f; y += 1.0f / sqrt(cfg[SpriteCount])) {
		for (float x = -1.2f; x < 1.2f; x += 1.0f / sqrt(cfg[SpriteCount])) {
			if (Noise::random() < 0.001f) {
				sprites.push_back(new Impostor(next_palette(), Point(x, y)));
			} else {
				sprites.push_back(new Yonker(next_texture(), Point(x, y)));
			}
		}
	}

	return sprites;
}

const Texture *SpriteGenerator::next_texture() {
	return Texture::of(next_palette(), lk);
}

PaletteName SpriteGenerator::next_palette() {
	// A weighted coin?! Now that's certainly cheating!
	// You'd be kicked outta Vegas for logarithmic repeating.
	while (true) {
		for (std::vector<PaletteName>::iterator it = m_palettes.begin(); it != m_palettes.end(); ++it) {
			if (Noise::random() < 1.8f / cfg[MaxColors]) {
				return *it;
			}
		}
	}
}

SpriteChoreographer::SpriteChoreographer(SpritePattern choreography, Sprites sprites, Context *ctx)
	: m_pattern(choreography), m_ctx(ctx), m_sprites(sprites), m_id_offset(0) { }

void SpriteChoreographer::update() {
	for (Sprites::iterator it = m_sprites.begin(); it != m_sprites.end(); ++it) {
		do_move_function(m_pattern, *it, m_ctx, hash((*it)->id() + m_id_offset));
		(*it)->update(*m_ctx);
	}

	// We must change the pattern every so often...
	// But not, of course, when we're just getting started.
	if (m_ctx->frame_count() % (int) cfg[PatternChangeInterval] == 0 && m_ctx->frame_count() != 0) {
		m_pattern = (SpritePattern) (int) (Noise::random() * _PATTERN_COUNT);
		m_id_offset++;
	}
}

void SpriteChoreographer::do_move_function(SpritePattern pattern, Sprite *sprite, Context *ctx, float offset) {
	switch (pattern) {
	case Roamers: {
		sprite->m_home.x += offset / cfg[TimeDivisor];
		sprite->m_home.y += sin(ctx->t() * offset) / cfg[TimeDivisor];

		break;
	} case Waves: {
		sprite->m_home.x += sin(ctx->t() * offset) / cfg[TimeDivisor];
		sprite->m_home.y += cos(ctx->t() * offset) / cfg[TimeDivisor];

		break;
	} case Square: {
		sprite->m_home.x += offset < 0.5f ? ((1.0f - offset) / cfg[TimeDivisor]) : 0.0f;
		sprite->m_home.y += offset < 0.5f ? 0.0f : (offset / cfg[TimeDivisor]);

		break;
	} case Bouncy: {
		static int NorthWest = 1;
		static int NorthEast = 0;
		static int SouthEast = 2;
		static int SouthWest = 3;
		static int West = 1;
		static int South = 2;

		static std::map<Id, int> directions;

		float lateral_modifier = (directions[sprite->id()] & West) ? -1.0f : 1.0f;
		float vertical_modifier = (directions[sprite->id()] & South) ? -1.0f : 1.0f;

		sprite->m_home.x += (offset / cfg[TimeDivisor]) * lateral_modifier;
		sprite->m_home.y += (1.0f - offset) / cfg[TimeDivisor] * vertical_modifier;

		if (sprite->m_home.x > 1.0f || sprite->m_home.x < -1.0f) {
			directions[sprite->id()] ^= West;
			sprite->m_home.x = signbit(sprite->m_home.x) ? -1.0f : 1.0f;
		}

		if (sprite->m_home.y > 1.0f || sprite->m_home.y < -1.0f) {
			directions[sprite->id()] ^= South;
			sprite->m_home.y = signbit(sprite->m_home.y) ? -1.0f : 1.0f;
		}

		break;
	} case Lissajous: {
		// Unlike the patterns you see above,
		// For this one, well, push comes to shove.
		// We know exactly where we must be,
		// So we won't let our sprites roam around freely...
		float target_x = sin(ctx->t() - (offset * 0.07f * cfg[SpriteCount])) * 0.8f;
		float target_y = cos(ctx->t() - (offset * 0.05f * cfg[SpriteCount])) * 0.8f;

		// But! To send them straight to their fate is unsightly,
		// So instead of assign, we just push ever lightly.
		sprite->m_home.x = target_x + ((sprite->m_home.x - target_x) * 0.9f);
		sprite->m_home.y = target_y + ((sprite->m_home.y - target_y) * 0.9f);

		break;
	} case Rose: {
		float t = ctx->t() - (offset * 0.03f * cfg[SpriteCount]);
		float r = 0.04f * cfg[SpriteCount] * t;

		float target_x = sin(r) * cos(t) * 0.8f;
		float target_y = sin(r) * sin(t) * 0.8f;

		sprite->m_home.x = target_x + ((sprite->m_home.x - target_x) * 0.9f);
		sprite->m_home.y = target_y + ((sprite->m_home.y - target_y) * 0.9f);

		break;
	} case Lattice: {
		// The flocking of birds, the schooling of fish,
		// The dancing of insects with a firefly's wish...
		// There's beauty in movement, I must agree,
		// But beauty in stillness, I also can see.

		break;
	}
	}
}

float SpriteChoreographer::hash(unsigned int n) {
	return ((n * n * 562448657) % 4096) / 4096.0f;
}
