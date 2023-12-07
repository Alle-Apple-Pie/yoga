/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Utils.h"
#include <stdexcept>

using namespace facebook;

FBYGFlexDirection FBYGFlexDirectionCross(
    const FBYGFlexDirection flexDirection,
    const FBYGDirection direction) {
  return FBYGFlexDirectionIsColumn(flexDirection)
      ? FBYGResolveFlexDirection(FBYGFlexDirectionRow, direction)
      : FBYGFlexDirectionColumn;
}

float FBYGFloatMax(const float a, const float b) {
  if (!yoga::isUndefined(a) && !yoga::isUndefined(b)) {
    return fmaxf(a, b);
  }
  return yoga::isUndefined(a) ? b : a;
}

float FBYGFloatMin(const float a, const float b) {
  if (!yoga::isUndefined(a) && !yoga::isUndefined(b)) {
    return fminf(a, b);
  }

  return yoga::isUndefined(a) ? b : a;
}

bool FBYGValueEqual(const FBYGValue& a, const FBYGValue& b) {
  if (a.unit != b.unit) {
    return false;
  }

  if (a.unit == FBYGUnitUndefined ||
      (yoga::isUndefined(a.value) && yoga::isUndefined(b.value))) {
    return true;
  }

  return fabs(a.value - b.value) < 0.0001f;
}

bool FBYGFloatsEqual(const float a, const float b) {
  if (!yoga::isUndefined(a) && !yoga::isUndefined(b)) {
    return fabs(a - b) < 0.0001f;
  }
  return yoga::isUndefined(a) && yoga::isUndefined(b);
}

bool FBYGDoubleEqual(const double a, const double b) {
  if (!yoga::isUndefined(a) && !yoga::isUndefined(b)) {
    return fabs(a - b) < 0.0001;
  }
  return yoga::isUndefined(a) && yoga::isUndefined(b);
}

float FBYGFloatSanitize(const float val) {
  return yoga::isUndefined(val) ? 0 : val;
}

FBYGFloatOptional FBYGFloatOptionalMax(FBYGFloatOptional op1, FBYGFloatOptional op2) {
  if (op1 >= op2) {
    return op1;
  }
  if (op2 > op1) {
    return op2;
  }
  return op1.isUndefined() ? op2 : op1;
}

void yoga::throwLogicalErrorWithMessage(const char* message) {
#if defined(__cpp_exceptions)
  throw std::logic_error(message);
#else // !defined(__cpp_exceptions)
  std::terminate();
#endif // defined(__cpp_exceptions)
}
