//
// Created by W9990 on 2023/11/18.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "object.h"

class sphere : public object
{
public:
    // Stationary Sphere
    sphere(point _center, double _radius, shared_ptr<material> _material): center1(_center), radius(_radius), mat(_material), is_moving(false){
        auto rvec = vec(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    }

    // Moving Sphere
    sphere(point _center1, point _center2, double _radius, shared_ptr<material> _material)
            : center1(_center1), radius(_radius), mat(_material), is_moving(true)
    {
        auto rvec = vec(radius, radius, radius);
        aabb box1(_center1 - rvec, _center1 + rvec);
        aabb box2(_center2 - rvec, _center2 + rvec);
        bbox = aabb(box1, box2);
        center_vec = _center2 - _center1;
    }
    bool intersection(const ray& r, interval ray_t, hit_record& rec) const override
    {
        point center = is_moving ? sphere_center(r.time()) : center1;
        vec oc = r.origin() - center;
        auto a = glm::length(r.direction());
        a *= a;
        auto half_b = dot(oc, r.direction());
        auto c = glm::length(oc) * glm::length(oc) - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd =sqrt(discriminant);

        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
            root = (-half_b +sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }
        rec.t = root;
        rec.p = r.at(rec.t);
        vec outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;
        //std::cout<<"intersection true"<<std::endl;
        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    point center1;
    double radius;
    shared_ptr<material> mat;
    bool is_moving;
    vec center_vec;

    point sphere_center(double time) const {
        // Linearly interpolate from center1 to center2 according to time, where t=0 yields
        // center1, and t=1 yields center2.
        return center1 + time*center_vec;
    }

    static void get_sphere_uv(const point& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y);
        auto phi = atan2(-p.z, p.x) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }

    aabb bbox;

};

#endif //RAYTRACING_SPHERE_H
