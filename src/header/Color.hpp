
#ifndef FOURIER_COLOR_H
#define FOURIER_COLOR_H

#include "Settings.hpp"

// Predefined Colors
#define COLOR_RED			Color(179, 0, 0)
#define COLOR_GREEN			Color(0, 179, 0)
#define COLOR_BLUE 			Color(0, 0, 179)
#define COLOR_YELLOW 		Color(230, 184, 0)
#define COLOR_BLACK 		Color(0, 0, 0)
#define COLOR_GRAY_LIGHT 	Color(220, 220, 220)
#define COLOR_GRAY_MEDIUM 	Color(191, 191, 191)
#define COLOR_GRAY_DARK 	Color(115, 115, 115)
#define COLOR_WHITE 		Color(255, 255, 255)

namespace Fourier {

template <class T>
struct _Color {
public:
	T r, g, b, a;

public:
	_Color() : r(255), g(255), b(255), a(255) {} // white as default
	_Color(const _Color& rhs) = default;
	_Color(_Color&& rhs) noexcept = default;
	_Color(T red, T green, T blue) : r(red), g(green), b(blue), a(255) {}
	_Color(T red, T green, T blue, T alpha) : r(red), g(green), b(blue), a(alpha) {}

	//
	// Assignment Operator (Copy-Swap)
	//
	_Color& operator=(_Color rhs) noexcept { 
		std::swap(r, rhs.r); 
		std::swap(g, rhs.g); 
		std::swap(b, rhs.b); 
		std::swap(a, rhs.a); 
		return *this; 
	}
	
	//
	// Arithmetic Operators (For implementation details see "Vec2.hpp")
	//
	_Color& operator+=(const _Color& rhs) { r += rhs.r; g += rhs.g; b += rhs.b; a += rhs.a; return *this; }
	_Color& operator-=(const _Color& rhs) { r -= rhs.r; g -= rhs.g; b -= rhs.b; a -= rhs.a; return *this; }
	_Color& operator*=(const _Color& rhs) { r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a; return *this; }
	_Color& operator/=(const _Color& rhs) { r /= rhs.r; g /= rhs.g; b /= rhs.b; a /= rhs.a; return *this; }
	friend _Color operator+(_Color lhs, const _Color& rhs) { lhs += rhs; return lhs; }
	friend _Color operator-(_Color lhs, const _Color& rhs) { lhs -= rhs; return lhs; }
	friend _Color operator*(_Color lhs, const _Color& rhs) { lhs *= rhs; return lhs; }
	friend _Color operator/(_Color lhs, const _Color& rhs) { lhs /= rhs; return lhs; }
	
	//
	// Comparison Operators
	//
	friend inline bool operator==(const _Color& lhs, const _Color& rhs) { return (lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a); }
	friend inline bool operator!=(const _Color& lhs, const _Color& rhs) { return !(lhs == rhs); }
	
	~_Color() = default;
};

// Predefined Color type 
// (Could be "byte", but that would likely crash the arithmetic operators)
typedef _Color<ushort> Color;

}

#endif // FOURIER_COLOR_H
