/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

#include <yoga/Yoga.h>

#include "CompactValue.h"

using FBYGVector = std::vector<FBYGNodeRef>;

FBYG_EXTERN_C_BEGIN

void FBYGNodeCalculateLayoutWithContext(
    FBYGNodeRef node,
    float availableWidth,
    float availableHeight,
    FBYGDirection ownerDirection,
    void* layoutContext);

// Deallocates a Yoga Node. Unlike FBYGNodeFree, does not remove the node from
// its parent or children.
void FBYGNodeDeallocate(FBYGNodeRef node);

FBYG_EXTERN_C_END

namespace facebook {
namespace yoga {

inline bool isUndefined(float value) {
  return std::isnan(value);
}

inline bool isUndefined(double value) {
  return std::isnan(value);
}

void throwLogicalErrorWithMessage(const char* message);

} // namespace yoga
} // namespace facebook

extern const std::array<FBYGEdge, 4> trailing;
extern const std::array<FBYGEdge, 4> leading;
extern const FBYGValue FBYGValueUndefined;
extern const FBYGValue FBYGValueAuto;
extern const FBYGValue FBYGValueZero;

struct FBYGCachedMeasurement {
  float availableWidth;
  float availableHeight;
  FBYGMeasureMode widthMeasureMode;
  FBYGMeasureMode heightMeasureMode;

  float computedWidth;
  float computedHeight;

  FBYGCachedMeasurement()
      : availableWidth(-1),
        availableHeight(-1),
        widthMeasureMode(FBYGMeasureModeUndefined),
        heightMeasureMode(FBYGMeasureModeUndefined),
        computedWidth(-1),
        computedHeight(-1) {}

  bool operator==(FBYGCachedMeasurement measurement) const {
    using namespace facebook;

    bool isEqual = widthMeasureMode == measurement.widthMeasureMode &&
        heightMeasureMode == measurement.heightMeasureMode;

    if (!yoga::isUndefined(availableWidth) ||
        !yoga::isUndefined(measurement.availableWidth)) {
      isEqual = isEqual && availableWidth == measurement.availableWidth;
    }
    if (!yoga::isUndefined(availableHeight) ||
        !yoga::isUndefined(measurement.availableHeight)) {
      isEqual = isEqual && availableHeight == measurement.availableHeight;
    }
    if (!yoga::isUndefined(computedWidth) ||
        !yoga::isUndefined(measurement.computedWidth)) {
      isEqual = isEqual && computedWidth == measurement.computedWidth;
    }
    if (!yoga::isUndefined(computedHeight) ||
        !yoga::isUndefined(measurement.computedHeight)) {
      isEqual = isEqual && computedHeight == measurement.computedHeight;
    }

    return isEqual;
  }
};

// This value was chosen based on empirical data:
// 98% of analyzed layouts require less than 8 entries.
#define FBYG_MAX_CACHED_RESULT_COUNT 8

namespace facebook {
namespace yoga {
namespace detail {

template <size_t Size>
class Values {
private:
  std::array<CompactValue, Size> values_;

public:
  Values() = default;
  Values(const Values& other) = default;

  explicit Values(const FBYGValue& defaultValue) noexcept {
    values_.fill(defaultValue);
  }

  const CompactValue& operator[](size_t i) const noexcept { return values_[i]; }
  CompactValue& operator[](size_t i) noexcept { return values_[i]; }

  template <size_t I>
  FBYGValue get() const noexcept {
    return std::get<I>(values_);
  }

  template <size_t I>
  void set(FBYGValue& value) noexcept {
    std::get<I>(values_) = value;
  }

  template <size_t I>
  void set(FBYGValue&& value) noexcept {
    set<I>(value);
  }

  bool operator==(const Values& other) const noexcept {
    for (size_t i = 0; i < Size; ++i) {
      if (values_[i] != other.values_[i]) {
        return false;
      }
    }
    return true;
  }

  Values& operator=(const Values& other) = default;
};
} // namespace detail
} // namespace yoga
} // namespace facebook

static const float kDefaultFlexGrow = 0.0f;
static const float kDefaultFlexShrink = 0.0f;
static const float kWebDefaultFlexShrink = 1.0f;

extern bool FBYGFloatsEqual(const float a, const float b);
