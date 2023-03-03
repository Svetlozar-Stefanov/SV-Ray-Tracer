#pragma once
#include "Material.h"

class Lambertian : public Material
{
public:
	Lambertian(color a) 
		: albedo(a) {}

	bool scatter(const ray& r_in, const Intersection& intersection, color& attenuation, ray& scattared) const override
	{
		auto scatter_dir = intersection.normal + random_unit_vector();
		scattared = ray(intersection.p, scatter_dir);
		attenuation = albedo;

		if (scatter_dir.near_zero())
		{
			scatter_dir = intersection.normal;
		}

		return true;
	}

private:
	color albedo;
};