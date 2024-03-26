//
// Created by W9990 on 2023/11/16.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "vec_tools.h"

class ray{
public:
    ray() {}

    ray(const point& origin, const vec& direction) : orig(origin), dir(direction), tm(0)
    {}

    ray(const point& origin, const vec& direction, double time = 0.0)
            : orig(origin), dir(direction), tm(time)
    {}

    point origin() const { return orig; }
    vec direction() const { return dir; }
    double time() const    { return tm; }

    point at(double t) const
    {
        return orig + t * dir;
    }
     void dir_set(vec a)
     {
        dir.x = a.x;
        dir.y = a.y;
        dir.z = a.z;
     }


private:
    point orig;
    vec dir;
    double tm;
};


#endif //RAYTRACING_RAY_H
