#ifndef RAY_3
#define RAY_3

#include "vec3.h"

template <typename T> class ray {
public:
  constexpr ray() = default;
  constexpr ray(const point<T> &origin, const dir<T> &dir)
      : origin_{origin}, dir_{dir} {}

  constexpr point<T> origin() const noexcept { return origin_; }
  constexpr dir<T> direction() const noexcept { return dir_; }

  constexpr point<T> at(T t) const noexcept { return origin_ + t * dir_; }

private:
  point<T> origin_;
  dir<T> dir_;
};

#endif