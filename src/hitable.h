#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "vec3.h"
#include <optional>

template <typename T> struct hit_data {
  point<T> p;
  dir<T> normal;
  T t;
  bool front_face;
};

template <typename T> struct hitable {
  virtual std::optional<hit_data<T>> hit(const ray<T> &r, T t_min,
                                         T t_max) const noexcept = 0;
  virtual ~hitable() = default;
};

#endif