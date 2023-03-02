#pragma once
#include "../Utils/Math/vec3.h"
#include "../Utils/Math/ray.h"

struct Intersection
{
	point p;
	vec3 normal;
	float t;

	bool front_face;

	inline void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Intersectable
{
public:
	virtual bool hit(const ray& r, float tMin, float tMax, Intersection& intersection) const = 0;
};