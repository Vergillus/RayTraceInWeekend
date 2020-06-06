// RayTraceInWeekend01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <random>
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

float fRand()
{
    return float(rand()) / float(RAND_MAX + 1.0);
}

hitable* random_scene()
{
    int n = 500;

    hitable** list = new hitable * [n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5f, 0.5f, 0.5f)));

    int i = 1;

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float chooseMat = fRand();

            vec3 center(a + 0.9 * fRand(), 0.2, b + 0.9 * fRand());

            if ((center - vec3(4,0.2,0)).length() > 0.9)
            {
                if (chooseMat < 0.8) // diffuse
                {
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(fRand() * fRand(), fRand() * fRand(), fRand() * fRand())));
                }
                else if (chooseMat < 0.95) //metal
                {
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5f * (1.0f + fRand()), 0.5f * (1.0f + fRand()), 0.5f * (1.0f + fRand())), 0.5f * fRand()));

                }
                else // glass
                {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4,0.2,0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7,0.6,0.5), 0.0));

    return new hitable_list(list, i);
}

vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;   
    if (world->hit(r,0.001,FLT_MAX,rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
        
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }    
}

int main()
{
    int nx = 1200;
    int ny = 800;
    int ns = 10;

    std::ofstream ofs;

    try
    {
        ofs.open("RayOutput.ppm", std::ios::binary);
        if (ofs.fail()) throw("Can't open output file");

        ofs << "P3\n" << nx << " " << ny << " \n255\n";
        //std::cout << "P3\n" << nx << " " << ny << "\n255\n";

        /*hitable* list[arrSize];
        list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1,0.2,0.5)));
        list[1] = new sphere(vec3(0, -100.5f, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
        list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 1.0));
        list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
        list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));

        hitable* world = new hitable_list(list, arrSize);*/

        hitable* world = random_scene();

        vec3 lookFrom(13, 2, 3);
        vec3 lookAt(0, 0, 0);

        float dist_to_focus = 10.0;
        float aperture = 0.1;

        camera cam(lookFrom, lookAt, vec3(0, 1, 0), 20 ,float(nx)/float(ny),aperture,dist_to_focus);
        

        for (int j = ny - 1; j >= 0; j--)
        {
            for (int i = 0; i < nx; i++)
            {
                vec3 col(0, 0, 0);
                for (int s = 0; s < ns; s++)
                {
                    float u = float(i + fRand()) / float(nx);
                    float v = float(j + fRand()) / float(ny);

                    ray r = cam.get_ray(u, v);
                    col += color(r, world,0);
                }                            
               
                col /= float(ns);
                col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

                int ir = int(255.99 * col[0]);
                int ig = int(255.99 * col[1]);
                int ib = int(255.99 * col[2]);

                ofs << ir << " " << ig << " " << ib << "\n";

                //std::cout << ir << " " << ig << " " << ib << "\n";
            }
        }
        ofs.close();
    }
    catch (const std::exception&)
    {

    }

    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
