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

enum class Align : uint8_t {
  Auto = FBYGAlignAuto,
  FlexStart = FBYGAlignFlexStart,
  Center = FBYGAlignCenter,
  FlexEnd = FBYGAlignFlexEnd,
  Stretch = FBYGAlignStretch,
  Baseline = FBYGAlignBaseline,
  SpaceBetween = FBYGAlignSpaceBetween,
  SpaceAround = FBYGAlignSpaceAround,
  SpaceEvenly = FBYGAlignSpaceEvenly,
};

template <>
constexpr int32_t ordinalCount<Align>() {
  return 9;
}

constexpr Align scopedEnum(FBYGAlign unscoped) {
  return static_cast<Align>(unscoped);
}

constexpr FBYGAlign unscopedEnum(Align scoped) {
  return static_cast<FBYGAlign>(scoped);
}

inline const char* toString(Align e) {
  return FBYGAlignToString(unscopedEnum(e));
}

} // namespace facebookyg::yoga
