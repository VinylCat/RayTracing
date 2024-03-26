//
// Created by W9990 on 2023/12/3.
//

#ifndef RAYTRACING_CONSTANT_MEDIUM_H
#define RAYTRACING_CONSTANT_MEDIUM_H

#include "../constants.h"

#include "object.h"
#include "../material.h"
#include "../texture.h"

class constant_medium : public object {
public:
    constant_medium(shared_ptr<object> b, double d, shared_ptr<texture> a)
            : boundary(b), neg_inv_density(-1/d), phase_function(make_shared<isotropic>(a))
    {}

    constant_medium(shared_ptr<object> b, double d, color c)
            : boundary(b), neg_inv_density(-1/d), phase_function(make_shared<isotropic>(c))
    {}

    bool intersection(const ray& r, interval ray_t, hit_record& rec) const override {
        // Print occasional samples when debugging. To enable, set enableDebug true.
        const bool enableDebug = false;
        const bool debugging = enableDebug && random_double() < 0.00001;

        hit_record rec1, rec2;

        if (!boundary->intersection(r, interval::universe, rec1))
            return false;

        if (!boundary->intersection(r, interval(rec1.t + 0.0001, infinity), rec2))
            return false;

        if (debugging) std::clog << "\nray_tmin=" << rec1.t << ", ray_tmax=" << rec2.t << '\n';

        if (rec1.t < ray_t.min) rec1.t = ray_t.min;
        if (rec2.t > ray_t.max) rec2.t = ray_t.max;

        if (rec1.t >= rec2.t)
            return false;

        if (rec1.t < 0)
            rec1.t = 0;

        auto ray_length = r.direction().length();
        auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
        auto hit_distance = neg_inv_density * log(random_double());

        if (hit_distance > distance_inside_boundary)
            return false;

        rec.t = rec1.t + hit_distance / ray_length;
        rec.p = r.at(rec.t);

        rec.normal = vec(1,0,0);  // arbitrary
        rec.front_face = true;     // also arbitrary
        rec.mat = phase_function;

        return true;
    }

    aabb bounding_box() const override { return boundary->bounding_box(); }

private:
    shared_ptr<object> boundary;
    double neg_inv_density;
    shared_ptr<material> phase_function;
};

#endif //RAYTRACING_CONSTANT_MEDIUM_H
