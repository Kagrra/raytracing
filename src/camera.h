#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

template <typename T> class camera {
public:
  constexpr camera() noexcept {
    constexpr auto aspect_ratio = 16.0 / 9.0;
    constexpr auto viewport_height = 2.0;
    constexpr auto viewport_width = aspect_ratio * viewport_height;
    constexpr auto focal_length = 1.0;

    horizontal_ = point3d(viewport_width, 0, 0);
    vertical_ = point3d(0, viewport_height, 0);
    lower_left_corner_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 -
                         point3d(0, 0, focal_length);
  }

  constexpr ray<T> get_ray(T u, T v) const noexcept {
    return ray<T>(origin_, interpret_as<type::direction>(
                               lower_left_corner_ + u * horizontal_ +
                               v * vertical_ - origin_));
  }

private:
  point<T> origin_{0, 0, 0};
  point<T> horizontal_;
  point<T> vertical_;
  point<T> lower_left_corner_;
};

#endif