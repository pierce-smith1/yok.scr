#include <stdlib.h>
#include <math.h>

#include "noise.h"

float round(float x) {
	double integral;
	float frac = modf(x, &integral);

	if (frac < 0.5f) {
		return integral;
	} else {
		return integral + 1;
	}
}

Vector3d::Vector3d(float x, float y, float z) 
	: x(x), y(y), z(z) { }

float Vector3d::dot(const Vector3d &v) const {
	return x * v.x + y * v.y + z * v.z;
}

Vector3d Vector3d::sub(const Vector3d &v) const {
	return Vector3d(x - v.x, y - v.y, z - v.z);
};

Vector3d PerlinNoise::grad_vector(int x, int y, int z) {
	// To grow a seed from a single point
	// With orderly chaos a constant we anoint
	// But be careful not to overflow
	// For what happens then, well, no one knows
	unsigned int seed = 1;
	seed = 31 * seed + x;
	seed = 31 * seed + y;
	seed = 31 * seed + z;

	srand(seed);
	float f_x = (float) rand() / RAND_MAX;
	float f_y = (float) rand() / RAND_MAX;
	float f_z = (float) rand() / RAND_MAX;

	float mag = sqrt(f_x * f_x + f_y * f_y + f_z * f_z);
	f_x /= mag;
	f_y /= mag;
	f_z /= mag;

	return Vector3d(f_x, f_y, f_z);
}

Vector3d PerlinNoise::grad_vector_from_corner(const Vector3d &v) {
	return grad_vector(round(v.x), round(v.y), round(v.z));
}

float PerlinNoise::get(float x, float y, float z) {
	Vector3d v = Vector3d(x, y, z);
	return cell_interpolate(cell_dots(v), v);
	//return 1.0f;
}

Vector3d *PerlinNoise::cell_corners(const Vector3d &v) {
	int x = floor(v.x);
	int y = floor(v.y);
	int z = floor(v.z);

	static Vector3d *corners = new Vector3d[8];

	corners[0] = Vector3d(x, y, z);
	corners[1] = Vector3d(x + 1.0f, y, z);
	corners[2] = Vector3d(x, y + 1.0f, z);
	corners[3] = Vector3d(x, y, z + 1.0f);
	corners[4] = Vector3d(x + 1.0f, y + 1.0f, z);
	corners[5] = Vector3d(x + 1.0f, y, z + 1.0f);
	corners[6] = Vector3d(x, y + 1.0f, z + 1.0f);
	corners[7] = Vector3d(x + 1.0f, y + 1.0f, z + 1.0f);

	return corners;
}

Vector3d *PerlinNoise::offset_vectors(const Vector3d &v) {
	Vector3d *corners = cell_corners(v);

	static Vector3d *offsets = new Vector3d[8];

	offsets[0] = v.sub(corners[0]);
	offsets[1] = v.sub(corners[1]);
	offsets[2] = v.sub(corners[2]);
	offsets[3] = v.sub(corners[3]);
	offsets[4] = v.sub(corners[4]);
	offsets[5] = v.sub(corners[5]);
	offsets[6] = v.sub(corners[6]);
	offsets[7] = v.sub(corners[7]);

	return offsets;
}

float *PerlinNoise::cell_dots(const Vector3d &v) {
	Vector3d *offsets = offset_vectors(v);
	Vector3d *corners = cell_corners(v);

	static float *dots = new float[8];

	dots[0] = offsets[0].dot(grad_vector_from_corner(corners[0]));
	dots[1] = offsets[1].dot(grad_vector_from_corner(corners[1]));
	dots[2] = offsets[2].dot(grad_vector_from_corner(corners[2]));
	dots[3] = offsets[3].dot(grad_vector_from_corner(corners[3]));
	dots[4] = offsets[4].dot(grad_vector_from_corner(corners[4]));
	dots[5] = offsets[5].dot(grad_vector_from_corner(corners[5]));
	dots[6] = offsets[6].dot(grad_vector_from_corner(corners[6]));
	dots[7] = offsets[7].dot(grad_vector_from_corner(corners[7]));

	return dots;
}

float PerlinNoise::cell_interpolate(float *dots, const Vector3d &v) {
	float w_x = v.x - floor(v.x);
	float w_y = v.y - floor(v.y);
	float w_z = v.z - floor(v.z);

	float bottom_i1 = interpolate(dots[0], dots[1], w_x);
	float bottom_i2 = interpolate(dots[2], dots[4], w_x);
	float bottom_i = interpolate(bottom_i1, bottom_i2, w_y);

	float top_i1 = interpolate(dots[3], dots[5], w_x);
	float top_i2 = interpolate(dots[6], dots[7], w_x);
	float top_i = interpolate(top_i1, top_i2, w_y);

	return interpolate(bottom_i, top_i, w_z);
}

float PerlinNoise::interpolate(float a, float b, float w) {
	return (b - a) * (3.0 - w * 2.0) * w * w + a;
}

float Noise::wiggle(float base, float min, float max, float step) {
	bool up = random() < 0.5f;

	if (up) {
		return base + random() * (max - base) * step;
	} else {
		return base - random() * (base - min) * step;
	}
}

float Noise::random() {
	static unsigned seed = 1;
	seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
	return (float) seed / 0x7fffffffU;
}