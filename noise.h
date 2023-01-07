#pragma once

#include <tuple>
#include <random>
#include <array>

// The student says, this already is done!
// To make something worse, it's absurd, what is won?
// The poet says; indeed it's been sung,
// So may the world be empty, no beauty, no fun.
class PerlinNoise {
public:
	static float get(float x, float y, float z);

private:
	class Vector : public std::tuple<float, float, float> {
	public:
		Vector(float x, float y, float z);

		float x() const;
		float y() const;
		float z() const;
		float dot(const Vector &v) const;
		Vector sub(const Vector &v) const; // Overload? Well, if one would...
		                                   // One will only choose what they think that they should
	};

	static Vector grad_vector(int x, int y, int z);
	static Vector grad_vector_from_corner(const Vector &v);
	static std::array<Vector, 8> cell_corners(const Vector &v);
	static std::array<Vector, 8> offset_vectors(const Vector &v);
	static std::array<float, 8> cell_dots(const Vector &v);
	static float cell_interpolate(std::array<float, 8> dots, const Vector &v);
	static float interpolate(float a, float b, float w);
};

class Noise {
public:
	static float wiggle(float base, float min, float max, float step);
	static float random();
};