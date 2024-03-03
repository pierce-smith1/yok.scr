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

	PaletteGroup palette_group = (PaletteGroup) (cfg.at(YonkPalette));

	std::vector<const Palette *> bag_of_palettes;
	if (palette_group == PaletteGroup::RandomlyGenerated) {
		for (int i = (int) config_ranges.at(MaxColors).first; i < (int) config_ranges.at(MaxColors).second; i++) {
			bag_of_palettes.push_back(RandomPalettes::random(i));
		}
	} else {
		auto palette_names = PALETTES_BY_GROUP.at(palette_group);
		for (auto name : palette_names) {
			bag_of_palettes.push_back(&PALETTES.at(name));
		}
	}

	int max_colors = (int)round(cfg.at(MaxColors) * bag_of_palettes.size() / config_ranges.at(MaxColors).second);
	max_colors = std::clamp(max_colors, (int)config_ranges.at(MaxColors).first, (int)bag_of_palettes.size());

	for (int i = 0; i < max_colors; i++) {
		size_t random_palette_index = std::rand() % bag_of_palettes.size();
		m_palettes.push_back(bag_of_palettes[random_palette_index]);
		bag_of_palettes.erase(bag_of_palettes.begin() + random_palette_index);
	}
}

std::vector<Sprite *> SpriteGenerator::make(unsigned int n) const {
	Sprites sprites;

	for (float y = -1.2f; y < 1.2f; y += 1.0f / sqrt(cfg.at(SpriteCount))) {
		for (float x = -1.2f; x < 1.2f; x += 1.0f / sqrt(cfg.at(SpriteCount))) {
			if (Noise::random() < pow(cfg.at(ImpostorChance), 3)) {
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
			if (Noise::random() < 1.8f / cfg.at(MaxColors)) {
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
	if (cfg.at(IsPatternFixed)) {
		return false;
	}

	// We must change the pattern every so often...
	// But not, of course, when we're just getting started.
	return m_ctx->frame_count() % (int) cfg.at(PatternChangeInterval) == 0 && m_ctx->frame_count() != 0;
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
		sprite->home().x += offset / cfg.at(TimeDivisor);
		sprite->home().y += sin(ctx->t() * offset) / cfg.at(TimeDivisor);
	}},
	{ Waves, [](Sprite *sprite, Context *ctx, float offset) {
		sprite->home().x += sin(ctx->t() * offset) / cfg.at(TimeDivisor);
		sprite->home().y += cos(ctx->t() * offset) / cfg.at(TimeDivisor);
	}},
	{ Square, [](Sprite *sprite, Context *ctx, float offset) {
		sprite->home().x += offset < 0.5 ? ((1.0f - offset) / cfg.at(TimeDivisor)) : 0.0f;
		sprite->home().y += offset < 0.5 ? 0.0f : (offset / cfg.at(TimeDivisor));
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

		sprite->home().x += (offset / cfg.at(TimeDivisor)) * lateral_modifier;
		sprite->home().y += (1.0f - offset) / cfg.at(TimeDivisor) * vertical_modifier;

		if (sprite->home().x > 1.0f || sprite->home().x < -1.0f) {
			directions[sprite->id()] ^= West;
			sprite->home().x = signbit(sprite->home().x) ? -1.0f : 1.0f;
		}

		if (sprite->home().y > 1.0f || sprite->home().y < -1.0f) {
			directions[sprite->id()] ^= South;
			sprite->home().y = signbit(sprite->home().y) ? -1.0f : 1.0f;
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
		sprite->home().x = target_x + (sprite->home().x - target_x) * 0.9f;
		sprite->home().y = target_y + (sprite->home().y - target_y) * 0.9f;
	}},
	{ Rose, [](Sprite *sprite, Context *ctx, float offset) {
		float t = ctx->t() - (offset * 0.03f * cfg.at(SpriteCount));
		float r = 0.04f * cfg.at(SpriteCount) * t;

		float target_x = sin(r) * cos(t) * 0.8f;
		float target_y = sin(r) * sin(t) * 0.8f;

		sprite->home().x = target_x + (sprite->home().x - target_x) * 0.9f;
		sprite->home().y = target_y + (sprite->home().y - target_y) * 0.9f;
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
		const static float BUBBLE_Y_RADIUS = (10.0f / (cfg.at(SpriteCount) / 1.5f + 40.0f)) * std::powf(SCREEN_SIZE / (1080 * 1920) / 3.0f + 0.7f, 1.1f);
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

				float dist_x = a->final_x() - b->final_x();
				float dist_y = (a->final_y() - b->final_y()) * STRETCH_RATIO;
				float dist = std::sqrt(dist_x * dist_x + dist_y * dist_y);

				if (dist < BUBBLE_X_RADIUS) {
					collisions.push_back({ a, b });
				}
			}
		}

		for (const auto &collision : collisions) {
			Sprite *a = collision.first;
			Sprite *b = collision.second;

			Point L = { -velocity[a->id()].x, -velocity[a->id()].y };
			float mag_L = std::sqrt(L.x * L.x + L.y * L.y);
			Point L_u = { L.x / mag_L, L.y / mag_L };

			Point N = { a->final_x() - b->final_x(), a->final_y() - b->final_y() };
			float mag_N = std::sqrt(N.x * N.x + N.y * N.y);
			N.x /= mag_N;
			N.y /= mag_N;

			float cos_theta = L_u.x * N.x + L_u.y * N.y;
			
			if (cos_theta > 0) {
				cos_theta *= std::signbit(L.x * N.y - L.y * N.x) ? -1.0f : 1.0f;

				float cos_theta_sq = cos_theta * cos_theta;
				float cos_2theta = 2 * cos_theta_sq - 1;

				float sin_theta = std::sqrt(1 - cos_theta_sq);
				float sin_2theta = (sin_theta + cos_theta) * (sin_theta + cos_theta) - 1;

				float Rx = L.x * cos_2theta - L.y * sin_2theta;
				float Ry = L.x * sin_2theta + L.y * cos_2theta;

				velocity[a->id()].x = Rx;
				velocity[a->id()].y = Ry;
			}
		}

		for (Sprite *sprite : *sprites) {
			sprite->home().x += velocity[sprite->id()].x / cfg.at(TimeDivisor) * 0.5f;
			sprite->home().y += velocity[sprite->id()].y / cfg.at(TimeDivisor) / STRETCH_RATIO * 0.5f;

			glBindTexture(GL_TEXTURE_2D, 0);
			glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 20; i++) {
				float theta = 2.0f * M_PI * i / 20.0f;
				float x = BUBBLE_X_RADIUS / 2 * std::cosf(theta);
				float y = BUBBLE_Y_RADIUS / 2 * std::sinf(theta);
				glVertex2f(x + sprite->final_x(), y + sprite->final_y());
			}
			glEnd();
		}
	}},
};
