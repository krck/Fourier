
#ifndef FOURIER_CIRCLE_H
#define FOURIER_CIRCLE_H

#include "Vec2.hpp"
#include <utility>

namespace Fourier {
	
class Circle {
public:
	Pixel Center;
	Pixel CycleDot;
	ushort Radius;
	ushort AngleOffset;
	ushort Frequency;

public:
	Circle(const Pixel& center, const ushort& radius, const ushort& angleOffset, const ushort& frequency)
		: Center(center), CycleDot(Pixel(center.X + radius, center.Y)), Radius(radius), AngleOffset(angleOffset), Frequency(frequency) {}

	Circle(Pixel&& center, ushort&& radius, ushort&& angleOffset, ushort&& frequency) noexcept
		: Center(center), CycleDot(Pixel(center.X + radius, center.Y)), Radius(radius), AngleOffset(angleOffset), Frequency(frequency) {}

	Circle(const Circle& rhs)
		: Circle(rhs.Center, rhs.Radius, rhs.AngleOffset, rhs.Frequency) {}

	Circle(Circle&& rhs) noexcept
		: Circle(std::move(rhs.Center), std::move(rhs.Radius), std::move(rhs.AngleOffset), std::move(rhs.Frequency)) {}

	Circle& operator=(const Circle& rhs) { return *this = Circle(rhs); }
	Circle& operator=(Circle&& rhs) noexcept {
		std::swap(Center, rhs.Center);
		std::swap(CycleDot, rhs.CycleDot);
		std::swap(Radius, rhs.Radius);
		std::swap(AngleOffset, rhs.AngleOffset);
		std::swap(Frequency, rhs.Frequency);
		return *this;
	}

	~Circle() = default;
};

}

#endif // FOURIER_CIRCLE_H
