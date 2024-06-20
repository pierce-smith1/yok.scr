#include "noise.h"
#include "common.h"

double PerlinNoise::get(double x, double y, double z) {
	Vector v = Vector(x, y, z);
	return cell_interpolate(cell_dots(v), v);
}

PerlinNoise::Vector::Vector(double x, double y, double z)
	: std::tuple<double, double, double>(x, y, z) { }

double PerlinNoise::Vector::x() const {
	return std::get<0>(*this);
}

double PerlinNoise::Vector::y() const {
	return std::get<1>(*this);
}

double PerlinNoise::Vector::z() const {
	return std::get<2>(*this);
}

double PerlinNoise::Vector::dot(const Vector &v) const {
	return x() * v.x() + y() * v.y() + z() * v.z();
}

PerlinNoise::Vector PerlinNoise::Vector::sub(const Vector &v) const {
	return Vector(x() - v.x(), y() - v.y(), z() - v.z());
};

PerlinNoise::Vector PerlinNoise::grad_vector(double x, double y, double z) {
	// To grow a seed from a single point
	// With orderly chaos a constant we anoint
	// But be careful not to overflow
	// For what happens then, well, no one knows
	double seed = 1;
	seed = 31 * seed + x;
	seed = 31 * seed + y;
	seed = 31 * seed + z;

	std::srand(cast<unsigned int>(seed));
	double f_x = (double) std::rand() / RAND_MAX;
	double f_y = (double) std::rand() / RAND_MAX;
	double f_z = (double) std::rand() / RAND_MAX;

	double mag = sqrt(f_x * f_x + f_y * f_y + f_z * f_z);
	f_x /= mag;
	f_y /= mag;
	f_z /= mag;

	return Vector(f_x, f_y, f_z);
}

PerlinNoise::Vector PerlinNoise::grad_vector_from_corner(const Vector &v) {
	return grad_vector(round(v.x()), round(v.y()), round(v.z()));
}

std::array<PerlinNoise::Vector, 8> PerlinNoise::cell_corners(const Vector &v) {
	double x = floor(v.x());
	double y = floor(v.y());
	double z = floor(v.z());

	return {
		Vector(x, y, z),
		Vector(x + 1.0, y, z),
		Vector(x, y + 1.0, z),
		Vector(x, y, z + 1.0),
		Vector(x + 1.0, y + 1.0, z),
		Vector(x + 1.0, y, z + 1.0),
		Vector(x, y + 1.0, z + 1.0),
		Vector(x + 1.0, y + 1.0, z + 1.0)
	};
}

std::array<PerlinNoise::Vector, 8> PerlinNoise::offset_vectors(const Vector &v) {
	auto corners = cell_corners(v);

	return {
		v.sub(corners[0]),
		v.sub(corners[1]),
		v.sub(corners[2]),
		v.sub(corners[3]),
		v.sub(corners[4]),
		v.sub(corners[5]),
		v.sub(corners[6]),
		v.sub(corners[7]),
	};
}

std::array<double, 8> PerlinNoise::cell_dots(const Vector &v) {
	auto offsets = offset_vectors(v);
	auto corners = cell_corners(v);

	return {
		offsets[0].dot(grad_vector_from_corner(corners[0])),
		offsets[1].dot(grad_vector_from_corner(corners[1])),
		offsets[2].dot(grad_vector_from_corner(corners[2])),
		offsets[3].dot(grad_vector_from_corner(corners[3])),
		offsets[4].dot(grad_vector_from_corner(corners[4])),
		offsets[5].dot(grad_vector_from_corner(corners[5])),
		offsets[6].dot(grad_vector_from_corner(corners[6])),
		offsets[7].dot(grad_vector_from_corner(corners[7])),
	};
}

double PerlinNoise::cell_interpolate(std::array<double, 8> dots, const Vector &v) {
	double w_x = v.x() - floor(v.x());
	double w_y = v.y() - floor(v.y());
	double w_z = v.z() - floor(v.z());

	double bottom_i1 = interpolate(dots[0], dots[1], w_x);
	double bottom_i2 = interpolate(dots[2], dots[4], w_x);
	double bottom_i = interpolate(bottom_i1, bottom_i2, w_y);

	double top_i1 = interpolate(dots[3], dots[5], w_x);
	double top_i2 = interpolate(dots[6], dots[7], w_x);
	double top_i = interpolate(top_i1, top_i2, w_y);

	return interpolate(bottom_i, top_i, w_z);
}

double PerlinNoise::interpolate(double a, double b, double w) {
	return (b - a) * (3.0 - w * 2.0) * w * w + a;
}

double Noise::random() {
	static std::random_device device;
	static std::mt19937 rng(device());
	static std::uniform_real_distribution<> dist(0.0, 1.0);

	return dist(rng);
}