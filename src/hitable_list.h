#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.h"
#include "material.h"
#include <memory>
#include <vector>

template <typename T> class hitable_list : public hitable<T> {
public:
  hitable_list() {}
  hitable_list(std::shared_ptr<hitable<T>> object) { add(object); }
  template <typename... U> hitable_list(U... u) : objects_{u...} {}

  void clear() { objects_.clear(); }

  void add(std::shared_ptr<hitable<T>> object) { objects_.push_back(object); }

  constexpr std::optional<hit_data<T>> hit(const ray<T> &r, T t_min,
                                           T t_max) const noexcept override {
    hit_data<T> best_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object : objects_) {
      if (auto temp_rec = object->hit(r, t_min, closest_so_far)) {
        hit_anything = true;
        closest_so_far = temp_rec.value().t;
        best_rec = temp_rec.value();
      }
    }

    return hit_anything ? std::optional<hit_data<T>>{best_rec} : std::nullopt;
  }

public:
  std::vector<std::shared_ptr<hitable<T>>> objects_;
};

#endif