#ifndef MISC_H
#define MISC_H

#include <numbers>
#include <random>

inline double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double random_double(double min, double max) {
  static std::uniform_real_distribution<double> distribution(min, max);
  static std::mt19937 generator;
  return distribution(generator);
}

template <typename T, T Min, T Max> struct clamp {
  T operator()(T value) {
    if (value < Min)
      return Min;
    if (value > Max)
      return Max;
    return value;
  }
};

template <float Min, float Max> //
inline clamp<float, Min, Max> clampf{};

template <double Min, double Max> //
inline clamp<double, Min, Max> clampd{};

template <typename T> inline T degrees_to_radians(T degrees) {
  return degrees * std::numbers::pi_v<T> / 180.0;
}

#endif