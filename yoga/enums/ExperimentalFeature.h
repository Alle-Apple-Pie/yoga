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

enum class ExperimentalFeature : uint8_t {
  WebFlexBasis = FBYGExperimentalFeatureWebFlexBasis,
};

template <>
constexpr int32_t ordinalCount<ExperimentalFeature>() {
  return 1;
}

constexpr ExperimentalFeature scopedEnum(FBYGExperimentalFeature unscoped) {
  return static_cast<ExperimentalFeature>(unscoped);
}

constexpr FBYGExperimentalFeature unscopedEnum(ExperimentalFeature scoped) {
  return static_cast<FBYGExperimentalFeature>(scoped);
}

inline const char* toString(ExperimentalFeature e) {
  return FBYGExperimentalFeatureToString(unscopedEnum(e));
}

} // namespace facebookyg::yoga
