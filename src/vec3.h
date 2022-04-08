#ifndef VECTOR_3
#define VECTOR_3

#include <array>
enum class type { point, vector, color };

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

using vec3f = vec3<type::vector, float>;
using vec3d = vec3<type::vector, double>;
using vec3i = vec3<type::vector, int>;

using point3f = vec3<type::point, float>;
using point3d = vec3<type::point, double>;
using point3i = vec3<type::point, int>;

using color3f = vec3<type::color, float>;
using color3d = vec3<type::color, double>;
using color3i = vec3<type::color, int>;

#endif