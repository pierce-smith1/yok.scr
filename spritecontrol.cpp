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
		for (int i = cast<int>(Cfg::MaxColors.range.first); i < Cfg::MaxColors.range.second; i++) {
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
		? cast<int>(bag_of_palettes.size())
		: cast<int>(Cfg::MaxColors.range.first);
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
	: m_pattern(choreography), m_ctx(ctx), m_sprites(sprites), m_enabled_patterns(PatternRepository::load_enabled_patterns())
{
	m_players = { new SinglePassPlayer(sprites, ctx), new GlobalPlayer(sprites, ctx) };
	if (m_pattern == RandomPattern) {
		change_pattern();
	}
	update_player();
}

void SpriteChoreographer::update() {
	m_current_player->update();
	if (non_screen_wrapping_patterns.contains(m_pattern)) {
		m_current_player->clamp_off_screen_sprites();
	} else {
		m_current_player->wrap_off_screen_sprites();
	}
	if (should_change_pattern()) {
		change_pattern();
	}
}

bool SpriteChoreographer::should_change_pattern() {
	if (m_enabled_patterns.size() <= 1) {
		return false;
	}

	// We must change the pattern every so often...
	// But not, of course, when we're just getting started.
	return m_ctx->frame_count() % (int) cfg[Cfg::PatternChangeInterval] == 0 && m_ctx->frame_count() != 0;
}

void SpriteChoreographer::change_pattern() {
	std::vector<PatternName> candidate_patterns = { };
	std::copy_if(m_enabled_patterns.begin(), m_enabled_patterns.end(), std::back_inserter(candidate_patterns), [&](const PatternName &pattern) {
		return pattern != m_pattern;
	});

	size_t random_index = (size_t) (Noise::random() * candidate_patterns.size());
	m_pattern = candidate_patterns.at(random_index);
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

void PatternPlayer::wrap_off_screen_sprites() {
	auto wrap = [](double home, double total, double min, double max) -> double {
		if (total < min) {
			return home + (max - min);
		} else if (total > max) {
			return home + (min - max);
		} else {
			return home;
		}
	};

	double edge_boundary = 0.15 + Sprite::get_size() / 1.1;
	double horizontal_correction = max((double) m_ctx->rect().right / (double) m_ctx->rect().bottom, 1.0);
	double vertical_correction = max((double) m_ctx->rect().bottom / (double) m_ctx->rect().right, 1.0);

	for (Sprite *sprite : *m_sprites) {
		get<X>(sprite->home()) = wrap(get<X>(sprite->home()), sprite->final<X>(), -1.0 - (edge_boundary / horizontal_correction), 1.0 + (edge_boundary / horizontal_correction));
		get<Y>(sprite->home()) = wrap(get<Y>(sprite->home()), sprite->final<Y>(), -1.0 - (edge_boundary / vertical_correction), 1.0 + (edge_boundary / vertical_correction));
	}
}

void PatternPlayer::clamp_off_screen_sprites() {
	auto keep_in_bounds = [](double home, double total, double min, double max) -> double {
		if (total < min) {
			return home + (min - total);
		} else if (total > max) {
			return home + (max - total);
		} else {
			return home;
		}
	};

	double edge_boundary = 0.15 + Sprite::get_size() / 1.1;
	double horizontal_correction = max((double) m_ctx->rect().right / (double) m_ctx->rect().bottom, 1.0);
	double vertical_correction = max((double) m_ctx->rect().bottom / (double) m_ctx->rect().right, 1.0);

	// Still keep them within bounds if wrapping is not allowed. Should help prevent teleporting on screen when the pattern changes.
	// Some patterns will be fighting against this, but since it's happening off screen and after pattern movement, it shouldn't matter.
	for (Sprite *sprite : *m_sprites) {
		get<X>(sprite->home()) = keep_in_bounds(get<X>(sprite->home()), sprite->final<X>(), -1.0 - (edge_boundary / horizontal_correction), 1.0 + (edge_boundary / horizontal_correction));
		get<Y>(sprite->home()) = keep_in_bounds(get<Y>(sprite->home()), sprite->final<Y>(), -1.0 - (edge_boundary / vertical_correction), 1.0 + (edge_boundary / vertical_correction));
	}
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
	{ Roamers, [](Sprite *sprite, Context *ctx, double offset) {
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
		get<Y>(sprite->home()) += offset < 0.5 ? 0.0 : (offset / cfg[Cfg::TimeDivisor]);
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

		if (sprite->final<X>() > 1.0) {
			directions[sprite->id()] |= West;
		} else if (sprite->final<X>() < -1.0) {
			directions[sprite->id()] &= ~West;
		}

		if (sprite->final<Y>() > 1.0) {
			directions[sprite->id()] |= South;
		} else if (sprite->final<Y>() < -1.0) {
			directions[sprite->id()] &= ~South;
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
		Bubbles,
		Boids,
	};

	return patterns;
}

std::map<PatternName, GlobalPlayer::MoveFunction> GlobalPlayer::move_functions {
	{ Bubbles, [](Sprites *sprites, Context *ctx, std::function<double(Id)> get_offset) {
		const static double SCREEN_SIZE = (double) ((long long) ctx->rect().bottom * ctx->rect().right);
		const static double STRETCH_RATIO = (double) (ctx->rect().bottom) / ctx->rect().right;
		const static double BUBBLE_Y_RADIUS = (10.0 / (cfg[Cfg::SpriteCount] / 1.5 + 40.0)) * std::pow(SCREEN_SIZE / (1080LL * 1920LL) / 3.0 + 0.7, 1.1);
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
	{ Boids, [](Sprites *sprites, Context *ctx, std::function<double(Id)> get_offset) {
		const static double SCREEN_SIZE = (double) ((long long) ctx->rect().bottom * ctx->rect().right);
		const static double STRETCH_RATIO = (double) (ctx->rect().bottom) / ctx->rect().right;
		const static double SEPARATION_Y_RADIUS = (6.0 / (cfg[Cfg::SpriteCount] / 2.5 + 40.0)) * std::pow(SCREEN_SIZE / (1080LL * 1920LL) / 3.0 + 0.7, 1.1);
		const static double SEPARATION_X_RADIUS = SEPARATION_Y_RADIUS * STRETCH_RATIO;
		const static double VISION_Y_RADIUS = (10.0 / (cfg[Cfg::SpriteCount] / 8.0 + 15.0)) * std::pow(SCREEN_SIZE / (1080LL * 1920LL) / 3.0 + 0.7, 1.1);
		const static double VISION_X_RADIUS = VISION_Y_RADIUS * STRETCH_RATIO;
		const static double BLIND_RADIANS = 45.0 * M_PI / 180.0;

		const static double DEFAULT_FORCE_MULT = 0.1;	// Multiplier for all forces below
		const static double SEPARATION_FORCE_MULT = DEFAULT_FORCE_MULT * 1.0;			// How strongly to separate sprites that are too close
		const static double ALIGNMENT_FORCE_MULT = DEFAULT_FORCE_MULT * 1.2;			// How strongly to align sprites that are in a pack
		const static double COHESION_FORCE_MULT = DEFAULT_FORCE_MULT * 0.5;				// How strongly to pull sprites towards the middle of their pack
		const static double DESIRED_VELOCITY_RETURN_MULT = DEFAULT_FORCE_MULT * 0.3;	// How strongly to accelerate sprites towards their desired velocity

		static std::map<Id, Point> velocity;
		static std::map<Id, double> desired_speed;
		static std::map<Id, double> desired_separation;

		// debug shit
		static size_t random_sprite = (size_t) (Noise::random() * sprites->size());
		Point random_average_velocity;
		Point random_average_position;

		// Gives a random value between (1 - negative_variation; 1 + positive_variation).
		// Exponent affects the bias of the curve towards 1 before rapidly diverging at the edges.
		// Slope affects how linear the curve is. Slope = 1 behaves like exponent = 1.
		// Variation shouldn't result in a number below 0. Exponent must be larger than 0. Slope should be from 0 to 1.
		auto random_curve = [](double negative_variation, double positive_variation, double exponent = 5.0, double slope = 0.1) {
			double random = Noise::random();
			double sign = Noise::random() < 0.5 ? -1.0 : 1.0;
			double variation = sign < 0 ? negative_variation : positive_variation;
			random = random * slope + pow(random, exponent) * (1 - slope);
			return 1.0 + sign * random * variation;
			// Equation: 1 + sign(rand) * (|rand| * slope + |rand|^exp * (1 - slope)) * (sign(rand) < 0 ? negative_variation : positive_variation)
		};

		// Converts a vector to an angle in the range (-M_PI, M_PI].
		auto get_angle = [](const Point &vector) {
			const auto &[x, y] = vector;

			if (x == 0 && y == 0) {
				return 0.0;
			}

			double angle = atan2(y, x);
			return angle;
		};

		// Wraps angles to be inside the range (-M_PI, M_PI]
		auto wrap_angle = [](double angle) {
			while (angle > M_PI) {
				angle -= 2.0 * M_PI;
			}
			while (angle < -M_PI) {
				angle += 2.0 * M_PI;
			}
			return angle;
		};

		auto get_vector_magnitude = [](const Point &vector) {
			const auto &[x, y] = vector;

			return sqrt(x * x + y * y);
		};

		auto normalize_vector = [&](const Point &vector) {
			double magnitude = get_vector_magnitude(vector);
			return magnitude != 0.0 ? vector / magnitude : vector;
		};

		if (velocity.empty() || desired_speed.empty() || desired_separation.empty()) {
			for (const Sprite *sprite : *sprites) {
				double magnitude = Noise::random() + 0.8;
				double radians = Noise::random() * M_PI * 2;
				velocity[sprite->id()] = Point(std::cos(radians) * magnitude, std::sin(radians) * magnitude);
				desired_speed[sprite->id()] = magnitude;

				double separation_radius = random_curve(0.2, 0.2, 2.5) * SEPARATION_X_RADIUS;
				desired_separation[sprite->id()] = separation_radius;
			}
		}

		std::map<Sprite *, Point> separation_velocity_changes;
		std::map<Sprite *, Point> alignment_velocity_changes;
		std::map<Sprite *, Point> cohesion_velocity_changes;

		size_t sprite_num = 0;
		for (Sprite *current_sprite : *sprites) {
			const Point &current_velocity = velocity[current_sprite->id()];
			const double &current_desired_separation = desired_separation[current_sprite->id()];

			Point current_final = Point(current_sprite->final<X>(), current_sprite->final<Y>());

			Point separation_velocity = Point(0.0, 0.0);
			size_t sprites_seen = 1;
			Point average_velocity = Point(current_velocity);
			Point average_pos = Point(current_final);

			for (Sprite *other_sprite : *sprites) {		// dejil... i am sorry...
				if (current_sprite == other_sprite) {
					continue;
				}

				Point other_final = Point(other_sprite->final<X>(), other_sprite->final<Y>());
				Point diff = current_final - other_final;
				get<Y>(diff) *= STRETCH_RATIO;

				double dist = get_vector_magnitude(diff);

				if (dist > VISION_X_RADIUS) {
					continue;
				}

				double current_angle = get_angle(current_velocity);
				double relative_angle = get_angle(diff);
				relative_angle = wrap_angle(relative_angle - current_angle);	// 180 deg: straight ahead; 0 deg: straight behind (assuming i'm not bad at math)

				if (abs(relative_angle) >= BLIND_RADIANS) {
					sprites_seen++;
					Point other_velocity = velocity[other_sprite->id()];
					average_velocity += other_velocity;
					average_pos += other_final;
				}

				if (dist < current_desired_separation) {
					diff /= dist * dist / current_desired_separation;
					separation_velocity += diff;
				}
			}

			separation_velocity_changes[current_sprite] = separation_velocity;

			if (sprites_seen > 1) {
				average_velocity /= (double) sprites_seen;
				average_pos /= (double) sprites_seen;

				alignment_velocity_changes[current_sprite] = average_velocity;

				Point relative_average_pos = average_pos - Point(current_sprite->final<X>(), current_sprite->final<Y>());
				if (sprite_num == random_sprite) {
					random_average_position = relative_average_pos;
				}
				cohesion_velocity_changes[current_sprite] = normalize_vector(relative_average_pos);
			}

			if (sprite_num == random_sprite) {
				if (alignment_velocity_changes.contains(current_sprite)) {
					random_average_velocity = alignment_velocity_changes[current_sprite];
				} else {
					random_average_velocity = Point(current_velocity);
				}
				random_average_velocity /= cfg[Cfg::TimeDivisor] / 30.0;

				if (!cohesion_velocity_changes.contains(current_sprite)) {
					random_average_position = Point(0, 0);
				}
			}

			sprite_num++;
		}

		for (auto &[sprite, velocity_change] : separation_velocity_changes) {
			double scale = get_vector_magnitude(velocity_change);
			scale = min(desired_speed[sprite->id()] / scale, 1.0) * SEPARATION_FORCE_MULT;
			velocity[sprite->id()] += velocity_change * scale;
		}

		for (auto &[sprite, velocity_change] : alignment_velocity_changes) {
			Point &sprite_velocity = velocity[sprite->id()];
			double magnitude = get_vector_magnitude(sprite_velocity);
			sprite_velocity /= magnitude;

			Point diff = velocity_change - sprite_velocity;
			diff *= ALIGNMENT_FORCE_MULT;

			sprite_velocity = normalize_vector(sprite_velocity + diff) * magnitude;
			// there's probably a better way to do this but i can't be bothered figuring it out now
		}

		for (auto &[sprite, velocity_change] : cohesion_velocity_changes) {
			Point &sprite_velocity = velocity[sprite->id()];
			double magnitude = get_vector_magnitude(sprite_velocity);
			sprite_velocity /= magnitude;

			Point diff = velocity_change - sprite_velocity;
			diff *= COHESION_FORCE_MULT;

			sprite_velocity = normalize_vector(sprite_velocity + diff) * magnitude;
			// refer to previous comment
		}

		sprite_num = 0;
		for (Sprite *sprite : *sprites) {
			double magnitude = get_vector_magnitude(velocity[sprite->id()]);
			double velocity_change = (desired_speed[sprite->id()] - magnitude) * DESIRED_VELOCITY_RETURN_MULT;

			velocity[sprite->id()] += velocity[sprite->id()] / magnitude * velocity_change;

			get<X>(sprite->home()) += get<X>(velocity[sprite->id()]) / cfg[Cfg::TimeDivisor] * 0.5;
			get<Y>(sprite->home()) += get<Y>(velocity[sprite->id()]) / cfg[Cfg::TimeDivisor] / STRETCH_RATIO * 0.5;

			glBindTexture(GL_TEXTURE_2D, 0);
			glColor4d(0.2, 0.2, 0.2, 1.0);

			// temporary visuals
			if (sprite_num == random_sprite) {
				glColor4d(0.5, 0.5, 0.5, 1.0);

				glBegin(GL_LINE_LOOP);
				for (int i = 0; i < 360; i++) {
					double theta = 2.0 * M_PI * i / 360.0;
					double x = 0;
					double y = 0;
					if (abs(theta - M_PI) >= BLIND_RADIANS) {
						x = VISION_X_RADIUS * std::cos(theta + get_angle(velocity[sprite->id()]));
						y = VISION_X_RADIUS / STRETCH_RATIO * std::sin(theta + get_angle(velocity[sprite->id()]));
					}
					glVertex2d(x + sprite->final<X>(), y + sprite->final<Y>());
				}
				glEnd();

				glBegin(GL_LINES);
				glColor4d(0.8, 0.2, 0.2, 1.0);	// red: average velocity
				glVertex2d(sprite->final<X>(), sprite->final<Y>());
				glVertex2d(get<X>(random_average_velocity) + sprite->final<X>(), get<Y>(random_average_velocity) + sprite->final<Y>());

				glColor4d(0.2, 0.5, 0.8, 1.0);	// blue: average position
				glVertex2d(sprite->final<X>(), sprite->final<Y>());
				glVertex2d(get<X>(random_average_position) + sprite->final<X>(), get<Y>(random_average_position) + sprite->final<Y>());
				glEnd();

				glColor4d(0.2, 0.2, 0.2, 1.0);
			}
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 20; i++) {
				double theta = 2.0 * M_PI * i / 20.0;
				double x = desired_separation[sprite->id()] / 2.0 * std::cos(theta);
				double y = desired_separation[sprite->id()] / 2.0 / STRETCH_RATIO * std::sin(theta);
				glVertex2d(x + sprite->final<X>(), y + sprite->final<Y>());
			}
			glEnd();

			sprite_num++;
		}
	}},
};

std::vector<PatternName> PatternRepository::load_disabled_patterns() {
	Registry registry;

	std::vector<PatternName> disabled_patterns = { };

	std::vector<std::wstring> disabled_patterns_strings = split<std::wstring>(registry.get_string(disabled_patterns_name, disabled_patterns_default), disabled_patterns_string_delimiter);
	for (auto &pattern : pattern_strings) {
		auto disabled_pattern = std::find_if(disabled_patterns_strings.begin(), disabled_patterns_strings.end(), [&](const std::wstring &string) {
			return string == pattern.second && pattern.first != RandomPattern;
		});
		if (disabled_pattern != disabled_patterns_strings.end()) {
			disabled_patterns.push_back(pattern.first);
		}
	}

	if (get_enabled_patterns(disabled_patterns).size() < 1) {
		disabled_patterns.erase(std::find(disabled_patterns.begin(), disabled_patterns.end(), default_pattern_all_disabled));
	}

	return disabled_patterns;
}

std::vector<PatternName> PatternRepository::get_enabled_patterns(const std::vector<PatternName> &disabled_patterns) {
	std::vector<PatternName> enabled_patterns = { };

	for (auto &pattern : pattern_strings) {
		if (std::find(disabled_patterns.begin(), disabled_patterns.end(), pattern.first) == disabled_patterns.end() && pattern.first != RandomPattern) {
			enabled_patterns.push_back(pattern.first);
		}
	}

	return enabled_patterns;
}

std::vector<PatternName> PatternRepository::load_enabled_patterns() {
	return get_enabled_patterns(load_disabled_patterns());
}

void PatternRepository::save_disabled_patterns(const std::vector<PatternName> &disabled_patterns) {
	std::wstring disabled_patterns_string = disabled_patterns_default;
	for (auto &pattern : disabled_patterns) {
		if (pattern != *disabled_patterns.begin()) {
			disabled_patterns_string.append(disabled_patterns_string_delimiter);
		}
		disabled_patterns_string.append(pattern_strings.at(pattern));
	}

	Registry registry;
	registry.write_string(disabled_patterns_name, disabled_patterns_string);
}
