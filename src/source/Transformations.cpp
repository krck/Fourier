#include "../header/Transformations.hpp"

void Transformations::Transform(std::vector<Circle>& circles, float angle) {
	Pixel prevDot = circles.front().Center;
	for (Circle& c : circles) {
		c.Center = prevDot;

		Rotate(c, angle);

		prevDot = c.CycleDot;
	}
}

void Transformations::Rotate(Circle& circle, float angle) {
	const Pixel center = circle.Center;
	const float radius = circle.Radius;
	const float ar = ((angle * circle.Frequency) + circle.AngleOffset) * (M_PI / 180);

	// Update the Dot position on the Circle circumference based on the new angle
	circle.CycleDot = Pixel(radius * cos(ar) + center.X, radius * sin(ar) + center.Y);
}
