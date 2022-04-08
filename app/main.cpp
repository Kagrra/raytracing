#include <algorithm>
#include <array>
#include <iostream>

#include "vec3.h"

int main() {

  constexpr size_t image_width = 512;
  constexpr size_t image_height = 512;

  std::array<vec3d, image_height * image_width> data;

  auto color = [&, i = 0]() mutable {
    return vec3d{static_cast<double>(i % image_width) / (image_width - 1),
                 static_cast<double>(i++ / image_width) / (image_height - 1),
                 0.25};
  };

  std::generate(data.begin(), data.end(), color);

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (const auto &pix : data) {
    std::cout << static_cast<int>(255.999 * pix.r()) << ' '
              << static_cast<int>(255.999 * pix.g()) << ' '
              << static_cast<int>(255.999 * pix.b()) << '\n';
  }
  return 0;
}
