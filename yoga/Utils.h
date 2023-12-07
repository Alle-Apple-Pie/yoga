/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "YGNode.h"
#include "Yoga-internal.h"
#include "CompactValue.h"

// This struct is an helper model to hold the data for step 4 of flexbox algo,
// which is collecting the flex items in a line.
//
// - itemsOnLine: Number of items which can fit in a line considering the
//   available Inner dimension, the flex items computed flexbasis and their
//   margin. It may be different than the difference between start and end
//   indicates because we skip over absolute-positioned items.
//
// - sizeConsumedOnCurrentLine: It is accumulation of the dimensions and margin
//   of all the children on the current line. This will be used in order to
//   either set the dimensions of the node if none already exist or to compute
//   the remaining space left for the flexible children.
//
// - totalFlexGrowFactors: total flex grow factors of flex items which are to be
//   laid in the current line
//
// - totalFlexShrinkFactors: total flex shrink factors of flex items which are
//   to be laid in the current line
//
// - endOfLineIndex: Its the end index of the last flex item which was examined
//   and it may or may not be part of the current line(as it may be absolutely
//   positioned or including it may have caused to overshoot availableInnerDim)
//
// - relativeChildren: Maintain a vector of the child nodes that can shrink
//   and/or grow.

struct FBYGCollectFlexItemsRowValues {
  uint32_t itemsOnLine;
  float sizeConsumedOnCurrentLine;
  float totalFlexGrowFactors;
  float totalFlexShrinkScaledFactors;
  uint32_t endOfLineIndex;
  std::vector<FBYGNodeRef> relativeChildren;
  float remainingFreeSpace;
  // The size of the mainDim for the row after considering size, padding, margin
  // and border of flex items. This is used to calculate maxLineDim after going
  // through all the rows to decide on the main axis size of owner.
  float mainDim;
  // The size of the crossDim for the row after considering size, padding,
  // margin and border of flex items. Used for calculating containers crossSize.
  float crossDim;
};

bool FBYGValueEqual(const FBYGValue& a, const FBYGValue& b);
inline bool FBYGValueEqual(
    facebook::yoga::detail::CompactValue a,
    facebook::yoga::detail::CompactValue b) {
  return FBYGValueEqual((FBYGValue) a, (FBYGValue) b);
}

// This custom float equality function returns true if either absolute
// difference between two floats is less than 0.0001f or both are undefined.
bool FBYGFloatsEqual(const float a, const float b);

bool FBYGDoubleEqual(const double a, const double b);

float FBYGFloatMax(const float a, const float b);

FBYGFloatOptional FBYGFloatOptionalMax(
    const FBYGFloatOptional op1,
    const FBYGFloatOptional op2);

float FBYGFloatMin(const float a, const float b);

// This custom float comparison function compares the array of float with
// FBYGFloatsEqual, as the default float comparison operator will not work(Look
// at the comments of FBYGFloatsEqual function).
template <std::size_t size>
bool FBYGFloatArrayEqual(
    const std::array<float, size>& val1,
    const std::array<float, size>& val2) {
  bool areEqual = true;
  for (std::size_t i = 0; i < size && areEqual; ++i) {
    areEqual = FBYGFloatsEqual(val1[i], val2[i]);
  }
  return areEqual;
}

// This function returns 0 if FBYGFloatIsUndefined(val) is true and val otherwise
float FBYGFloatSanitize(const float val);

FBYGFlexDirection FBYGFlexDirectionCross(
    const FBYGFlexDirection flexDirection,
    const FBYGDirection direction);

inline bool FBYGFlexDirectionIsRow(const FBYGFlexDirection flexDirection) {
  return flexDirection == FBYGFlexDirectionRow ||
      flexDirection == FBYGFlexDirectionRowReverse;
}

inline FBYGFloatOptional FBYGResolveValue(
    const FBYGValue value,
    const float ownerSize) {
  switch (value.unit) {
    case FBYGUnitPoint:
      return FBYGFloatOptional{value.value};
    case FBYGUnitPercent:
      return FBYGFloatOptional{value.value * ownerSize * 0.01f};
    default:
      return FBYGFloatOptional{};
  }
}

inline FBYGFloatOptional FBYGResolveValue(
    facebook::yoga::detail::CompactValue value,
    float ownerSize) {
  return FBYGResolveValue((FBYGValue) value, ownerSize);
}

inline bool FBYGFlexDirectionIsColumn(const FBYGFlexDirection flexDirection) {
  return flexDirection == FBYGFlexDirectionColumn ||
      flexDirection == FBYGFlexDirectionColumnReverse;
}

inline FBYGFlexDirection FBYGResolveFlexDirection(
    const FBYGFlexDirection flexDirection,
    const FBYGDirection direction) {
  if (direction == FBYGDirectionRTL) {
    if (flexDirection == FBYGFlexDirectionRow) {
      return FBYGFlexDirectionRowReverse;
    } else if (flexDirection == FBYGFlexDirectionRowReverse) {
      return FBYGFlexDirectionRow;
    }
  }

  return flexDirection;
}

inline FBYGFloatOptional FBYGResolveValueMargin(
    facebook::yoga::detail::CompactValue value,
    const float ownerSize) {
  return value.isAuto() ? FBYGFloatOptional{0} : FBYGResolveValue(value, ownerSize);
}
