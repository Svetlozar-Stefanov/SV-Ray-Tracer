#pragma once
#include "Material.h"

class Dielectric : public Material
{
public:
	Dielectric(float index_of_refraction)
		: ir(index_of_refraction) {}

	bool scatter(const ray& r_in, const Intersection& intersection, color& attenuation, ray& scattared) const override
	{
		vec3 unit_dir = unit_vector(r_in.direction());

		float cos_theta = fminf(dot(-unit_dir, intersection.normal), 1.0);
		float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		float refr_ratio = intersection.front_face ? 1.0f / ir : ir;

		vec3 dir;
		if (refr_ratio * sin_theta > 1.0
			|| reflectance(cos_theta, ir) > rand_float())
		{
			dir = reflect(unit_dir, intersection.normal);
		}
		else
		{
			dir = refract(unit_dir, intersection.normal, refr_ratio);
		}

		attenuation = color(1.0, 1.0, 1.0);
		scattared = ray(intersection.p, dir);
		
		return true;
	}

private:
	float ir;

	static float reflectance(float cos, float index_of_refraction) {
		auto r0 = (1 - index_of_refraction) / (1 + index_of_refraction);
		r0 *= r0;
		return r0 + (1 - r0) * pow((1 - cos), 5);
	}
};