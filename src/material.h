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
  metal(const color<T> &albedo, T fuzz = 0) : albedo_{albedo}, fuzz_{fuzz} {}

  std::optional<std::tuple<color<T>, ray<T>>>
  scatter(const ray<T> &r,
          const hit_data<T> &hit_data) const noexcept override {
    auto reflect_dir = reflect(unit_vector(r.direction()), hit_data.normal);
    if (dot(reflect_dir, hit_data.normal) <= 0)
      return {};

    return std::make_tuple(
        albedo_, ray<T>(hit_data.p,
                        reflect_dir + fuzz_ * dir<T>::random_in_unit_sphere()));
  }

private:
  color<T> albedo_;
  T fuzz_;
};

template <typename T> struct dielectric : public material<T> {
public:
  dielectric(T index_of_refraction) : ir_{index_of_refraction} {}

  std::optional<std::tuple<color<T>, ray<T>>>
  scatter(const ray<T> &r,
          const hit_data<T> &hit_data) const noexcept override {
    T refraction_ratio = hit_data.front_face ? 1.0 / ir_ : ir_;
    auto unit_direction = unit_vector(r.direction());

    const double cos_theta = fmin(dot(-unit_direction, hit_data.normal), 1.0);
    const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    const bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    if (cannot_refract)
      return std::make_tuple(
          color<T>{1.0, 1.0, 1.0},
          ray<T>(hit_data.p, reflect(unit_direction, hit_data.normal)));
    else {
      return std::make_tuple(
          color<T>{1.0, 1.0, 1.0},
          ray<T>(hit_data.p,
                 refract(unit_direction, hit_data.normal, refraction_ratio)));
    }
  }

private:
  T ir_;
};

#endif