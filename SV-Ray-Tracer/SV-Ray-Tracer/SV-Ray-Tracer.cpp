#include "Utils/SV-Ray-Tracer.h"
#include "Objects/Intersectable.h"
#include "Objects/IntersectableList.h"

#include <iostream>


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Utils/General/stb_image_write.h"
#include "Objects/Sphere.h"

color ray_color(const ray& r, const Intersectable& world)
{
	//Intersection Normal Gradient
	Intersection intersect;
	if (world.hit(r, 0, infinity, intersect))
	{
		return 0.5 * (intersect.normal + color(1, 1, 1));
	}
	
	//Background
	vec3 unit_dir = unit_vector(r.direction());
	float t = 0.5 * (unit_dir.y() + 1.0);
	return (1 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	//Image
	const float aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = (int)(image_width / aspect_ratio);
	const int channels = 3;

	//World
	IntersectableList world;
	world.add(make_shared<Sphere>(point(0.0, 0.0, -1.0), 0.5));
	world.add(make_shared<Sphere>(point(0.0, -100.5, -1.0), 100.0));

	//Camera	
	float viewport_height = 2.0;
	float viewport_width = aspect_ratio * viewport_height;
	float focal_length = 1.0;

	point origin = point(0.0, 0.0, 0.0);
	vec3 horizontal = vec3(viewport_width, 0.0, 0.0);
	vec3 vertical = vec3(0.0, viewport_height, 0.0);
	vec3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0.0, 0.0, focal_length);

	unsigned char* data = new unsigned char[image_height * image_width * channels];
	for (int j = image_height - 1; j >= 0; j--)
	{
		for (int i = 0; i < image_width; i++)
		{
			float u = float(i) / (image_width - 1);
			float v = float(j) / (image_height - 1);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);

			color c = ray_color(r, world);

			int ir = (int)(255.999 * c.x());
			int ig = (int)(255.999 * c.y());
			int ib = (int)(255.999 * c.z());

			data[((image_height - 1 - j) * image_width * channels) + channels * i] = ir;
			data[((image_height - 1 - j) * image_width * channels) + channels * i + 1] = ig;
			data[((image_height - 1 - j) * image_width * channels) + channels * i + 2] = ib;
		}
	}

	if (!stbi_write_jpg("image.jpg", image_width, image_height, channels, data, 90))
	{
		std::cerr << "\nCould not create image.\n";
		delete[] data;
		return 1;
	}
	std::cerr << "\nDone.\n";
	delete[] data;
}