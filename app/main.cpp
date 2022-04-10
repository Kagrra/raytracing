#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <numeric>

#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "material.h"
#include "misc.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

constexpr size_t samples_per_pixel = 100;
constexpr size_t bounces = 50;

color3d ray_color(const ray<double> &r, const hitable<double> &world,
                  size_t depth) {
  if (depth == 0)
    return color3d(0.0, 0.0, 0.0);

  auto rec = world.hit(r, 0.001, std::numeric_limits<double>::infinity());
  if (rec) {
    /*
    point3d target = rec.value().p + rec.value().normal +
                     unit_vector(point3d::random_in_unit_sphere());
    return 0.5 *
           ray_color(ray<double>(rec.value().p, interpret_as<type::direction>(
                                                    target - rec.value().p)),
                     world, depth - 1);
                     */
    auto scatter = rec.value().mat->scatter(r, rec.value());
    if (scatter)
      return std::get<0>(scatter.value()) *
             ray_color(std::get<1>(scatter.value()), world, depth - 1);
    return color3d(0.0, 0.0, 0.0);
  }
  dir3d unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color3d(1.0, 1.0, 1.0) + t * color3d(0.5, 0.7, 1.0);
}

int main() {
  std::cerr << "Time start!\n";
  auto timer = std::chrono::system_clock::now();

  // Image
  constexpr auto aspect_ratio = 16.0 / 9.0;
  constexpr size_t image_width = 800;
  constexpr size_t image_height = static_cast<int>(image_width / aspect_ratio);
  std::array<color3d, image_height * image_width> screen;

  // Camera
  camera<double> cam;

  // World
  lambertian<double> matte_red{color3d{0.8, 0.3, 0.4}};
  lambertian<double> matte_grey{color3d{0.7, 0.7, 0.7}};
  lambertian<double> matte_white{color3d{1.0, 1.0, 1.0}};
  lambertian<double> matte_green{color3d{0.2, 0.5, 0.1}};

  hitable_list<double> world{
      std::make_shared<sphere<double>>(point3d{0.7, -0.3, -0.8}, 0.2,
                                       matte_grey),
      std::make_shared<sphere<double>>(point3d{-0.7, -0.3, -0.8}, 0.2,
                                       matte_white),
      std::make_shared<sphere<double>>(point3d{0.0, 0.0, -1.0}, 0.5, matte_red),
      std::make_shared<sphere<double>>(point3d{0, -100.5, -1}, 100,
                                       matte_green)};

  // Render
  auto color = [&, x = 0, y = image_height]() mutable {
    color3d pixel_color{0.0, 0.0, 0.0};

    for (int s = 0; s < samples_per_pixel; s++) {
      const auto u = (double(x) + random_double()) / (image_width - 1);
      const auto v = (double(y) + random_double()) / (image_height - 1);
      ray<double> r = cam.get_ray(u, v);
      pixel_color += ray_color(r, world, bounces);
    }

    x++;
    if (x == image_width) {
      x = 0;
      y--;
    }

    return pixel_color;
  };

  std::cerr << "Setup: "
            << std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now() - timer)
            << "\n";
  timer = std::chrono::system_clock::now();

  std::generate(screen.begin(), screen.end(), color);

  std::cerr << "Render: "
            << std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now() - timer)
            << "\n";
  timer = std::chrono::system_clock::now();

  // Save to file
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (const auto &pix : screen) {
    static constexpr double scale = 1.0 / samples_per_pixel;

    auto r = sqrt(pix.r() * scale);
    auto g = sqrt(pix.g() * scale);
    auto b = sqrt(pix.b() * scale);

    std::cout << static_cast<int>(256 * clampd<0.0, 0.999>(r)) << ' '
              << static_cast<int>(256 * clampd<0.0, 0.999>(g)) << ' '
              << static_cast<int>(256 * clampd<0.0, 0.999>(b)) << '\n';
  }

  std::cerr << "Save: "
            << std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now() - timer)
            << "\n";

  return 0;
}
