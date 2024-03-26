//
// Created by W9990 on 2024/1/7.
//

#ifndef RAYTRACING_RECORD_H
#define RAYTRACING_RECORD_H

#include "constants.h"
#include "aabb.h"

class material;

class hit_record
{
public:
    point p;
    vec normal;
    shared_ptr<material> mat;
    double t;
    double u;
    double v;
    bool front_face;

    void set_face_normal(const ray&r, const vec& outward_normal)
    {
        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};


#endif //RAYTRACING_RECORD_H
