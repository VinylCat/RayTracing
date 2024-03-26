cmake_minimum_required(VERSION 3.26)
project(RayTracing)

include_directories(include)
link_directories(lib/x64)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(RayTracing main.cpp
        ray.h
        object/object.h
        object/sphere.h
        scene.h
        constants.h
        interval.h
        camera.h
        material.h
        vec_tools.h
        aabb.h
        bvh.h
        texture.h
        utils/rtw_stb_image.h
        utils/stb_image.h
        object/quad.h
        object/constant_medium.h
        object/triangle.h
        utils/OBJ_Loader.h
        record.h
)
target_link_libraries(RayTracing libopengl32.a libfreeglut.a glu32.dll)