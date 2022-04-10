#ifndef MATERIAL_H
#define MATERIAL_H

#include <optional>
#include <tuple>

#include "hitable.h"
#include "ray.h"
#include "vec3.h"

template <typename T> struct material {
public:
  virtual std::optional<std::tuple<color<T>, ray<T>>>
  scatter(const ray<T> &r, const hit_data<T> &hit_data) const noexcept = 0;

  virtual ~material() = default;
};

template <typename T> struct lambertian : public material<T> {
public:
  lambertian(const color<T> &albedo) : albedo_{albedo} {}

  std::optional<std::tuple<color<T>, ray<T>>>
  scatter(const ray<T> &r,
          const hit_data<T> &hit_data) const noexcept override {
    auto scatter_dir = hit_data.normal + dir<T>::random_unit_vector();

    if (scatter_dir.near_zero())
      scatter_dir = hit_data.normal;

    return std::make_tuple(albedo_, ray<T>(hit_data.p, scatter_dir));
  }

private:
  color<T> albedo_;
};

template <typename T> struct metal : public material<T> {
public:
  metal(const color<T> &albedo) : albedo_{albedo} {}

  std::optional<std::tuple<color<T>, ray<T>>>
  scatter(const ray<T> &r,
          const hit_data<T> &hit_data) const noexcept override {
    auto reflect_dir = reflect(unit_vector(r.direction()), hit_data.normal);
    if (dot(reflect_dir, hit_data.normal) <= 0)
      return {};

    return std::make_tuple(albedo_, ray<T>(hit_data.p, reflect_dir));
  }

private:
  color<T> albedo_;
};
#endif