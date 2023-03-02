#include "Utils/SV-Ray-Tracer.h"
#include "Objects/Intersectable.h"
#include "Objects/IntersectableList.h"

#include <iostream>


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Utils/General/stb_image_write.h"
#include "Objects/Sphere.h"
#include "Utils/Camera.h"

//Options
const bool antialiasing = true;

int max_ray_depth = 50;

//Image
const float aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = (int)(image_width / aspect_ratio);
const int channels = 3;
const int samples_per_pixel = 5;

color ray_color(const ray& r, const Intersectable& world, int depth)
{
	if (depth <= 0)
	{
		return color(0.0, 0.0, 0.0);
	}

	//Intersection Normal Gradient
	Intersection intersect;
	if (world.hit(r, 0.001, infinity, intersect))
	{
		point target = intersect.p + intersect.normal + random_unit_vector();
		return 0.5 * ray_color(ray(intersect.p, target - intersect.p), world, depth - 1);
	}
	
	//Background
	vec3 unit_dir = unit_vector(r.direction());
	float t = 0.5 * (unit_dir.y() + 1.0);
	return (1 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void write_color(unsigned char *& data, int i, int j, color pixel_col, int samples_per_pixel)
{
	float r = pixel_col.x();
	float g = pixel_col.y();
	float b = pixel_col.z();

	float scale = 1.0 / samples_per_pixel;

	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	data[((image_height - 1 - j) * image_width * channels) + channels * i] = 256 * clamp(r, 0.0, 0.999);
	data[((image_height - 1 - j) * image_width * channels) + channels * i + 1] = 256 * clamp(g, 0.0, 0.999);
	data[((image_height - 1 - j) * image_width * channels) + channels * i + 2] = 256 * clamp(b, 0.0, 0.999);
}

int main()
{
	//World
	IntersectableList world;
	world.add(make_shared<Sphere>(point(0.0, 0.0, -1.0), 0.5));
	world.add(make_shared<Sphere>(point(0.0, -100.5, -1.0), 100.0));

	//Camera
	Camera camera;

	unsigned char* data = new unsigned char[image_height * image_width * channels];
	for (int j = image_height - 1; j >= 0; j--)
	{
		for (int i = 0; i < image_width; i++)
		{
			color pixel_col(0.0, 0.0, 0.0);
			if (antialiasing)
			{
				for (int s = 0; s < samples_per_pixel; s++)
				{
					float u = (i + rand_float()) / (image_width - 1);
					float v = (j + rand_float()) / (image_height - 1);

					ray r = camera.getRay(u, v);

					pixel_col += ray_color(r, world, max_ray_depth);
				}
				write_color(data, i, j, pixel_col, samples_per_pixel);
			}
			else
			{
				float u = float(i) / (image_width - 1);
				float v = float(j) / (image_height - 1);

				ray r = camera.getRay(u, v);

				pixel_col = ray_color(r, world, max_ray_depth);
				write_color(data, i, j, pixel_col, 1);
			}
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