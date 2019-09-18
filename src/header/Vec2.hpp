
#ifndef FOURIER_VEC2_H
#define FOURIER_VEC2_H

#include "Settings.hpp"
#include <utility>
#include <cmath>

namespace Fourier {

//
// Rule of 4 (Copy-Swap-Assignment) Example Class
// With operator overloading based on implementations from
// https://en.cppreference.com/w/cpp/language/operators
//
template <class T>
struct _Vec2 {
public:
	T X, Y;

public:
	_Vec2() : X(0), Y(0) {}
	_Vec2(T v) : X(v), Y(v) {}
	_Vec2(T x, T y) : X(x), Y(y) {}
	_Vec2(const _Vec2& rhs) : X(rhs.X), Y(rhs.Y) {}
	_Vec2(_Vec2&& rhs) noexcept : X(std::move(rhs.X)), Y(std::move(rhs.Y)) {}

	//
	// Assignment Operator (aka. The Tricky One)
	// In a class with no benefit from memory reuse (no heap-allocated members, std::vector, etc.)
	// the most convenient Assignment-Operator implementation is based on the Copy-and-Swap idiom:
	// - Pass by value so it can be called as Copy OR Move Assignment Operator
	// - Strong exception guarantee: Rhs wont be modified directly and swap does not throw  
	//
	_Vec2& operator=(_Vec2 rhs) noexcept { std::swap(X, rhs.X); std::swap(Y, rhs.Y); return *this; }

	//
	// Arithmetic Operators
	// Operator +=: Compound assignment doesnt need to be a member, but often is, to modify the private members
	// Operator + : Passing lhs by value helps optimize chained "a+b+c" and also return by value (RVO / move constructor)
	// Operator + (vec, v): No need for the definition of (v, vec) because (10 + vec) constructs a vec from 10
	//
	_Vec2& operator+=(const T& v) { X += v; Y += v; return *this; }
	_Vec2& operator-=(const T& v) { X -= v; Y -= v; return *this; }
	_Vec2& operator*=(const T& v) { X *= v; Y *= v; return *this; }
	_Vec2& operator/=(const T& v) { X /= v; Y /= v; return *this; }
	_Vec2& operator+=(const _Vec2& rhs) { X += rhs.X; Y += rhs.Y; return *this; }
	_Vec2& operator-=(const _Vec2& rhs) { X -= rhs.X; Y -= rhs.Y; return *this; }
	_Vec2& operator*=(const _Vec2& rhs) { X *= rhs.X; Y *= rhs.Y; return *this; }
	_Vec2& operator/=(const _Vec2& rhs) { X /= rhs.X; Y /= rhs.Y; return *this; }
	friend _Vec2 operator+(_Vec2 lhs, const T& v) { lhs += v; return lhs; }
	friend _Vec2 operator-(_Vec2 lhs, const T& v) { lhs -= v; return lhs; }
	friend _Vec2 operator*(_Vec2 lhs, const T& v) { lhs *= v; return lhs; }
	friend _Vec2 operator/(_Vec2 lhs, const T& v) { lhs /= v; return lhs; }
	friend _Vec2 operator+(_Vec2 lhs, const _Vec2& rhs) { lhs += rhs; return lhs; }
	friend _Vec2 operator-(_Vec2 lhs, const _Vec2& rhs) { lhs -= rhs; return lhs; }
	friend _Vec2 operator*(_Vec2 lhs, const _Vec2& rhs) { lhs *= rhs; return lhs; }
	friend _Vec2 operator/(_Vec2 lhs, const _Vec2& rhs) { lhs /= rhs; return lhs; }

	//
	// Increment and Decrement Operators
	// operator++(): Pre-Increment overload used in case of ++vec
	// operator++(int): Post-Increment overload for vec++ (Creates a copy, increments this and returns the unchanged copy)
	//
	_Vec2& operator++() { X++; Y++; return *this; }
	_Vec2& operator--() { X--; Y--; return *this; }
	_Vec2 operator++(int) { _Vec2 tmp(*this); operator++(); return tmp; }
	_Vec2 operator--(int) { _Vec2 tmp(*this); operator--(); return tmp; }

	//
	// Comparison Operators (As non-member "friends" inside the class to access .length()) 
	// Inline "hint" is optional, because the compiler (especially whit Optimization) decides on its own what to inline
	//
	friend inline bool operator==(const _Vec2<T>& lhs, const _Vec2<T>& rhs) { return (lhs.X == rhs.X && lhs.Y == rhs.Y); }
	friend inline bool operator!=(const _Vec2<T>& lhs, const _Vec2<T>& rhs) { return !(lhs == rhs); }
	friend inline bool operator< (const _Vec2<T>& lhs, const _Vec2<T>& rhs) { return (lhs.length() < rhs.length()); }
	friend inline bool operator> (const _Vec2<T>& lhs, const _Vec2<T>& rhs) { return (rhs < lhs); }
	friend inline bool operator<=(const _Vec2<T>& lhs, const _Vec2<T>& rhs) { return !(lhs > rhs); }
	friend inline bool operator>=(const _Vec2<T>& lhs, const _Vec2<T>& rhs) { return !(lhs < rhs); }

	//
	// Vector specific calculations
	//
	T length() const { return std::sqrt((X * X) + (Y * Y)); }
	T dot(const _Vec2& rhs) const { return (X * rhs.X) + (Y * rhs.Y); }
	T cross(const _Vec2& rhs) const { return (X * rhs.Y) - (Y * rhs.X); }

	~_Vec2() = default;
};

typedef _Vec2<ushort> Pixel;
typedef _Vec2<int> Vec2i;
typedef _Vec2<float> Vec2;
typedef _Vec2<double> Vec2d;

}

#endif // FOURIER_VEC2_H
