//
// Created by W9990 on 2023/11/21.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "constants.h"
#include "scene.h"
#include "texture.h"

class hit_record;

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(
            const ray& r_in, const hit_record&, color& attenuation, ray& scattered) const = 0;

    virtual color emitted(double u, double v, const point& p) const {
        return color(0,0,0);
    }

};

class lambertian : public material{
public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}
    bool scatter(const ray& r_in,  const hit_record& rec, color& attenuation, ray& scattered)
    const override{
        auto scatter_direction = rec.normal + random_unit_vector();

        if (vec_tools::near_zero(scatter_direction))
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    shared_ptr<texture> albedo;

};

class metal : public material{
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1){}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override{
        vec tmp_reflected = glm::normalize(r_in.direction());
        vec reflected = reflect(tmp_reflected, rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (glm::dot(scattered.direction(), rec.normal) > 0);

    }
private:
    color albedo;
    double fuzz;
};

class dielectric : public material{
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction){}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override{
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec unit_direction = glm::normalize(r_in.direction());

        double cos_theta = fmin(glm::dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec direction;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction, r_in.time());

        return true;
    }
private:
    double ir;

    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class diffuse_light : public material {
public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        return false;
    }

    color emitted(double u, double v, const point& p) const override {
        return emit->value(u, v, p);
    }

private:
    shared_ptr<texture> emit;
};

class isotropic : public material {
public:
    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        scattered = ray(rec.p, random_unit_vector(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    shared_ptr<texture> albedo;
};

#endif //RAYTRACING_MATERIAL_H
