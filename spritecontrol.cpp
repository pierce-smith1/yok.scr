#include "spritecontrol.h"
#include "bitmaps.h"
#include "config.h"
#include "noise.h"
#include "configdialog.h"

#include <math.h>

using std::get;

SpriteGenerator::SpriteGenerator() {
	using namespace std::chrono;
	auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	std::srand(cast<unsigned int>(now));

	PaletteGroup palette_group = (PaletteGroup) (cfg[Cfg::Palette]);

	std::vector<Palettes::Definition> bag_of_palettes;
	if (cfg[Cfg::UseCustomPalettes] == 1.0f) {
		bag_of_palettes = PaletteRepository().get_all_custom_palettes();
	} else if (palette_group == PaletteGroup::RandomlyGenerated) {
		for (int i = Cfg::MaxColors.range.first; i < Cfg::MaxColors.range.second; i++) {
			bag_of_palettes.push_back(RandomPalettes::random(i));
		}
	} else {
		auto group = PaletteGroups::get(palette_group);
		bag_of_palettes = group.members;
	}

	if (bag_of_palettes.empty()) {
		bag_of_palettes = PaletteGroups::All.members;
	}

	int min_colors = bag_of_palettes.size() < Cfg::MaxColors.range.first
		? bag_of_palettes.size()
		: Cfg::MaxColors.range.first;
	int max_colors = (int) round(cfg[Cfg::MaxColors] * bag_of_palettes.size() / Cfg::MaxColors.range.second);
	max_colors = std::clamp(max_colors, min_colors, (int) bag_of_palettes.size());

	for (int i = 0; i < max_colors; i++) {
		size_t random_palette_index = std::rand() % bag_of_palettes.size();
		m_palettes.push_back(bag_of_palettes[random_palette_index].data);
		bag_of_palettes.erase(bag_of_palettes.begin() + random_palette_index);
	}
}

std::vector<Sprite *> SpriteGenerator::make(unsigned int n) const {
	Sprites sprites;

	for (double y = -1.2; y < 1.2; y += 1.0 / sqrt(cfg[Cfg::SpriteCount])) {
		for (double x = -1.2; x < 1.2; x += 1.0 / sqrt(cfg[Cfg::SpriteCount])) {
			if (Noise::random() < pow(cfg[Cfg::ImpostorChance], 3)) {
				sprites.push_back(new Impostor(next_palette(), Point(x, y)));
			} else {
				sprites.push_back(new Yonker(next_texture(), { x, y }));
			}
		}
	}

	return sprites;
}

const Texture *SpriteGenerator::next_texture() const {
	return Texture::of(next_palette(), Bitmaps::Lk);
}

