#include "spritecontrol.h"
#include "resources.h"
#include "config.h"
#include "noise.h"
#include "configdialog.h"

#include <math.h>

using std::get;

SpriteGenerator::SpriteGenerator() {
	using namespace std::chrono;
	std::srand(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());

	PaletteGroup palette_group = (PaletteGroup) (cfg[ConfigOptions::Palette]);

	std::vector<const Palette *> bag_of_palettes;
	if (palette_group == PaletteGroup::RandomlyGenerated) {
		for (int i = ConfigOptions::MaxColors.range.first; i < ConfigOptions::MaxColors.range.second; i++) {
			bag_of_palettes.push_back(RandomPalettes::random(i));
		}
	} else {
		auto palette_names = PALETTES_BY_GROUP.at(palette_group);
		for (auto name : palette_names) {
			bag_of_palettes.push_back(&PALETTES.at(name));
		}
	}

	int max_colors = (int) round(cfg[ConfigOptions::MaxColors] * bag_of_palettes.size() / ConfigOptions::MaxColors.range.second);
	max_colors = std::clamp(max_colors, (int) ConfigOptions::MaxColors.range.first, (int) bag_of_palettes.size());

	for (int i = 0; i < max_colors; i++) {
		size_t random_palette_index = std::rand() % bag_of_palettes.size();
		m_palettes.push_back(bag_of_palettes[random_palette_index]);
		bag_of_palettes.erase(bag_of_palettes.begin() + random_palette_index);
	}
}

std::vector<Sprite *> SpriteGenerator::make(unsigned int n) const {
	Sprites sprites;

	for (float y = -1.2f; y < 1.2f; y += 1.0f / sqrt(cfg[ConfigOptions::SpriteCount])) {
		for (float x = -1.2f; x < 1.2f; x += 1.0f / sqrt(cfg[ConfigOptions::SpriteCount])) {
			if (Noise::random() < pow(cfg[ConfigOptions::ImpostorChance], 3)) {
				sprites.push_back(new Impostor(next_palette(), Point(x, y)));
			} else {
				sprites.push_back(new Yonker(next_texture(), { x, y }));
			}
		}
	}

	return sprites;
}

const Texture *SpriteGenerator::next_texture() const {
	return Texture::of(next_palette(), lk);
}

const Palette *SpriteGenerator::next_palette() const {
	// A weighted coin?! Now that's certainly cheating!
	// You'd be kicked outta Vegas for logarithmic repeating.
	while (true) {
		for (auto palette : m_palettes) {
			if (Noise::random() < 1.8f / cfg[ConfigOptions::MaxColors]) {
				return palette;
			}
		}
	}
}

SpriteChoreographer::SpriteChoreographer(PatternName choreography, Sprites *sprites, Context *ctx)
	: m_pattern(choreography), m_ctx(ctx), m_sprites(sprites)
{ 
	m_players = { new SinglePassPlayer(sprites, ctx), new GlobalPlayer(sprites, ctx) };
	update_player();
}

void SpriteChoreographer::update() {
	m_current_player->update();
	if (should_change_pattern()) {
		change_pattern();
	}
}

bool SpriteChoreographer::should_change_pattern() {
	if (cfg[ConfigOptions::IsPatternFixed]) {
		return false;
	}

	// We must change the pattern every so often...
	// But not, of course, when we're just getting started.
	return m_ctx->frame_count() % (int) cfg[ConfigOptions::PatternChangeInterval] == 0 && m_ctx->frame_count() != 0;
}

void SpriteChoreographer::change_pattern() {
	m_pattern = (PatternName) (Noise::random() * _PATTERN_COUNT);
	update_player();
}

void SpriteChoreographer::update_player() {
	for (PatternPlayer *player : m_players) {
		if (player->compatible_patterns().find(m_pattern) != player->compatible_patterns().end()) {
			m_current_player = player;
			m_current_player->set_pattern(m_pattern);
		}
	}
}

void PatternPlayer::set_pattern(PatternName pattern) {
	m_pattern = pattern;
	m_hash_offset++;
}

PatternPlayer::PatternPlayer(Sprites *sprites, Context *ctx)
	: m_pattern(Roamers), m_sprites(sprites), m_ctx(ctx) { }

float PatternPlayer::hash(unsigned int n) {
	return ((n * n * 562448657) % 4096) / 4096.0f;
}

unsigned int PatternPlayer::m_hash_offset = 0;

SinglePassPlayer::SinglePassPlayer(Sprites *sprites, Context *ctx)
	: PatternPlayer(sprites, ctx) { }

