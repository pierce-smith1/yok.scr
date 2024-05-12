#pragma once

#include <algorithm>
#include <iterator>
#include <utility>
#include <map>
#include <vector>
#include <set>
#include <numeric>

#include "context.h"
#include "graphics.h"

float wrap(float n, float min, float max);
int clamp(int n, int min, int max);
int emotion_map_index_of(float emotion); 


class Sprite : public Identifiable<Empty> {
public:
	Sprite(const Texture *texture, const Point &home);

	void change_texture(const Texture *texture);
	virtual void draw(Context &ctx);
	virtual void update(Context &ctx);

	float final_x() const;
	float final_y() const;

protected:
	void transform();

	const Texture *m_texture;
	Point m_relpos;
	Point m_home;
	GLfloat m_size;

	friend class SpriteChoreographer;
};

typedef std::vector<float> EmotionVector;

enum Emotion {
	OPTIMISM = 0,
	EMPATHY = 1,
	AMBITION = 2,
	_EMOTIONS_COUNT
};


class Yonker : public Sprite {
public:
	Yonker(const Texture *texture, const Point &home);

	virtual void update(Context &ctx);

protected:
	const Bitmap &bitmap_for_current_emotion(Context &ctx) const;
	EmotionVector emotion_vector(Context &ctx) const;

	EmotionVector m_emotion_vector;
};

class Impostor : public Sprite {
public:
	Impostor(PaletteName palette, const Point &home);

protected:
	static BitmapName random_bitmap();
};
