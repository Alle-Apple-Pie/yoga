/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// @generated by enums.py
// clang-format off
#pragma once

#include <cstdint>
#include <yoga/YGEnums.h>
#include <yoga/enums/YogaEnums.h>

namespace facebookyg::yoga {

enum class PositionType : uint8_t {
  Static = FBYGPositionTypeStatic,
  Relative = FBYGPositionTypeRelative,
  Absolute = FBYGPositionTypeAbsolute,
};

template <>
constexpr int32_t ordinalCount<PositionType>() {
  return 3;
}

constexpr PositionType scopedEnum(FBYGPositionType unscoped) {
  return static_cast<PositionType>(unscoped);
}

constexpr FBYGPositionType unscopedEnum(PositionType scoped) {
  return static_cast<FBYGPositionType>(scoped);
}

inline const char* toString(PositionType e) {
  return FBYGPositionTypeToString(unscopedEnum(e));
}

} // namespace facebookyg::yoga
