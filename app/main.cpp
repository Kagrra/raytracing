#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>

#include "hitable.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

color3d ray_color(const ray<double> &r, const hitable<double> &world) {
  auto rec = world.hit(r, 0, std::numeric_limits<double>::infinity());
  if (rec) {
    return 0.5 * (color3d(1 + rec.value().normal.x(), //
                          1 + rec.value().normal.y(), //
                          1 + rec.value().normal.z()));
  }
  dir3d unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color3d(1.0, 1.0, 1.0) + t * color3d(0.5, 0.7, 1.0);
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

  // World

  sphere<double> s{{0, 0, -1}, 0.5};

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
    return ray_color(r, s);
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
