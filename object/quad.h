//
// Created by W9990 on 2023/12/3.
//

#ifndef RAYTRACING_QUAD_H
#define RAYTRACING_QUAD_H

#include "../constants.h"
#include <cmath>
#include "object.h"
#include "../scene.h"



class quad : public object {
public:
    quad(const point& _Q, const vec& _u, const vec& _v, shared_ptr<material> m)
            : Q(_Q), u(_u), v(_v), mat(m)
    {
        auto n = cross(u, v);
        normal = glm::normalize(n);
        D = dot(normal, Q);
        w = n / glm::dot(n,n);
        set_bounding_box();
    }

    virtual void set_bounding_box() {
        bbox = aabb(Q, Q + u + v).pad();
    }

    aabb bounding_box() const override { return bbox; }

    bool intersection(const ray& r, interval ray_t, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        // No intersection if the ray is parallel to the plane.
        if (fabs(denom) < 1e-8)
            return false;

        // Return false if the intersection point parameter t is outside the ray interval.
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        // Determine the intersection point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        vec planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the intersection record and return true.

        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double a, double b, hit_record& rec) const {
        // Given the intersection point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the intersection record UV coordinates and return true.

        if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    point Q;
    vec u, v;
    shared_ptr<material> mat;
    aabb bbox;
    vec normal;
    double D;
    vec w;
};


inline shared_ptr<scene> box(const point& a, const point& b, shared_ptr<material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = make_shared<scene>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    auto max = point(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    auto dx = vec(max.x - min.x, 0, 0);
    auto dy = vec(0, max.y - min.y, 0);
    auto dz = vec(0, 0, max.z - min.z);

    sides->add(make_shared<quad>(point(min.x, min.y, max.z),  dx,  dy, mat)); // front
    sides->add(make_shared<quad>(point(max.x, min.y, max.z), -dz,  dy, mat)); // right
    sides->add(make_shared<quad>(point(max.x, min.y, min.z), -dx,  dy, mat)); // back
    sides->add(make_shared<quad>(point(min.x, min.y, min.z),  dz,  dy, mat)); // left
    sides->add(make_shared<quad>(point(min.x, max.y, max.z),  dx, -dz, mat)); // top
    sides->add(make_shared<quad>(point(min.x, min.y, min.z),  dx,  dz, mat)); // bottom

    return sides;
}


#endif //RAYTRACING_QUAD_H