void SinglePassPlayer::update() {
	for (Sprite *sprite : *m_sprites) {
		move_functions[m_pattern](sprite, m_ctx, hash(sprite->id() + m_hash_offset));
		sprite->update(*m_ctx);
	}
}
 
std::set<PatternName> &SinglePassPlayer::compatible_patterns() {
	static std::set<PatternName> patterns = {
		Roamers,
		Waves,
		Square,
		Bouncy,
		Lissajous,
		Rose,
		Lattice,
	};

	return patterns;
}

std::map<PatternName, SinglePassPlayer::MoveFunction> SinglePassPlayer::move_functions {
	{ Roamers, [](Sprite* sprite, Context* ctx, float offset) {
		// Every pattern is made of three things!
		// The sprite, the creature who kindly participates -
		// The context, the timepiece by which we will calculate -
		// And the offset, by which our fate is encoded
		// One onto zero that chaos corroded.
		get<X>(sprite->home()) += offset / cfg[ConfigOptions::TimeDivisor];
		get<Y>(sprite->home()) += sin(ctx->t() * offset) / cfg[ConfigOptions::TimeDivisor];
	}},
	{ Waves, [](Sprite *sprite, Context *ctx, float offset) {
		get<X>(sprite->home()) += sin(ctx->t() * offset) / cfg[ConfigOptions::TimeDivisor];
		get<Y>(sprite->home()) += cos(ctx->t() * offset) / cfg[ConfigOptions::TimeDivisor];
	}},
	{ Square, [](Sprite *sprite, Context *ctx, float offset) {
		get<X>(sprite->home()) += offset < 0.5 ? ((1.0f - offset) / cfg[ConfigOptions::TimeDivisor]) : 0.0f;
		get<Y>(sprite->home()) += offset < 0.5 ? 0.0f : (offset / cfg[ConfigOptions::TimeDivisor]);
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

		get<X>(sprite->home()) += (offset / cfg[ConfigOptions::TimeDivisor]) * lateral_modifier;
		get<Y>(sprite->home()) += (1.0f - offset) / cfg[ConfigOptions::TimeDivisor] * vertical_modifier;

		if (get<X>(sprite->home()) > 1.0f || get<X>(sprite->home()) < -1.0f) {
			directions[sprite->id()] ^= West;
			get<X>(sprite->home()) = signbit(get<X>(sprite->home())) ? -1.0f : 1.0f;
		}

		if (get<Y>(sprite->home()) > 1.0f || get<Y>(sprite->home()) < -1.0f) {
			directions[sprite->id()] ^= South;
			get<Y>(sprite->home()) = signbit(get<Y>(sprite->home())) ? -1.0f : 1.0f;
		}
	}},
	{ Lissajous, [](Sprite *sprite, Context *ctx, float offset) {
		// Unlike the patterns you see above,
		// For this one, well, push comes to shove.
		// We know exactly where we must be,
		// So we won't let our sprites roam around freely...
		float target_x = sin(ctx->t() - (offset * 0.07f * cfg[ConfigOptions::SpriteCount])) * 0.8f;
		float target_y = cos(ctx->t() - (offset * 0.05f * cfg[ConfigOptions::SpriteCount])) * 0.8f;

		// But! To send them straight to their fate is unsightly,
		// So instead of assign, we just push ever lightly.
		get<X>(sprite->home()) = target_x + (get<X>(sprite->home()) - target_x) * 0.9f;
		get<Y>(sprite->home()) = target_y + (get<Y>(sprite->home()) - target_y) * 0.9f;
	}},
	{ Rose, [](Sprite *sprite, Context *ctx, float offset) {
		float t = ctx->t() - (offset * 0.03f * cfg[ConfigOptions::SpriteCount]);
		float r = 0.04f * cfg[ConfigOptions::SpriteCount] * t;

		float target_x = sin(r) * cos(t) * 0.8f;
		float target_y = sin(r) * sin(t) * 0.8f;

		get<X>(sprite->home()) = target_x + (get<X>(sprite->home()) - target_x) * 0.9f;
		get<Y>(sprite->home()) = target_y + (get<Y>(sprite->home()) - target_y) * 0.9f;
	}},
	{ Lattice, [](Sprite *_sprite, Context *_ctx, float _offset) {
		// The flocking of birds, the schooling of fish,
		// The dancing of insects with a firefly's wish...
		// There's beauty in movement, I must agree,
		// But beauty in stillness, I also can see.
	}}
};

GlobalPlayer::GlobalPlayer(Sprites *sprites, Context *ctx)
	: PatternPlayer(sprites, ctx) { }

