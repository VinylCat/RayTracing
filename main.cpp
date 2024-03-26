#include <GL/glut.h>
#include "constants.h"
#include "bvh.h"
#include "camera.h"
#include "scene.h"
#include "material.h"
#include "object/sphere.h"
#include "texture.h"
#include "object/quad.h"
#include "object/constant_medium.h"
#include "utils/OBJ_Loader.h"
#include "object/triangle.h"
using color = glm::dvec3;
using point = glm::dvec3;


void earth() {
    scene world;
    auto earth_texture = make_shared<image_texture>("gate.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point(4, 1, 0), 1.0, material3));
    world.add(make_shared<sphere>(point(0,0,0), 2, earth_surface));

    world = scene(make_shared<bvh_node>(world));

    camera cam;

    cam.ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point(0,0,12);
    cam.lookat   = point(0,0,0);
    cam.vup      = vec(0,1,0);

    cam.defocus_angle = 0;

    cam.show(world);
}

void final_scene() {
    scene world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    auto library_surface = make_shared<image_texture>("gate1.jpg");
    auto library_texture = make_shared<lambertian>(library_surface);
    auto stone_surface = make_shared<image_texture>("StoneRough.jpg");
    auto stone_texture = make_shared<lambertian>(stone_surface);

    world.add(make_shared<quad>(point(555,0,0), vec(0,555,0), vec(0,0,555), green));
    world.add(make_shared<quad>(point(0,0,0), vec(0,555,0), vec(0,0,555), red));
    world.add(make_shared<quad>(point(343, 554, 332), vec(-130,0,0), vec(0,0,-105), light));
    world.add(make_shared<quad>(point(0,0,0), vec(555,0,0), vec(0,0,555), white));
    world.add(make_shared<quad>(point(555,555,555), vec(-555,0,0), vec(0,0,-555), white));
    world.add(make_shared<quad>(point(0,0,555), vec(555,0,0), vec(0,555,0), library_texture));

    /*
    shared_ptr<object> box1 = box(point(0,0,0), point(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec(265,0,295));
    world.add(box1);

    shared_ptr<object> box2 = box(point(0,0,0), point(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec(130,0,65));
    world.add(box2);*/

    bool load_out;
    objl::Loader Loader;
    load_out = Loader.LoadFile("D:\\Works\\Course\\CS3310_Graphics\\RayTracing\\models\\masterpiece.obj");
    if(load_out)
    {
        std::cout<<"load obj sucessfully."<<std::endl;
        for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = Loader.LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                vec tmp[3];
                tmp[0] = vec(curMesh.Vertices[curMesh.Indices[j]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                tmp[1] = vec(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                tmp[2] = vec(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
                world.add(make_shared<triangle>(tmp[0], tmp[1], tmp[2], make_shared<lambertian>(color(0.99, 0.99, 0.4))));
            }
        }
    }
    else
        std::cout<<"Failed to load obj"<<std::endl;

    objl::Loader bee_loader;
    load_out = bee_loader.LoadFile("D:\\Works\\Course\\CS3310_Graphics\\RayTracing\\models\\bee.obj");
    if(load_out)
    {
        std::cout<<"load obj sucessfully."<<std::endl;
        for (int i = 0; i < bee_loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = bee_loader.LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                vec tmp[3];
                glm::dvec2 uvs[3];
                //uv from j ~ j + 2
                tmp[0] = vec(curMesh.Vertices[curMesh.Indices[j]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                tmp[1] = vec(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                tmp[2] = vec(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
                uvs[0] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
                uvs[1] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
                uvs[2] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
                world.add(make_shared<triangle>(tmp[0], tmp[1], tmp[2],
                                                uvs[0], uvs[1], uvs[2],
                                                make_shared<lambertian>(color(0.65, 0.91, 0.03))));
            }
        }
    }
    else
        std::cout<<"Failed to load obj"<<std::endl;

    objl::Loader stone_loader;
    load_out = stone_loader.LoadFile("D:\\Works\\Course\\CS3310_Graphics\\RayTracing\\models\\stone_set.obj");
    if(load_out)
    {
        std::cout<<"load obj sucessfully."<<std::endl;
        for (int i = 0; i < stone_loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = stone_loader.LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                vec tmp[3];
                glm::dvec2 uvs[3];
                //uv from j ~ j + 2
                tmp[0] = vec(curMesh.Vertices[curMesh.Indices[j]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                tmp[1] = vec(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                tmp[2] = vec(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
                uvs[0] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
                uvs[1] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
                uvs[2] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
                world.add(make_shared<triangle>(tmp[0], tmp[1], tmp[2],
                                                uvs[0], uvs[1], uvs[2],
                                                stone_texture));
            }
        }
    }//make_shared<lambertian>(color(0.42, 0.47, 0.46))
    else
        std::cout<<"Failed to load obj"<<std::endl;

   objl::Loader star_loader;
    load_out = star_loader.LoadFile("D:\\Works\\Course\\CS3310_Graphics\\RayTracing\\models\\stars.obj");
    if(load_out)
    {
        std::cout<<"load obj sucessfully."<<std::endl;
        for (int i = 0; i < star_loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = star_loader.LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                vec tmp[3];
                glm::dvec2 uvs[3];
                //uv from j ~ j + 2
                tmp[0] = vec(curMesh.Vertices[curMesh.Indices[j]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                tmp[1] = vec(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                tmp[2] = vec(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
                uvs[0] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
                uvs[1] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
                uvs[2] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
                world.add(make_shared<triangle>(tmp[0], tmp[1], tmp[2],
                                                uvs[0], uvs[1], uvs[2],
                                                make_shared<metal>(color(.99, .87, .08), 0.8)));
            }
        }
    }//make_shared<lambertian>(color(.99, .87, .08))
    else
        std::cout<<"Failed to load obj"<<std::endl;

    objl::Loader gstar_loader;
    load_out = gstar_loader.LoadFile("D:\\Works\\Course\\CS3310_Graphics\\RayTracing\\models\\glassstar.obj");
    if(load_out)
    {
        std::cout<<"load obj sucessfully."<<std::endl;
        for (int i = 0; i < gstar_loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = gstar_loader.LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                vec tmp[3];
                glm::dvec2 uvs[3];
                //uv from j ~ j + 2
                tmp[0] = vec(curMesh.Vertices[curMesh.Indices[j]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                tmp[1] = vec(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                tmp[2] = vec(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
                uvs[0] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
                uvs[1] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j + 1]].TextureCoordinate.Y);
                uvs[2] = glm::dvec2(curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.X,
                                    curMesh.Vertices[curMesh.Indices[j + 2]].TextureCoordinate.Y);
                world.add(make_shared<triangle>(tmp[0], tmp[1], tmp[2],
                                                uvs[0], uvs[1], uvs[2],
                                                make_shared<dielectric>(1.5)));
            }
        }
    }//make_shared<lambertian>(color(.99, .87, .08))
    else
        std::cout<<"Failed to load obj"<<std::endl;

    world = scene(make_shared<bvh_node>(world));

    camera cam;

    cam.ratio      = 1.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 20 ;
    cam.max_depth         = 8;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point(278, 278, -800);
    cam.lookat   = point(278, 278, 0);
    cam.vup      = vec(0,1,0);

    cam.defocus_angle = 0;

    cam.show(world);
}

void quads() {
    scene world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point(-3,-2, 5), vec(0, 0,-4), vec(0, 4, 0), left_red));
    world.add(make_shared<quad>(point(-2,-2, 0), vec(4, 0, 0), vec(0, 4, 0), back_green));
    world.add(make_shared<quad>(point( 3,-2, 1), vec(0, 0, 4), vec(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point(-2, 3, 1), vec(4, 0, 0), vec(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point(-2,-3, 5), vec(4, 0, 0), vec(0, 0,-4), lower_teal));

    camera cam;

    cam.ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = point(0,0,9);
    cam.lookat   = point(0,0,0);
    cam.vup      = vec(0,1,0);

    cam.defocus_angle = 0;

    cam.show(world);
}

void tri_try() {
    scene world;
    auto material1 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point(-4, 1, 0), 1.0, material1));
    auto material2 = make_shared<lambertian>(color(0.9 , 0.9, 0));
    auto tri = make_shared<triangle>(point(0,0,0),
                                       point(0, 1, 0), point(1,0, 0), material2);
    world.add(tri);
    world = scene(make_shared<bvh_node>(world));

    camera cam;

    cam.ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 60;
    cam.lookfrom = point(2,1,-2);
    cam.lookat   = point(0,0,0);
    cam.vup      = vec(0,0,1);

    cam.defocus_angle = 0;

    cam.show(world);
}

void obj_load()
{
    scene world;
    objl::Loader Loader;

    bool load_out = Loader.LoadFile("D:\\Works\\Course\\CS3310_Graphics\\RayTracing\\models\\test.obj");

    if(load_out)
    {
        std::cout<<"load obj sucessfully."<<std::endl;
        for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = Loader.LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                vec tmp[3];
                tmp[0] = vec(curMesh.Vertices[curMesh.Indices[j]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                tmp[1] = vec(curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                tmp[2] = vec(curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                             curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);
                std::cout<<tmp[0].x<<" "<<tmp[0].y<<" "<<tmp[0].z<<" "<<std::endl;
                world.add(make_shared<triangle>(tmp[0], tmp[1], tmp[2], make_shared<lambertian>(color(.9, .9, .1))));
            }
        }
    }
    else
    {
        std::cout<<"Failed to load obj"<<std::endl;
    }

    world = scene(make_shared<bvh_node>(world));
    camera cam;

    cam.ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 20;
    cam.max_depth         = 6;
    cam.background        = color(0.2,0.5,0.6);


    cam.vfov     = 40;
    cam.lookfrom = point(0,0,18);
    cam.lookat   = point(0,0,0);
    cam.vup      = vec(0,1,0);

    cam.defocus_angle = 0;
    cam.show(world);
}

int main(int argc, char *argv[]) {
    //glutInit(&argc, argv);
    //earth();
    final_scene();
    //obj_load();
    //tri_try();
    return 0;
}