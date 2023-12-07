/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <yoga/YGEnums.h>
#include <yoga/YGMacros.h>

FBYG_EXTERN_C_BEGIN

typedef struct FBYGValue {
  float value;
  FBYGUnit unit;
} FBYGValue;

YOGA_EXPORT extern const FBYGValue FBYGValueAuto;
YOGA_EXPORT extern const FBYGValue FBYGValueUndefined;
YOGA_EXPORT extern const FBYGValue FBYGValueZero;

FBYG_EXTERN_C_END

#ifdef __cplusplus
#include <limits>
constexpr float FBYGUndefined = std::numeric_limits<float>::quiet_NaN();
#else
#include <math.h>
#define FBYGUndefined NAN
#endif

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
