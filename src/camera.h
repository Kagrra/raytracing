#ifndef CAMERA_H
#define CAMERA_H

#include "misc.h"
#include "ray.h"
#include "vec3.h"

template <typename T> class camera {
public:
  constexpr camera(point<T> look_from, point<T> look_at, dir<T> up, T vfov,
                   T aspect_ratio, T aperture, T focus_distance) noexcept {
    auto theta = degrees_to_radians<T>(vfov);
    auto h = tan(theta / 2);

    const auto viewport_height = 2.0 * h;
    const auto viewport_width = aspect_ratio * viewport_height;

    w = interpret_as<type::direction>(unit_vector(look_from - look_at));
    u = unit_vector(cross(up, w));
    v = cross(w, u);

    origin_ = look_from;
    horizontal_ =
        interpret_as<type::point>(focus_distance * viewport_width * u);
    vertical_ = interpret_as<type::point>(focus_distance * viewport_height * v);
    lower_left_corner_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 -
                         focus_distance * interpret_as<type::point>(w);

    lens_radius = aperture / 2;
  }

  constexpr ray<T> get_ray(T s, T t) const noexcept {
    dir<T> rd = lens_radius * dir<T>::random_in_unit_disk();
    point<T> offset = interpret_as<type::point>(u * rd.x() + v * rd.y());

    return ray<T>(origin_ + offset, interpret_as<type::direction>(
                                        lower_left_corner_ + s * horizontal_ +
                                        t * vertical_ - origin_ - offset));
  }

private:
  point<T> origin_{0, 0, 0};
  point<T> horizontal_;
  point<T> vertical_;
  point<T> lower_left_corner_;

  dir<T> u, v, w;
  T lens_radius;
  /*
    point<T> look_at_;
    point<T> look_from_;
    const dri<T> up_;
  */
};

#endif