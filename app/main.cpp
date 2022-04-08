#include <algorithm>
#include <array>
#include <iostream>

#include "ray.h"
#include "vec3.h"

bool hit_sphere(const point3d &center, double radius, const ray<double> &r) {
  point3d oc = r.origin() - center;
  auto a = dot(r.direction(), r.direction());
  auto b = 2.0 * dot(oc, r.direction());
  auto c = dot(oc, oc) - radius * radius;
  auto discriminant = b * b - 4 * a * c;
  return (discriminant > 0);
}

color3d ray_color(const ray<double> &r) {
  if (hit_sphere(point3d(0, 0, -1), 0.5, r))
    return color3d(1, 0, 0);

  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color3d(1.0, 1.0, 1.0) + t * color3d(0.3, 0.6, 1.0);
}

int main() {

  // Image
  constexpr auto aspect_ratio = 16.0 / 9.0;
  constexpr size_t image_width = 800;
  constexpr size_t image_height = static_cast<int>(image_width / aspect_ratio);
  std::array<color3d, image_height * image_width> screen;

  // Camera
  constexpr auto viewport_height = 2.0;
  constexpr auto viewport_width = aspect_ratio * viewport_height;
  constexpr auto focal_length = 1.0;

  constexpr auto origin = point3d(0, 0, 0);
  constexpr auto horizontal = point3d(viewport_width, 0, 0);
  constexpr auto vertical = point3d(0, viewport_height, 0);
  constexpr auto lower_left_corner =
      origin - horizontal / 2.0 - vertical / 2.0 - point3d(0, 0, focal_length);

  // Render
  auto color = [&, x = 0, y = image_height]() mutable {
    const auto u = double(x) / (image_width - 1);
    const auto v = double(y) / (image_height - 1);
    auto dir = lower_left_corner + u * horizontal + v * vertical - origin;

    x++;
    if (x == image_width) {
      x = 0;
      y--;
    }

    ray<double> r(origin, {dir.x(), dir.y(), dir.z()});
    return ray_color(r);
  };
  std::generate(screen.begin(), screen.end(), color);

  // Save to file
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (const auto &pix : screen) {
    std::cout << static_cast<int>(255.999 * pix.r()) << ' '
              << static_cast<int>(255.999 * pix.g()) << ' '
              << static_cast<int>(255.999 * pix.b()) << '\n';
  }
  return 0;
}
