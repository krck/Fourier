#pragma once

#include "Circle.hpp"
#include <cmath>
#include <vector>

class Transformations {
public:
	void Transform(std::vector<Circle>& circles, float angle);

private:
	void Rotate(Circle& circle, float angle);
};
