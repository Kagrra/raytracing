#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"
#include "material.h"
#include "vec3.h"

template <typename T> class sphere : public hitable<T> {
public:
  constexpr sphere(point<T> center, T radius, material<T> &mat)
      : center_{center}, radius_{radius}, mat_{mat} {}

  constexpr std::optional<hit_data<T>> hit(const ray<T> &r, T t_min,
                                           T t_max) const noexcept override {
    dir<T> oc = interpret_as<type::direction>(r.origin() - center_);
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius_ * radius_;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
      return {};
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
      root = (-half_b + sqrtd) / a;
      if (root < t_min || t_max < root)
        return {};
    }

    hit_data<T> ret;
    ret.p = r.at(root);
    ret.t = root;
    auto outward_normal =
        interpret_as<type::direction>((ret.p - center_) / radius_);

    ret.front_face = dot(r.direction(), outward_normal) < 0;
    ret.normal = ret.front_face ? outward_normal : -outward_normal;
    ret.mat = &mat_;
    return ret;
  }

private:
  point<T> center_;
  T radius_;
  material<T> &mat_;
};

#endif