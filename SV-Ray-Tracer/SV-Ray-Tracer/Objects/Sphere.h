#pragma once
#include "Intersectable.h"

class Sphere : public Intersectable
{
public:
	Sphere() : c(0.0, 0.0, 0.0), r(1.0) { }
	Sphere(point center, float radius) : c(center), r(radius) { }

	point center() const { return c; }
	float radius() const { return r; }

	bool hit(const ray& r, float tMin, float tMax, Intersection& intersection) const override
	{
		vec3 oc = r.origin() - center();
		float a = r.direction().sqrd_len();
		float b = dot(r.direction(), oc);
		float c = oc.sqrd_len() - radius() * radius();

		float D = b * b - a * c;
		if (D < 0)
		{
			return false;
		}

		float dsqrd = sqrt(D);

		float t = (-b - dsqrd) / a;
		if (t < tMin || t > tMax)
		{
			t = (-b + dsqrd) / a;
			if (t < tMin || t > tMax)
			{
				return false;
			}
		}

		intersection.t = t;
		intersection.p = r.at(t);
		vec3 outward_normal = (r.at(t) - center()) / radius();
		intersection.set_face_normal(r, outward_normal);
		return true;
	}

private:
	point c;
	float r;
};