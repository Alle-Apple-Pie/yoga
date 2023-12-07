/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <type_traits>

#include <yoga/Yoga.h>

#include "CompactValue.h"
#include "YGFloatOptional.h"
#include "Yoga-internal.h"
#include "BitUtils.h"

class YOGA_EXPORT FBYGStyle {
  template <typename Enum>
  using Values =
      facebook::yoga::detail::Values<facebook::yoga::enums::count<Enum>()>;
  using CompactValue = facebook::yoga::detail::CompactValue;

public:
  using Dimensions = Values<FBYGDimension>;
  using Edges = Values<FBYGEdge>;
  using Gutters = Values<FBYGGutter>;

  template <typename T>
  struct BitfieldRef {
    FBYGStyle& style;
    size_t offset;
    operator T() const {
      return facebook::yoga::detail::getEnumData<T>(style.flags, offset);
    }
    BitfieldRef<T>& operator=(T x) {
      facebook::yoga::detail::setEnumData<T>(style.flags, offset, x);
      return *this;
    }
  };

  template <typename T, T FBYGStyle::*Prop>
  struct Ref {
    FBYGStyle& style;
    operator T() const { return style.*Prop; }
    Ref<T, Prop>& operator=(T value) {
      style.*Prop = value;
      return *this;
    }
  };

  template <typename Idx, Values<Idx> FBYGStyle::*Prop>
  struct IdxRef {
    struct Ref {
      FBYGStyle& style;
      Idx idx;
      operator CompactValue() const { return (style.*Prop)[idx]; }
      operator FBYGValue() const { return (style.*Prop)[idx]; }
      Ref& operator=(CompactValue value) {
        (style.*Prop)[idx] = value;
        return *this;
      }
    };

    FBYGStyle& style;
    IdxRef<Idx, Prop>& operator=(const Values<Idx>& values) {
      style.*Prop = values;
      return *this;
    }
    operator const Values<Idx>&() const { return style.*Prop; }
    Ref operator[](Idx idx) { return {style, idx}; }
    CompactValue operator[](Idx idx) const { return (style.*Prop)[idx]; }
  };

  FBYGStyle() {
    alignContent() = FBYGAlignFlexStart;
    alignItems() = FBYGAlignStretch;
  }
  ~FBYGStyle() = default;

private:
  static constexpr size_t directionOffset = 0;
  static constexpr size_t flexdirectionOffset =
      directionOffset + facebook::yoga::detail::bitWidthFn<FBYGDirection>();
  static constexpr size_t justifyContentOffset = flexdirectionOffset +
      facebook::yoga::detail::bitWidthFn<FBYGFlexDirection>();
  static constexpr size_t alignContentOffset =
      justifyContentOffset + facebook::yoga::detail::bitWidthFn<FBYGJustify>();
  static constexpr size_t alignItemsOffset =
      alignContentOffset + facebook::yoga::detail::bitWidthFn<FBYGAlign>();
  static constexpr size_t alignSelfOffset =
      alignItemsOffset + facebook::yoga::detail::bitWidthFn<FBYGAlign>();
  static constexpr size_t positionTypeOffset =
      alignSelfOffset + facebook::yoga::detail::bitWidthFn<FBYGAlign>();
  static constexpr size_t flexWrapOffset =
      positionTypeOffset + facebook::yoga::detail::bitWidthFn<FBYGPositionType>();
  static constexpr size_t overflowOffset =
      flexWrapOffset + facebook::yoga::detail::bitWidthFn<FBYGWrap>();
  static constexpr size_t displayOffset =
      overflowOffset + facebook::yoga::detail::bitWidthFn<FBYGOverflow>();

  uint32_t flags = 0;

  FBYGFloatOptional flex_ = {};
  FBYGFloatOptional flexGrow_ = {};
  FBYGFloatOptional flexShrink_ = {};
  CompactValue flexBasis_ = CompactValue::ofAuto();
  Edges margin_ = {};
  Edges position_ = {};
  Edges padding_ = {};
  Edges border_ = {};
  Gutters gap_ = {};
  Dimensions dimensions_{CompactValue::ofAuto()};
  Dimensions minDimensions_ = {};
  Dimensions maxDimensions_ = {};
  // Yoga specific properties, not compatible with flexbox specification
  FBYGFloatOptional aspectRatio_ = {};

public:
  // for library users needing a type
  using ValueRepr = std::remove_reference<decltype(margin_[0])>::type;

  FBYGDirection direction() const {
    return facebook::yoga::detail::getEnumData<FBYGDirection>(
        flags, directionOffset);
  }
  BitfieldRef<FBYGDirection> direction() { return {*this, directionOffset}; }

