//
// Created by W9990 on 2023/11/18.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include "object/object.h"

#include <memory>
#include <vector>
#include "aabb.h"


using std::shared_ptr;
using std::make_shared;

class scene : public object{
public:
    std::vector<shared_ptr<object>> objects;

    scene() {}
    scene(shared_ptr<object> object) { add(object);}

    void clear() {objects.clear(); }

    void add(shared_ptr<object> object)
    {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    bool intersection(const ray& r, interval ray_t, hit_record& rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects)
        {
            if(object->intersection(r, interval(ray_t.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
    aabb bounding_box() const override { return bbox; }

private:
    aabb bbox;
};

#endif //RAYTRACING_SCENE_H
