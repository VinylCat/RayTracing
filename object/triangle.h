//
// Created by W9990 on 2023/12/3.
//

#ifndef RAYTRACING_TRIANGLE_H
#define RAYTRACING_TRIANGLE_H

#include "object.h"
#include "../material.h"


class triangle : public object
{
public:
    triangle(){}
    triangle(point vertex0, point vertex1, point vertex2, shared_ptr<material> _material)
             :vertex0(vertex0), vertex1(vertex1), vertex2(vertex2), mat(_material){
    point pmin;
    pmin.x = fmin(vertex2.x, fmin(vertex0.x, vertex1.x));
    pmin.y = fmin(vertex2.y, fmin(vertex0.y, vertex1.y));
    pmin.z = fmin(vertex2.z, fmin(vertex0.z, vertex1.z));
    point pmax;
    pmax.x = fmax(vertex2.x, fmax(vertex0.x, vertex1.x));
    pmax.y = fmax(vertex2.y, fmax(vertex0.y, vertex1.y));
    pmax.z = fmax(vertex2.z, fmax(vertex0.z, vertex1.z));
    bbox = aabb(pmin, pmax).pad();
    vec edge1, edge2;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    normal = glm::normalize(glm::cross(edge1, edge2));
    };
    triangle(point vertex0, point vertex1, point vertex2,
             glm::dvec2 uv0, glm::dvec2 uv1, glm::dvec2 uv2, shared_ptr<material> _material)
            :vertex0(vertex0), vertex1(vertex1), vertex2(vertex2), uv0(uv0), uv1(uv1), uv2(uv2), mat(_material){
        point pmin;
        pmin.x = fmin(vertex2.x, fmin(vertex0.x, vertex1.x));
        pmin.y = fmin(vertex2.y, fmin(vertex0.y, vertex1.y));
        pmin.z = fmin(vertex2.z, fmin(vertex0.z, vertex1.z));
        point pmax;
        pmax.x = fmax(vertex2.x, fmax(vertex0.x, vertex1.x));
        pmax.y = fmax(vertex2.y, fmax(vertex0.y, vertex1.y));
        pmax.z = fmax(vertex2.z, fmax(vertex0.z, vertex1.z));
        bbox = aabb(pmin, pmax).pad();
        vec edge1, edge2;
        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;
        normal = glm::normalize(glm::cross(edge1, edge2));
    };

    bool intersection(const ray&r, interval ray_t, hit_record& rec) const override {

        vec edge1, edge2, pvec, tvec, q;
        double det, invDet, u, v;
        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;
        pvec = glm::cross(r.direction(), edge2);
        det = glm::dot(edge1, pvec);

        if (det < ray_t.min && det > -ray_t.min)
            return false;
        /*if (det < ray_t.min || det == 0)
            return false;*/

        invDet = 1.0 / det;
        tvec = r.origin() - vertex0;
        u = invDet * glm::dot(tvec, pvec);
        if (u < 0.0 || u > 1.0)
            return false;

        q = glm::cross(tvec, edge1);
        v = invDet * glm::dot(r.direction(), q);
        if (v < 0.0 || u + v > 1.0)
            return false;

        double t = invDet * glm::dot(edge2, q);

        if ( t > ray_t.min){
            rec.t = t;
            rec.p = r.at(rec.t);
            rec.mat = mat;
            rec.set_face_normal(r, normal);
            glm::dvec2 uv = (1 - u - v) * uv0 + u * uv1 + v * uv2;
            rec.u = uv.x;
            rec.v = uv.y;
            return true;
        }
        else
            return false;
    }
    aabb bounding_box() const override { return bbox; }

private:
    point vertex0, vertex1, vertex2;
    glm::dvec2 uv0, uv1, uv2;
    shared_ptr<material> mat;
    aabb bbox;
    point normal;
};

#endif //RAYTRACING_TRIANGLE_H
