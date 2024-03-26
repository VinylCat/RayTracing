//
// Created by W9990 on 2023/11/21.
//

#ifndef RAYTRACING_VEC_TOOLS_H
#define RAYTRACING_VEC_TOOLS_H
#include "glm/glm.hpp"
#include <cmath>
#include "constants.h"

using point = glm::dvec3;
using vec = glm::dvec3;
using color = glm::dvec3;
using std::sqrt;
using std::fabs;

class vec_tools
{
public:

    static vec random()
    {
        vec a(random_double(), random_double(), random_double());
        return a;
    }

    static vec random(double min, double max)
    {
        vec a(random_double(min, max), random_double(min, max), random_double(min, max));
        return a;
    }

    static bool near_zero(vec a) {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (fabs(a.x) < s) && (fabs(a.y) < s) && (fabs(a.z) < s);
    }



};


inline vec random_in_unit_sphere()
{
    while(true)
    {
        auto p = vec_tools::random(-1, 1);
        if (glm::length(p) < 1)
            return p;
    }
}

inline vec random_unit_vector()
{
    auto p = glm::normalize(random_in_unit_sphere());
    return p;
}

inline vec random_on_hemisphere(vec& normal) {
    vec on_unit_sphere = random_unit_vector();
    if (glm::dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

vec reflect(const vec& v, const vec& n)
{
    return  v - 2*glm::dot(v, n) * n;
}

inline vec refract(const vec& uv, const vec& n, double etai_over_etat)
{
    auto cos_theta = fmin(glm::dot(-uv, n), 1.0);
    vec r_out_prep = etai_over_etat * (uv + cos_theta * n);
    vec r_out_parallel = -sqrt(fabs(1.0 - glm::length(r_out_prep) * glm::length(r_out_prep))) * n;
    return r_out_prep + r_out_parallel;
}

inline vec random_in_unit_disk()
{
    while(true)
    {
        auto p = vec(random_double(-1, 1), random_double(-1, 1), 0);
        if (glm::length(p) < 1)
            return p;
    }
}

#endif //RAYTRACING_VEC_TOOLS_H
