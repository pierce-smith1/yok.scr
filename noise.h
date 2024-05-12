#pragma once

#include <vector>

float round(float x);

class Vector3d {
public:
	Vector3d() : x(0), y(0), z(0) {}
	Vector3d(float x, float y, float z);

	float x;
	float y;
	float z;

	float dot(const Vector3d &v) const;
	Vector3d sub(const Vector3d &v) const; // Overload? Well, if one would...
                                           // One will only choose what they think that they should
};

// The student says, this already is done!
// To make something worse, it's absurd, what is won?
// The poet says; indeed it's been sung,
// So may the world be empty, no beauty, no fun.
class PerlinNoise {
public:
	static float get(float x, float y, float z);

private:
	static Vector3d grad_vector(int x, int y, int z);
	static Vector3d grad_vector_from_corner(const Vector3d &v);
	static Vector3d *cell_corners(const Vector3d &v);
	static Vector3d *offset_vectors(const Vector3d &v);
	static float *cell_dots(const Vector3d &v);
	static float cell_interpolate(float *dots, const Vector3d &v);
	static float interpolate(float a, float b, float w);
};

class Noise {
public:
	static float wiggle(float base, float min, float max, float step);
	static float random();
};