void GlobalPlayer::update() {
	move_functions.at(m_pattern)(m_sprites, m_ctx, [&](Id id) -> float { return hash(id + m_hash_offset); });

	for (Sprite *sprite : *m_sprites) {
		sprite->update(*m_ctx);
	}
}

std::set<PatternName> &GlobalPlayer::compatible_patterns() {
	static std::set<PatternName> patterns = {
		Bubbles
	};

	return patterns;
}

std::map<PatternName, GlobalPlayer::MoveFunction> GlobalPlayer::move_functions {
	{ Bubbles, [](Sprites *sprites, Context *ctx, std::function<float(Id)> get_offset) {
		const static float SCREEN_SIZE = ctx->rect().bottom * ctx->rect().right;
		const static float STRETCH_RATIO = (float) (ctx->rect().bottom) / ctx->rect().right;
		const static float BUBBLE_Y_RADIUS = (10.0f / (cfg[ConfigOptions::SpriteCount] / 1.5f + 40.0f)) * std::powf(SCREEN_SIZE / (1080 * 1920) / 3.0f + 0.7f, 1.1f);
		const static float BUBBLE_X_RADIUS = BUBBLE_Y_RADIUS * STRETCH_RATIO;

		static std::map<Id, Point> velocity;

		if (velocity.empty()) {
			for (const Sprite *sprite : *sprites) {
				float radians = Noise::random() * M_PI * 2;
				float mag = Noise::random() + 0.4f;
				velocity[sprite->id()] = { std::cosf(radians) * mag, std::sinf(radians) * mag };
			}
		}

		std::vector<std::pair<Sprite *, Sprite *>> collisions;
		for (size_t i = 0; i < sprites->size(); i++) {
			for (size_t j = 0; j < sprites->size(); j++) {
				if (i == j) {
					continue;
				}

				Sprite *a = (*sprites)[i];
				Sprite *b = (*sprites)[j];

				float dist_x = a->final<X>() - b->final<X>();
				float dist_y = (a->final<Y>() - b->final<Y>()) * STRETCH_RATIO;
				float dist = std::sqrt(dist_x * dist_x + dist_y * dist_y);

				if (dist < BUBBLE_X_RADIUS) {
					collisions.push_back({ a, b });
				}
			}
		}

		for (const auto &collision : collisions) {
			Sprite *a = collision.first;
			Sprite *b = collision.second;

			Point L = { -get<X>(velocity[a->id()]), -get<Y>(velocity[a->id()]) };
			float mag_L = std::sqrt(get<X>(L) * get<X>(L) + get<Y>(L) * get<Y>(L));
			Point L_u = { get<X>(L) / mag_L, get<Y>(L) / mag_L };

			Point N = { a->final<X>() - b->final<X>(), a->final<Y>() - b->final<Y>() };
			float mag_N = std::sqrt(get<X>(N) * get<X>(N) + get<Y>(N) * get<Y>(N));
			get<X>(N) /= mag_N;
			get<Y>(N) /= mag_N;

			float cos_theta = get<X>(L_u) * get<X>(N) + get<Y>(L_u) * get<Y>(N);
			
			if (cos_theta > 0) {
				cos_theta *= std::signbit(get<X>(L) * get<Y>(N) - get<Y>(L) * get<X>(N)) ? -1.0f : 1.0f;

				float cos_theta_sq = cos_theta * cos_theta;
				float cos_2theta = 2 * cos_theta_sq - 1;

				float sin_theta = std::sqrt(1 - cos_theta_sq);
				float sin_2theta = (sin_theta + cos_theta) * (sin_theta + cos_theta) - 1;

				float Rx = get<X>(L) * cos_2theta - get<Y>(L) * sin_2theta;
				float Ry = get<X>(L) * sin_2theta + get<Y>(L) * cos_2theta;

				get<X>(velocity[a->id()]) = Rx;
				get<Y>(velocity[a->id()]) = Ry;
			}
		}

		for (Sprite *sprite : *sprites) {
			get<X>(sprite->home()) += get<X>(velocity[sprite->id()]) / cfg[ConfigOptions::TimeDivisor] * 0.5f;
			get<Y>(sprite->home()) += get<Y>(velocity[sprite->id()]) / cfg[ConfigOptions::TimeDivisor] / STRETCH_RATIO * 0.5f;

			glBindTexture(GL_TEXTURE_2D, 0);
			glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 20; i++) {
				float theta = 2.0f * M_PI * i / 20.0f;
				float x = BUBBLE_X_RADIUS / 2 * std::cosf(theta);
				float y = BUBBLE_Y_RADIUS / 2 * std::sinf(theta);
				glVertex2f(x + sprite->final<X>(), y + sprite->final<Y>());
			}
			glEnd();
		}
	}},
};
