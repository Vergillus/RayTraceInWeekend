#pragma once

/*
#ifndef CAMERA_H
#define CAMERA_H
#endif
*/

#include "ray.h"


vec3 random_in_unit_disk()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(float(rand()) / float(RAND_MAX), float(rand()) / float(RAND_MAX), 0) - vec3(1, 1, 0);
	} while (dot(p,p) >= 1.0);

	return p;
}

class camera 
{
public:
	camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vFov, float aspect, float aperture, float focus_dist) // vFov is top to bottom in degrees
	{
		lens_radius = aperture / 2.0f;
		
		float theta = vFov * std::_Pi / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vUp, w));
		v = cross(w, u);
		
		//lower_left_corner = vec3(-half_width, -half_height, -1.0);

		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		horizontal = 2.0 * half_width * focus_dist * u;
		vertical = 2.0 * half_height * focus_dist * v;
		
	}

	ray get_ray(float s, float t)
	{
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return  ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset); 
	}

	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
	vec3 u, v, w;

	float lens_radius;


	
};
