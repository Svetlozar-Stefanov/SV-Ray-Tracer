#pragma once
#include <vector>
#include "Intersectable.h"

class IntersectableList : public Intersectable
{
public:
	IntersectableList() {}
	IntersectableList(std::shared_ptr<Intersectable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<Intersectable> object)
	{
		objects.push_back(object);
	}

	bool hit(const ray& r, float tMin, float tMax, Intersection& intersection) const override
	{
		Intersection temp_intersec;
		bool hit = false;
		float closest = tMax;

		for (const auto& object : objects)
		{
			if (object->hit(r, tMin, closest, temp_intersec))
			{
				hit = true;
				closest = temp_intersec.t;
				intersection = temp_intersec;
			}
		}

		return hit;
	}

private:
	std::vector<std::shared_ptr<Intersectable>> objects;
};