/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdbool.h>

#include <yoga/YGEnums.h>
#include <yoga/YGMacros.h>

/**
 * Float value to represent "undefined" in style values.
 */
#ifdef __cplusplus
#include <limits>
constexpr float FBYGUndefined = std::numeric_limits<float>::quiet_NaN();
#else
#include <math.h>
#define FBYGUndefined NAN
#endif

FBYG_EXTERN_C_BEGIN

/**
 * Structure used to represent a dimension in a style.
 */
typedef struct FBYGValue {
  float value;
  FBYGUnit unit;
} FBYGValue;

/**
 * Constant for a dimension of "auto".
 */
FBYG_EXPORT extern const FBYGValue FBYGValueAuto;

/**
 * Constant for a dimension which is not defined.
 */
FBYG_EXPORT extern const FBYGValue FBYGValueUndefined;

/**
 * Constant for a dimension that is zero-length.
 */
FBYG_EXPORT extern const FBYGValue FBYGValueZero;

/**
 * Whether a dimension represented as a float is defined.
 */
FBYG_EXPORT bool FBYGFloatIsUndefined(float value);

FBYG_EXTERN_C_END

// Equality operators for comparison of FBYGValue in C++
#ifdef __cplusplus
inline bool operator==(const FBYGValue& lhs, const FBYGValue& rhs) {
  if (lhs.unit != rhs.unit) {
    return false;
  }

  switch (lhs.unit) {
    case FBYGUnitUndefined:
    case FBYGUnitAuto:
      return true;
    case FBYGUnitPoint:
    case FBYGUnitPercent:
      return lhs.value == rhs.value;
  }

  return false;
}

inline bool operator!=(const FBYGValue& lhs, const FBYGValue& rhs) {
  return !(lhs == rhs);
}

inline FBYGValue operator-(const FBYGValue& value) {
  return {-value.value, value.unit};
}
#endif