const PaletteData *SpriteGenerator::next_palette() const {
	// A weighted coin?! Now that's certainly cheating!
	// You'd be kicked outta Vegas for logarithmic repeating.
	while (true) {
		for (auto palette : m_palettes) {
			if (Noise::random() < 1.8 / cfg[Cfg::MaxColors]) {
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
	if (cfg[Cfg::IsPatternFixed]) {
		return false;
	}

	// We must change the pattern every so often...
	// But not, of course, when we're just getting started.
	return m_ctx->frame_count() % (int) cfg[Cfg::PatternChangeInterval] == 0 && m_ctx->frame_count() != 0;
}

void SpriteChoreographer::change_pattern() {
	m_pattern = (PatternName) (Noise::random() * cast<double>(_PATTERN_COUNT));
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

double PatternPlayer::hash(unsigned int n) {
	return ((n * n * 562448657) % 4096) / 4096.0;
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
	{ Roamers, [](Sprite* sprite, Context* ctx, double offset) {
		// Every pattern is made of three things!
		// The sprite, the creature who kindly participates -
		// The context, the timepiece by which we will calculate -
		// And the offset, by which our fate is encoded
		// One onto zero that chaos corroded.
		get<X>(sprite->home()) += offset / cfg[Cfg::TimeDivisor];
		get<Y>(sprite->home()) += sin(ctx->t() * offset) / cfg[Cfg::TimeDivisor];
	}},
	{ Waves, [](Sprite *sprite, Context *ctx, double offset) {
		get<X>(sprite->home()) += sin(ctx->t() * offset) / cfg[Cfg::TimeDivisor];
		get<Y>(sprite->home()) += cos(ctx->t() * offset) / cfg[Cfg::TimeDivisor];
	}},
	{ Square, [](Sprite *sprite, Context *ctx, double offset) {
		get<X>(sprite->home()) += offset < 0.5 ? ((1.0 - offset) / cfg[Cfg::TimeDivisor]) : 0.0;
		get<Y>(sprite->home()) += offset < 0.5 ? 0.0: (offset / cfg[Cfg::TimeDivisor]);
	}},
	{ Bouncy, [](Sprite *sprite, Context *ctx, double offset) {
		static int NorthWest = 0b01;
		static int NorthEast = 0b00;
		static int SouthEast = 0b10;
		static int SouthWest = 0b11;
		static int West = 0b1;
		static int South = 0b10;

		static std::map<Id, int> directions;

		double lateral_modifier = (directions[sprite->id()] & West) ? -1.0 : 1.0;
		double vertical_modifier = (directions[sprite->id()] & South) ? -1.0 : 1.0;

		get<X>(sprite->home()) += (offset / cfg[Cfg::TimeDivisor]) * lateral_modifier;
		get<Y>(sprite->home()) += (1.0 - offset) / cfg[Cfg::TimeDivisor] * vertical_modifier;

		if (get<X>(sprite->home()) > 1.0 || get<X>(sprite->home()) < -1.0) {
			directions[sprite->id()] ^= West;
			get<X>(sprite->home()) = signbit(get<X>(sprite->home())) ? -1.0 : 1.0;
		}

		if (get<Y>(sprite->home()) > 1.0 || get<Y>(sprite->home()) < -1.0) {
			directions[sprite->id()] ^= South;
			get<Y>(sprite->home()) = signbit(get<Y>(sprite->home())) ? -1.0 : 1.0;
		}
	}},
	{ Lissajous, [](Sprite *sprite, Context *ctx, double offset) {
		// Unlike the patterns you see above,
		// For this one, well, push comes to shove.
		// We know exactly where we must be,
		// So we won't let our sprites roam around freely...
		double target_x = sin(ctx->t() - (offset * 0.07 * cfg[Cfg::SpriteCount])) * 0.8;
		double target_y = cos(ctx->t() - (offset * 0.05 * cfg[Cfg::SpriteCount])) * 0.8;

		// But! To send them straight to their fate is unsightly,
		// So instead of assign, we just push ever lightly.
		get<X>(sprite->home()) = target_x + (get<X>(sprite->home()) - target_x) * 0.9;
		get<Y>(sprite->home()) = target_y + (get<Y>(sprite->home()) - target_y) * 0.9;
	}},
	{ Rose, [](Sprite *sprite, Context *ctx, double offset) {
		double t = ctx->t() - (offset * 0.03 * cfg[Cfg::SpriteCount]);
		double r = 0.04 * cfg[Cfg::SpriteCount] * t;

		double target_x = sin(r) * cos(t) * 0.8;
		double target_y = sin(r) * sin(t) * 0.8;

		get<X>(sprite->home()) = target_x + (get<X>(sprite->home()) - target_x) * 0.9;
		get<Y>(sprite->home()) = target_y + (get<Y>(sprite->home()) - target_y) * 0.9;
	}},
	{ Lattice, [](Sprite *_sprite, Context *_ctx, double _offset) {
		// The flocking of birds, the schooling of fish,
		// The dancing of insects with a firefly's wish...
		// There's beauty in movement, I must agree,
		// But beauty in stillness, I also can see.
	}}
};

GlobalPlayer::GlobalPlayer(Sprites *sprites, Context *ctx)
	: PatternPlayer(sprites, ctx) { }

void GlobalPlayer::update() {
	move_functions.at(m_pattern)(m_sprites, m_ctx, [&](Id id) -> double { return hash(id + m_hash_offset); });

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
	{ Bubbles, [](Sprites *sprites, Context *ctx, std::function<double(Id)> get_offset) {
		const static double SCREEN_SIZE = ctx->rect().bottom * ctx->rect().right;
		const static double STRETCH_RATIO = (double) (ctx->rect().bottom) / ctx->rect().right;
		const static double BUBBLE_Y_RADIUS = (10.0 / (cfg[Cfg::SpriteCount] / 1.5 + 40.0)) * std::pow(SCREEN_SIZE / (1080 * 1920) / 3.0 + 0.7, 1.1);
		const static double BUBBLE_X_RADIUS = BUBBLE_Y_RADIUS * STRETCH_RATIO;

		static std::map<Id, Point> velocity;

		if (velocity.empty()) {
			for (const Sprite *sprite : *sprites) {
				double radians = Noise::random() * M_PI * 2;
				double mag = Noise::random() + 0.4;
				velocity[sprite->id()] = { std::cos(radians) * mag, std::sin(radians) * mag };
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

				double dist_x = a->final<X>() - b->final<X>();
				double dist_y = (a->final<Y>() - b->final<Y>()) * STRETCH_RATIO;
				double dist = std::sqrt(dist_x * dist_x + dist_y * dist_y);

				if (dist < BUBBLE_X_RADIUS) {
					collisions.push_back({ a, b });
				}
			}
		}

		for (const auto &collision : collisions) {
			Sprite *a = collision.first;
			Sprite *b = collision.second;

			Point L = { -get<X>(velocity[a->id()]), -get<Y>(velocity[a->id()]) };
			double mag_L = std::sqrt(get<X>(L) * get<X>(L) + get<Y>(L) * get<Y>(L));
			Point L_u = { get<X>(L) / mag_L, get<Y>(L) / mag_L };

			Point N = { a->final<X>() - b->final<X>(), a->final<Y>() - b->final<Y>() };
			double mag_N = std::sqrt(get<X>(N) * get<X>(N) + get<Y>(N) * get<Y>(N));
			get<X>(N) /= mag_N;
			get<Y>(N) /= mag_N;

			double cos_theta = get<X>(L_u) * get<X>(N) + get<Y>(L_u) * get<Y>(N);
			
			if (cos_theta > 0) {
				cos_theta *= std::signbit(get<X>(L) * get<Y>(N) - get<Y>(L) * get<X>(N)) ? -1.0 : 1.0;

				double cos_theta_sq = cos_theta * cos_theta;
				double cos_2theta = 2 * cos_theta_sq - 1;

				double sin_theta = std::sqrt(1 - cos_theta_sq);
				double sin_2theta = (sin_theta + cos_theta) * (sin_theta + cos_theta) - 1;

				double Rx = get<X>(L) * cos_2theta - get<Y>(L) * sin_2theta;
				double Ry = get<X>(L) * sin_2theta + get<Y>(L) * cos_2theta;

				get<X>(velocity[a->id()]) = Rx;
				get<Y>(velocity[a->id()]) = Ry;
			}
		}

		for (Sprite *sprite : *sprites) {
			get<X>(sprite->home()) += get<X>(velocity[sprite->id()]) / cfg[Cfg::TimeDivisor] * 0.5;
			get<Y>(sprite->home()) += get<Y>(velocity[sprite->id()]) / cfg[Cfg::TimeDivisor] / STRETCH_RATIO * 0.5;

			glBindTexture(GL_TEXTURE_2D, 0);
			glColor4d(0.2, 0.2, 0.2, 1.0);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 20; i++) {
				double theta = 2.0 * M_PI * i / 20.0;
				double x = BUBBLE_X_RADIUS / 2 * std::cos(theta);
				double y = BUBBLE_Y_RADIUS / 2 * std::sin(theta);
				glVertex2d(x + sprite->final<X>(), y + sprite->final<Y>());
			}
			glEnd();
		}
	}},
};