  FBYGFlexDirection flexDirection() const {
    return facebook::yoga::detail::getEnumData<FBYGFlexDirection>(
        flags, flexdirectionOffset);
  }
  BitfieldRef<FBYGFlexDirection> flexDirection() {
    return {*this, flexdirectionOffset};
  }

  FBYGJustify justifyContent() const {
    return facebook::yoga::detail::getEnumData<FBYGJustify>(
        flags, justifyContentOffset);
  }
  BitfieldRef<FBYGJustify> justifyContent() {
    return {*this, justifyContentOffset};
  }

  FBYGAlign alignContent() const {
    return facebook::yoga::detail::getEnumData<FBYGAlign>(
        flags, alignContentOffset);
  }
  BitfieldRef<FBYGAlign> alignContent() { return {*this, alignContentOffset}; }

  FBYGAlign alignItems() const {
    return facebook::yoga::detail::getEnumData<FBYGAlign>(
        flags, alignItemsOffset);
  }
  BitfieldRef<FBYGAlign> alignItems() { return {*this, alignItemsOffset}; }

  FBYGAlign alignSelf() const {
    return facebook::yoga::detail::getEnumData<FBYGAlign>(flags, alignSelfOffset);
  }
  BitfieldRef<FBYGAlign> alignSelf() { return {*this, alignSelfOffset}; }

  FBYGPositionType positionType() const {
    return facebook::yoga::detail::getEnumData<FBYGPositionType>(
        flags, positionTypeOffset);
  }
  BitfieldRef<FBYGPositionType> positionType() {
    return {*this, positionTypeOffset};
  }

  FBYGWrap flexWrap() const {
    return facebook::yoga::detail::getEnumData<FBYGWrap>(flags, flexWrapOffset);
  }
  BitfieldRef<FBYGWrap> flexWrap() { return {*this, flexWrapOffset}; }

  FBYGOverflow overflow() const {
    return facebook::yoga::detail::getEnumData<FBYGOverflow>(
        flags, overflowOffset);
  }
  BitfieldRef<FBYGOverflow> overflow() { return {*this, overflowOffset}; }

  FBYGDisplay display() const {
    return facebook::yoga::detail::getEnumData<FBYGDisplay>(flags, displayOffset);
  }
  BitfieldRef<FBYGDisplay> display() { return {*this, displayOffset}; }

  FBYGFloatOptional flex() const { return flex_; }
  Ref<FBYGFloatOptional, &FBYGStyle::flex_> flex() { return {*this}; }

  FBYGFloatOptional flexGrow() const { return flexGrow_; }
  Ref<FBYGFloatOptional, &FBYGStyle::flexGrow_> flexGrow() { return {*this}; }

  FBYGFloatOptional flexShrink() const { return flexShrink_; }
  Ref<FBYGFloatOptional, &FBYGStyle::flexShrink_> flexShrink() { return {*this}; }

  CompactValue flexBasis() const { return flexBasis_; }
  Ref<CompactValue, &FBYGStyle::flexBasis_> flexBasis() { return {*this}; }

  const Edges& margin() const { return margin_; }
  IdxRef<FBYGEdge, &FBYGStyle::margin_> margin() { return {*this}; }

  const Edges& position() const { return position_; }
  IdxRef<FBYGEdge, &FBYGStyle::position_> position() { return {*this}; }

  const Edges& padding() const { return padding_; }
  IdxRef<FBYGEdge, &FBYGStyle::padding_> padding() { return {*this}; }

  const Edges& border() const { return border_; }
  IdxRef<FBYGEdge, &FBYGStyle::border_> border() { return {*this}; }

  const Gutters& gap() const { return gap_; }
  IdxRef<FBYGGutter, &FBYGStyle::gap_> gap() { return {*this}; }

  const Dimensions& dimensions() const { return dimensions_; }
  IdxRef<FBYGDimension, &FBYGStyle::dimensions_> dimensions() { return {*this}; }

  const Dimensions& minDimensions() const { return minDimensions_; }
  IdxRef<FBYGDimension, &FBYGStyle::minDimensions_> minDimensions() {
    return {*this};
  }

  const Dimensions& maxDimensions() const { return maxDimensions_; }
  IdxRef<FBYGDimension, &FBYGStyle::maxDimensions_> maxDimensions() {
    return {*this};
  }

  // Yoga specific properties, not compatible with flexbox specification
  FBYGFloatOptional aspectRatio() const { return aspectRatio_; }
  Ref<FBYGFloatOptional, &FBYGStyle::aspectRatio_> aspectRatio() { return {*this}; }
};

YOGA_EXPORT bool operator==(const FBYGStyle& lhs, const FBYGStyle& rhs);
YOGA_EXPORT inline bool operator!=(const FBYGStyle& lhs, const FBYGStyle& rhs) {
  return !(lhs == rhs);
}
