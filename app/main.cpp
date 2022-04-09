#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>

#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "misc.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

constexpr size_t samples_per_pixel = 100;

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
  camera<double> cam;

  // World

  hitable_list<double> world{
      std::make_shared<sphere<double>>(point3d{0, 0, -1}, 0.5),
      std::make_shared<sphere<double>>(point3d{0, -100.5, -1}, 100)};

  // Render
  auto color = [&, x = 0, y = image_height]() mutable {
    color3d pixel_color{0.0, 0.0, 0.0};

    for (int s = 0; s < samples_per_pixel; s++) {
      const auto u = (double(x) + random_double()) / (image_width - 1);
      const auto v = (double(y) + random_double()) / (image_height - 1);
      ray<double> r = cam.get_ray(u, v);
      pixel_color += ray_color(r, world);
    }

    x++;
    if (x == image_width) {
      x = 0;
      y--;
    }

    return pixel_color;
  };
  std::generate(screen.begin(), screen.end(), color);

  // Save to file
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (const auto &pix : screen) {
    static constexpr double scale = 1.0 / samples_per_pixel;

    auto r = pix.r() * scale;
    auto g = pix.g() * scale;
    auto b = pix.b() * scale;

    std::cout << static_cast<int>(256 * clampd<0.0, 0.999>(r)) << ' '
              << static_cast<int>(256 * clampd<0.0, 0.999>(g)) << ' '
              << static_cast<int>(256 * clampd<0.0, 0.999>(b)) << '\n';
  }
  return 0;
}
