#ifndef PDF_H
#define PDF_H

#include "rtweekend.h"
#include "hittable.h"
#include "onb.h"

class pdf {
public:
    virtual double value(const vec3& direction) const = 0;
    virtual vec3 generate() const = 0;
};

class cosine_pdf : public pdf {
public:
    onb uvw;
public:
    cosine_pdf(const vec3& w) { uvw.build_from_w(w); }

    virtual double value(const vec3& direction) const override {
        double cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine/pi;
    }

    virtual vec3 generate() const override {
        return uvw.local(random_cosine_direction());
    }
};

class hittable_pdf : public pdf {
public:
    point3 o;
    shared_ptr<hittable> ptr;
public:
    hittable_pdf(shared_ptr<hittable> p, const point3& origin) :
        o(origin), ptr(p) {}

    virtual double value(const vec3& direction) const override {
        return ptr->pdf_value(o, direction);
    }

    virtual vec3 generate() const override {
        return ptr->random(o);
    }
};

#endif