#include "spritecontrol.h"
#include "resources.h"
#include "config.h"
#include "noise.h"

using std::get;

SpriteGenerator::SpriteGenerator() {
	using namespace std::chrono;
	std::srand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

	std::vector<int> bag_of_palettes(_PALETTE_COUNT);
	std::iota(bag_of_palettes.begin(), bag_of_palettes.end(), 0);

	for (int i = 0; i < round(cfg.at(MaxColors)); i++) {
		size_t random_palette_index = std::rand() % bag_of_palettes.size();
		m_palettes.push_back((PaletteName) bag_of_palettes[random_palette_index]);
		bag_of_palettes.erase(bag_of_palettes.begin() + random_palette_index);
	}
}

std::vector<Sprite *> SpriteGenerator::make(unsigned int n) const {
	Sprites sprites;

	for (float y = -1.2f; y < 1.2f; y += 1.0f / sqrt(cfg.at(SpriteCount))) {
		for (float x = -1.2f; x < 1.2f; x += 1.0f / sqrt(cfg.at(SpriteCount))) {
			if (Noise::random() < (6.0f / 17.0f)) {
				sprites.push_back(new Impostor(next_palette(), Point(x, y)));
			} else {
				sprites.push_back(new Yonker(next_texture(), Point(x, y)));
			}
		}
	}

	return sprites;
}

const Texture *SpriteGenerator::next_texture() const {
	return Texture::of(next_palette(), lk);
}

PaletteName SpriteGenerator::next_palette() const {
	// A weighted coin?! Now that's certainly cheating!
	// You'd be kicked outta Vegas for logarithmic repeating.
	while (true) {
		for (PaletteName palette_name : m_palettes) {
			if (Noise::random() < 1.8f / cfg.at(MaxColors)) {
				return palette_name;
			}
		}
	}
}

SpriteChoreographer::SpriteChoreographer(SpritePattern choreography, Sprites sprites, Context *ctx)
	: m_pattern(choreography), m_ctx(ctx), m_sprites(sprites), m_id_offset(0) { }

void SpriteChoreographer::update() {
	for (Sprite *sprite : m_sprites) {
		move_functions[m_pattern](sprite, m_ctx, hash(sprite->id() + m_id_offset));
		sprite->update(*m_ctx);
	}

	// We must change the pattern every so often...
	// But not, of course, when we're just getting started.
	if (m_ctx->frame_count() % (int) cfg.at(PatternChangeInterval) == 0 && m_ctx->frame_count() != 0) {
		m_pattern = (SpritePattern) (Noise::random() * _PATTERN_COUNT);
		m_id_offset++;
	}
}

float SpriteChoreographer::hash(unsigned int n) {
	return ((n * n * 562448657) % 4096) / 4096.0f;
}

std::map<SpritePattern, SpriteChoreographer::MoveFunction> SpriteChoreographer::move_functions {
	{ Roamers, [](Sprite *sprite, Context *ctx, float offset) {
		// Every pattern is made of three things!
		// The sprite, the creature who kindly participates -
		// The context, the timepiece by which we will calculate -
		// And the offset, by which our fate is encoded
		// One onto zero that chaos corroded.
		get<X>(sprite->m_home) += offset / cfg.at(TimeDivisor);
		get<Y>(sprite->m_home) += sin(ctx->t() * offset) / cfg.at(TimeDivisor);
	}},
	{ Waves, [](Sprite *sprite, Context *ctx, float offset) {
		get<X>(sprite->m_home) += sin(ctx->t() * offset) / cfg.at(TimeDivisor);
		get<Y>(sprite->m_home) += cos(ctx->t() * offset) / cfg.at(TimeDivisor);
	}},
	{ Square, [](Sprite *sprite, Context *ctx, float offset) {
		get<X>(sprite->m_home) += offset < 0.5 ? ((1.0f - offset) / cfg.at(TimeDivisor)) : 0.0f;
		get<Y>(sprite->m_home) += offset < 0.5 ? 0.0f : (offset / cfg.at(TimeDivisor));
	}},
	{ Bouncy, [](Sprite *sprite, Context *ctx, float offset) {
		static int NorthWest = 0b01;
		static int NorthEast = 0b00;
		static int SouthEast = 0b10;
		static int SouthWest = 0b11;
		static int West = 0b1;
		static int South = 0b10;

		static std::map<Id, int> directions;

		float lateral_modifier = (directions[sprite->id()] & West) ? -1.0f : 1.0f;
		float vertical_modifier = (directions[sprite->id()] & South) ? -1.0f : 1.0f;

		get<X>(sprite->m_home) += (offset / cfg.at(TimeDivisor)) * lateral_modifier;
		get<Y>(sprite->m_home) += (1.0f - offset) / cfg.at(TimeDivisor) * vertical_modifier;

		if (get<X>(sprite->m_home) > 1.0f || get<X>(sprite->m_home) < -1.0f) {
			directions[sprite->id()] ^= West;
			get<X>(sprite->m_home) = signbit(get<X>(sprite->m_home)) ? -1.0f : 1.0f;
		}

		if (get<Y>(sprite->m_home) > 1.0f || get<Y>(sprite->m_home) < -1.0f) {
			directions[sprite->id()] ^= South;
			get<Y>(sprite->m_home) = signbit(get<Y>(sprite->m_home)) ? -1.0f : 1.0f;
		}
	}},
	{ Lissajous, [](Sprite *sprite, Context *ctx, float offset) {
		// Unlike the patterns you see above,
		// For this one, well, push comes to shove.
		// We know exactly where we must be,
		// So we won't let our sprites roam around freely...
		float target_x = sin(ctx->t() - (offset * 0.07f * cfg.at(SpriteCount))) * 0.8f;
		float target_y = cos(ctx->t() - (offset * 0.05f * cfg.at(SpriteCount))) * 0.8f;

		// But! To send them straight to their fate is unsightly,
		// So instead of assign, we just push ever lightly.
		get<X>(sprite->m_home) = target_x + ((get<X>(sprite->m_home) - target_x) * 0.9f);
		get<Y>(sprite->m_home) = target_y + ((get<Y>(sprite->m_home) - target_y) * 0.9f);
	}},
	{ Rose, [](Sprite *sprite, Context *ctx, float offset) {
		float t = ctx->t() - (offset * 0.03f * cfg.at(SpriteCount));
		float r = 0.04f * cfg.at(SpriteCount) * t;

		float target_x = sin(r) * cos(t) * 0.8f;
		float target_y = sin(r) * sin(t) * 0.8f;

		get<X>(sprite->m_home) = target_x + ((get<X>(sprite->m_home) - target_x) * 0.9f);
		get<Y>(sprite->m_home) = target_y + ((get<Y>(sprite->m_home) - target_y) * 0.9f);
	}},
	{ Lattice, [](Sprite *_sprite, Context *_ctx, float _offset) {
		// The flocking of birds, the schooling of fish,
		// The dancing of insects with a firefly's wish...
		// There's beauty in movement, I must agree,
		// But beauty in stillness, I also can see.
	}}
};
