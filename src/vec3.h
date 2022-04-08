#ifndef VECTOR_3
#define VECTOR_3

#include <array>

template <typename T> class vec3 {
public:
  constexpr vec3() = default;
  constexpr vec3(T a, T b, T c) : d_{a, b, c} {}

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

using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec3i = vec3<int>;

#endif