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
	static double get(double x, double y, double z);

private:
	class Vector : public std::tuple<double, double, double> {
	public:
		Vector(double x, double y, double z);

		double x() const;
		double y() const;
		double z() const;
		double dot(const Vector &v) const;
		Vector sub(const Vector &v) const; // Overload? Well, if one would...
		                                   // One will only choose what they think that they should
	};

	const static unsigned int base_seed;

	static Vector grad_vector(double x, double y, double z);
	static Vector grad_vector_from_corner(const Vector &v);
	static std::array<Vector, 8> cell_corners(const Vector &v);
	static std::array<Vector, 8> offset_vectors(const Vector &v);
	static std::array<double, 8> cell_dots(const Vector &v);
	static double cell_interpolate(std::array<double, 8> dots, const Vector &v);
	static double interpolate(double a, double b, double w);
};

class Noise {
public:
	static double wiggle(double base, double min, double max, double step);
	static double random();
};