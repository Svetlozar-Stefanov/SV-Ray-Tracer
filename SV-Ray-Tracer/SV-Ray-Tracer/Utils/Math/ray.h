#pragma once
#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const vec3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	point origin() const { return orig; }
	vec3 direction() const { return dir; }
	
	point at(float t) const
	{
		return orig + t * dir;
	}
	
private:
	point orig;
	vec3 dir;
};