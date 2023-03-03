#pragma once
#include "Material.h"

class Metal : public Material
{
public:
	Metal(color a, float f)
		: albedo(a), fuzz(f < 1 ? f : 1) {}

	bool scatter(const ray& r_in, const Intersection& intersection, color& attenuation, ray& scattared) const override
	{
		vec3 reflected_dir = reflect(unit_vector(r_in.direction()), intersection.normal);
		scattared = ray(intersection.p, reflected_dir + fuzz * random_in_unit_sphere());
		attenuation = albedo;

		return (dot(reflected_dir, intersection.normal) > 0);
	}

private:
	color albedo;
	float fuzz;
};