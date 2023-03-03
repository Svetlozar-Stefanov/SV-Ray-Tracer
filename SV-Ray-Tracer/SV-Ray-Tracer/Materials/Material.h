#pragma once
#include "..\Utils\SV-Ray-Tracer.h"
#include "..\Objects\Intersectable.h"

struct Intersection;

class Material
{
public:
	virtual bool scatter(const ray& r_in, const Intersection& intersection, color& attenuation, ray& scattared) const = 0;
};