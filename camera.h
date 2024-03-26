//
// Created by W9990 on 2023/11/18.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H
#include "constants.h"
#include <GL/glut.h>
#include "object/object.h"
#include "material.h"
#include "utils/svpng.inc"
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <thread>
#include <atomic>
//extern scene world;


using color = glm::dvec3;
using point = glm::dvec3;



class camera
{
public:
    double ratio = 16.0/9.0;
    int image_width = 1600;
    int samples_per_pixel = 10;
    int max_depth = 10;
    color  background;               // Scene background color


    double vfov = 90;
    point lookfrom = point(0, 0, -1);
    point lookat = point(0, 0, 0);
    vec vup = vec(0, 1, 0);

    double defocus_angle = 0;
    double focus_dist = 10;

    inline double linear_to_gamma(double linear_component)
    {
        return sqrt(linear_component);
    }

    void show(const object& world)
    {
        initialize();
        //static camera* instance = this;
        //auto glutWrapperStatic = [](void) {
        //    instance->glutWrapperLambda();
        //};

        //glutDisplayFunc(glutWrapperStatic);
        render(world);
        //glutMainLoop();
    }

private:

    int image_height;
    point center;
    point pixel00_loc;
    vec pixel_delta_u;
    vec pixel_delta_v;
    vec u, v, w;

    vec defocus_disk_u;
    vec defocus_disk_v;


    void initialize()
    {

        image_height = static_cast<int>(image_width / ratio);
        //std::cout<<image_height<<std::endl;
        center = lookfrom ;


        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = glm::normalize(lookfrom - lookat);
        u = glm::normalize(glm::cross(vup, w));
        v = glm::cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec viewport_v = viewport_height * -v;  // Vector down viewport vertical edge
        pixel_delta_u = double(1.0/image_width) * viewport_u;;
        pixel_delta_v = double(1.0/image_height) * viewport_v;
        auto viewport_upper_left =
                center - (focus_dist * w) - 0.5 * viewport_u - 0.5 *viewport_v;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;

        //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        //glutInitWindowSize(image_width, image_height);
        //glutInitWindowPosition(200, 100);
        //glutCreateWindow("OpenGL Demo");


    }

    color ray_color(const ray& r, int depth, const object& world) const
    {
        hit_record rec;
        if (depth <= 0)
            return color(0, 0, 0);
        if (!world.intersection(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);

        return color_from_emission + color_from_scatter;
    }

    /*void write_color(color pixel_color, point pixel_location, int samples_per_pixel)
    {
        auto r = pixel_color.x;
        auto g = pixel_color.y;
        auto b = pixel_color.z;

        auto scale = 1.0 / samples_per_pixel;
        r *= scale;
        g *= scale;
        b *= scale;

        r = linear_to_gamma(r);
        g = linear_to_gamma(g);
        b = linear_to_gamma(b);

        static const interval intensity(0.000, 0.999);
        glBegin(GL_POINTS);
        glColor3d(double(intensity.clamp(r)),
                  double(intensity.clamp(g)),
                  double(intensity.clamp(b)));
        glVertex3d(double(-1.0 + pixel_location.x/(image_width/2.0)),
                   double(1.0 - pixel_location.y/(image_height/2.0)),
                   0);
        glEnd();
    }*/

    void render(const object& world)
    {
        //glClear(GL_COLOR_BUFFER_BIT);
        FILE *fp = std::fopen("D:\\Pictures\\test.png", "wb");
        unsigned char rgb[image_height * image_width * 3];
        //glFlush();
        //glutMainLoop();
        const int num_threads = 15;
        std::vector<std::thread> threads;
        std::atomic<int> next_row(0);

        auto render_thread = [this, &world, &rgb, &next_row](){
            while(true)
            {
                int j = next_row++;
                if (j >= image_height) break;
                std::clog<<"Processing: "<< 100.0 *((float)j /(float)image_height) <<"%\n"<<std::flush;
                    unsigned char* p = rgb + (j * image_width * 3);
                    for (int i = 0; i < image_width; ++i)
                    {
                        color  pixel_color(0, 0, 0);
                        for (int sample = 0; sample < samples_per_pixel; ++sample)
                        {
                            ray r = get_ray(i, j);
                            pixel_color += ray_color(r, max_depth,world);
                        }

                        auto r = pixel_color.x;
                        auto g = pixel_color.y;
                        auto b = pixel_color.z;

                        // Divide the color by the number of samples.
                        auto scale = 1.0 / samples_per_pixel;
                        r *= scale;
                        g *= scale;
                        b *= scale;

                        // Apply the linear to gamma transform.
                        r = linear_to_gamma(r);
                        g = linear_to_gamma(g);
                        b = linear_to_gamma(b);

                        // Write the translated [0,255] value of each color component.
                        static const interval intensity(0.000, 0.999);
                        *p++ = static_cast<int>(256 * intensity.clamp(r));
                        *p++ = static_cast<int>(256 * intensity.clamp(g));
                        *p++ = static_cast<int>(256 * intensity.clamp(b));
                    }
            }
        };

        for (int i = 0; i < num_threads; ++i)
            threads.emplace_back(render_thread);

        for (auto& thread : threads)
        {
            if(thread.joinable())
                thread.join();
        }

        svpng(fp, image_width, image_height, rgb, 0);
        std::fclose(fp);
        std::cout<<"finished";
    }

    ray get_ray(int i, int j) const
    {
        auto pixel_center = pixel00_loc + (double(i) * pixel_delta_u) + (double(j) * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    point defocus_disk_sample() const
    {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) +(p[1] * defocus_disk_v);
    }

    vec pixel_sample_square() const
    {
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

};

#endif //RAYTRACING_CAMERA_H