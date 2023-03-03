#pragma once
#include "Math/vec3.h"
#include "Math/ray.h"

class Camera
{
public:
	Camera(point lookfrom, point lookat, vec3 vup, 
		float fov, float aspect_ratio,
		float apeture, float focus_dist) {
		float theta = degrees_to_radians(fov);
		float h = tan(theta / 2);
		float viewport_height = 2.0 * h;
		float viewport_width = aspect_ratio * viewport_height;
		float focal_length = 1.0;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);



		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist*w;

		lens_radius = apeture / 2;
	}

	ray getRay(float s, float t) const {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();

		return ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset);
	}

private:
	point origin;
	vec3 horizontal;
	vec3 vertical;
	vec3 lower_left_corner;
	vec3 w, u, v;
	float lens_radius;
};