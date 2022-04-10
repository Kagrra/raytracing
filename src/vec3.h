#ifndef VECTOR_3
#define VECTOR_3

#include "misc.h"
#include <array>
#include <ostream>

enum class type { point, direction, color };

template <type Type, typename T> class vec3 {
public:
  constexpr vec3() = default;
  constexpr vec3(T a, T b, T c) : d_{a, b, c} {}

  constexpr vec3(const vec3 &src) = default;
  constexpr vec3(vec3 &&src) = default;

  constexpr vec3 &operator=(const vec3 &src) = default;
  constexpr vec3 &operator=(vec3 &&src) = default;

  constexpr T &r() noexcept { return d_[0]; }
  constexpr T &g() noexcept { return d_[1]; }
  constexpr T &b() noexcept { return d_[2]; }

  constexpr const T &r() const noexcept { return d_[0]; }
  constexpr const T &g() const noexcept { return d_[1]; }
  constexpr const T &b() const noexcept { return d_[2]; }

  constexpr T &x() noexcept { return d_[0]; }
  constexpr T &y() noexcept { return d_[1]; }
  constexpr T &z() noexcept { return d_[2]; }

  constexpr const T &x() const noexcept { return d_[0]; }
  constexpr const T &y() const noexcept { return d_[1]; }
  constexpr const T &z() const noexcept { return d_[2]; }

  constexpr vec3 operator-() const { return vec3(-d_[0], -d_[1], -d_[2]); }
  constexpr T &operator[](size_t i) noexcept { return d_[i]; }
  constexpr const T &operator[](size_t i) const noexcept { return d_[i]; }

  template <size_t N>
  requires(N >= 0 && N < 3) constexpr T &get() noexcept { return d_[N]; }

  template <size_t N>
  requires(N >= 0 && N < 3) constexpr const T &get() const noexcept {
    return d_[N];
  }

  constexpr vec3 &operator+=(const vec3 &v) noexcept {
    d_[0] += v.d_[0];
    d_[1] += v.d_[1];
    d_[2] += v.d_[2];
    return *this;
  }

  constexpr vec3 &operator*=(const T t) noexcept {
    d_[0] *= t;
    d_[1] *= t;
    d_[2] *= t;
    return *this;
  }

  constexpr vec3 &operator/=(const T t) noexcept {
    d_[0] /= t;
    d_[1] /= t;
    d_[2] /= t;
    return *this;
  }

  constexpr T length() const noexcept { return sqrt(length_squared()); }
  constexpr T length_squared() const noexcept {
    return d_[0] * d_[0] + d_[1] * d_[1] + d_[2] * d_[2];
  }

  constexpr void normalize() noexcept {
    auto len = this->length();
    d_[0] /= len;
    d_[1] /= len;
    d_[2] /= len;
  }

  constexpr static vec3 random(T min, T max) {
    return vec3(random_double(min, max), //
                random_double(min, max), //
                random_double(min, max));
  }

  constexpr static vec3 random_in_unit_sphere() {
    while (true) {
      auto p = random(-1.0, 1.0);
      if (p.length_squared() >= 1.0)
        continue;
      return p;
    }
  }

  constexpr static vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
  }

  constexpr bool near_zero() {
    static constexpr T e = 1e-8;
    return (fabs(d_[0]) < e) && (fabs(d_[1]) < e) && (fabs(d_[2]) < e);
  }

private:
  std::array<T, 3> d_;
};

template <type Type, typename T>
std::ostream &operator<<(std::ostream &out, const vec3<Type, T> &v) {
  return out << v.get<0>() << ' ' << v.get<1>() << ' ' << v.get<2>();
}

template <type Type, typename T>
constexpr vec3<Type, T> operator+(const vec3<Type, T> &rhs,
                                  const vec3<Type, T> &lhs) {
  return vec3<Type, T>{rhs.get<0>() + lhs.get<0>(), rhs.get<1>() + lhs.get<1>(),
                       rhs.get<2>() + lhs.get<2>()};
}

template <type Type, typename T>
constexpr vec3<Type, T> operator-(const vec3<Type, T> &rhs,
                                  const vec3<Type, T> &lhs) {
  return vec3<Type, T>{rhs.get<0>() - lhs.get<0>(), rhs.get<1>() - lhs.get<1>(),
                       rhs.get<2>() - lhs.get<2>()};
}

template <type Type, typename T>
constexpr vec3<Type, T> operator*(const vec3<Type, T> &rhs,
                                  const vec3<Type, T> &lhs) {
  return vec3<Type, T>{rhs.get<0>() * lhs.get<0>(), rhs.get<1>() * lhs.get<1>(),
                       rhs.get<2>() * lhs.get<2>()};
}

template <type Type, typename T>
constexpr vec3<Type, T> operator*(T rhs, const vec3<Type, T> &lhs) {
  return vec3<Type, T>{rhs * lhs.get<0>(), rhs * lhs.get<1>(),
                       rhs * lhs.get<2>()};
}

template <type Type, typename T>
constexpr vec3<Type, T> operator*(const vec3<Type, T> &rhs, double lhs) {
  return vec3<Type, T>{rhs.get<0>() * lhs, rhs.get<1>() * lhs,
                       rhs.get<2>() * lhs};
}

template <type Type, typename T>
constexpr vec3<Type, T> operator/(const vec3<Type, T> &lhs, T rhs) {
  return (1 / rhs) * lhs;
}

template <type Type, typename T>
constexpr T dot(const vec3<Type, T> &rhs, const vec3<Type, T> &lhs) {
  return rhs.get<0>() * lhs.get<0>() + rhs.get<1>() * lhs.get<1>() +
         rhs.get<2>() * lhs.get<2>();
}

template <type Type, typename T>
constexpr vec3<Type, T> cross(const vec3<Type, T> &lhs,
                              const vec3<Type, T> &rhs) {
  return vec3<Type, T>{
      lhs.get<1>() * rhs.get<2>() - lhs.get<2>() * rhs.get<1>(),
      lhs.get<2>() * rhs.get<0>() - lhs.get<0>() * rhs.get<2>(),
      lhs.get<0>() * rhs.get<1>() - lhs.get<1>() * rhs.get<0>()};
}

template <type Type, typename T>
constexpr vec3<Type, T> unit_vector(const vec3<Type, T> &v) {
  return v / v.length();
}

template <typename T> using dir = vec3<type::direction, T>;
using dir3f = dir<float>;
using dir3d = dir<double>;
using dir3i = dir<int>;

template <typename T> using point = vec3<type::point, T>;
using point3f = point<float>;
using point3d = point<double>;
using point3i = point<int>;

template <typename T> using color = vec3<type::color, T>;
using color3f = color<float>;
using color3d = color<double>;
using color3i = color<int>;

template <type To, type From, typename T>
constexpr vec3<To, T> interpret_as(const vec3<From, T> &src) {
  return vec3<To, T>{src.get<0>(), src.get<1>(), src.get<2>()};
}

template <typename T>
constexpr point<T> operator+(const point<T> &rhs, const dir<T> &lhs) {
  return point<T>{rhs.get<0>() + lhs.get<0>(), rhs.get<1>() + lhs.get<1>(),
                  rhs.get<2>() + lhs.get<2>()};
}

template <type Type1, type Type2, typename T>
constexpr T dot(const vec3<Type1, T> &rhs, const vec3<Type2, T> &lhs) {
  return rhs.get<0>() * lhs.get<0>() + rhs.get<1>() * lhs.get<1>() +
         rhs.get<2>() * lhs.get<2>();
}

#endif