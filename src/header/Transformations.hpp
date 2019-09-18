
#ifndef FOURIER_TRANSFORMATIONS_H
#define FOURIER_TRANSFORMATIONS_H

#include "Circle.hpp"
#include <cmath>
#include <vector>

namespace Fourier {
	
class Transformations {
public:
	void Transform(std::vector<Circle>& circles, float angle);

private:
	void Rotate(Circle& circle, float angle);
};

}

#endif // FOURIER_TRANSFORMATIONS_H
