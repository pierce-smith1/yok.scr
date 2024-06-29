#pragma once

#include <vector>
#include <functional>
#include <cmath>

#include "graphics.h"
#include "sprite.h"

const static double M_PI = std::acos(-1);

using Sprites = std::vector<Sprite *>;

enum PatternName {
	Roamers,
	Waves,
	Square,
	Bouncy,
	Lissajous,
	Rose,
	Lattice,
	Bubbles,
	_PATTERN_COUNT
};

const static std::set<PatternName> non_screen_wrapping_patterns = {
	Bouncy,
	Rose,
	Lissajous
};

class SpriteGenerator {
public:
	SpriteGenerator();

	Sprites make(unsigned int n) const;

private:
	const Texture *next_texture() const;
	const PaletteData *next_palette() const;

	std::vector<const PaletteData *> m_palettes;
};

class PatternPlayer {
public:
	void set_pattern(PatternName pattern);
	void wrap_off_screen_sprites();
	void clamp_off_screen_sprites();

	virtual void update() = 0;
	virtual std::set<PatternName> &compatible_patterns() = 0;

protected:
	PatternPlayer(Sprites *sprites, Context *ctx);

	static double hash(unsigned int n);

	static unsigned int m_hash_offset;
	PatternName m_pattern;
	Sprites *m_sprites;
	Context *m_ctx;
};

class SinglePassPlayer : public PatternPlayer {
public:
	SinglePassPlayer(Sprites *sprites, Context *ctx);

	void update() override;
	std::set<PatternName> &compatible_patterns() override;

protected:
	using MoveFunction = std::function<void(Sprite *, Context *, double offset)>;
	static std::map<PatternName, MoveFunction> move_functions;
};

class GlobalPlayer : public PatternPlayer {
public:
	GlobalPlayer(Sprites *sprites, Context *ctx);

	void update() override;
	std::set<PatternName> &compatible_patterns() override;

protected:
	using MoveFunction = std::function<void(Sprites *, Context *, std::function<double(Id)>)>;
	static std::map<PatternName, MoveFunction> move_functions;
};

class SpriteChoreographer {
public:
	SpriteChoreographer(PatternName pattern, Sprites *sprites, Context *ctx);

	void update();

protected:
	void change_pattern();
	bool should_change_pattern();
	void update_player();

	Sprites *m_sprites;
	Context *m_ctx;
	PatternName m_pattern;
	std::vector<PatternPlayer *> m_players;
	PatternPlayer *m_current_player;
};

