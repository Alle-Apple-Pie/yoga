/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cmath>
#include <limits>
#include "Yoga-internal.h"

struct FBYGFloatOptional {
private:
  float value_ = std::numeric_limits<float>::quiet_NaN();

public:
  explicit constexpr FBYGFloatOptional(float value) : value_(value) {}
  constexpr FBYGFloatOptional() = default;

  // returns the wrapped value, or a value x with FBYGIsUndefined(x) == true
  constexpr float unwrap() const { return value_; }

  bool isUndefined() const { return std::isnan(value_); }
};

// operators take FBYGFloatOptional by value, as it is a 32bit value

inline bool operator==(FBYGFloatOptional lhs, FBYGFloatOptional rhs) {
  return lhs.unwrap() == rhs.unwrap() ||
      (lhs.isUndefined() && rhs.isUndefined());
}
inline bool operator!=(FBYGFloatOptional lhs, FBYGFloatOptional rhs) {
  return !(lhs == rhs);
}

inline bool operator==(FBYGFloatOptional lhs, float rhs) {
  return lhs == FBYGFloatOptional{rhs};
}
inline bool operator!=(FBYGFloatOptional lhs, float rhs) {
  return !(lhs == rhs);
}

inline bool operator==(float lhs, FBYGFloatOptional rhs) {
  return rhs == lhs;
}
inline bool operator!=(float lhs, FBYGFloatOptional rhs) {
  return !(lhs == rhs);
}

inline FBYGFloatOptional operator+(FBYGFloatOptional lhs, FBYGFloatOptional rhs) {
  return FBYGFloatOptional{lhs.unwrap() + rhs.unwrap()};
}

inline bool operator>(FBYGFloatOptional lhs, FBYGFloatOptional rhs) {
  return lhs.unwrap() > rhs.unwrap();
}

inline bool operator<(FBYGFloatOptional lhs, FBYGFloatOptional rhs) {
  return lhs.unwrap() < rhs.unwrap();
}

inline bool operator>=(FBYGFloatOptional lhs, FBYGFloatOptional rhs) {
  return lhs > rhs || lhs == rhs;
}

inline bool operator<=(FBYGFloatOptional lhs, FBYGFloatOptional rhs) {
  return lhs < rhs || lhs == rhs;
}
