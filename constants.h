//
// Created by W9990 on 2023/11/18.
//

#ifndef RAYTRACING_CONSTANTS_H
#define RAYTRACING_CONSTANTS_H


#include <limits>
#include <memory>
#include <cstdlib>
#include <cmath>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


const double pi = 3.1415926535897932385;


inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double random_double()
{
    return rand() / ( RAND_MAX + 1.0 );
}

inline double random_double(double min, double max)
{
    return min + ( max - min) * random_double();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max+1));
}

#include "interval.h"
#include "ray.h"
#include "vec_tools.h"

#endif //RAYTRACING_CONSTANTS_H
