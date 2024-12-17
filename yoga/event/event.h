/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <yoga/Yoga.h>

#include <array>
#include <cstdint>
#include <functional>
#include <vector>

namespace facebookyg::yoga {

enum struct LayoutType : int {
  kLayout = 0,
  kMeasure = 1,
  kCachedLayout = 2,
  kCachedMeasure = 3
};

enum struct LayoutPassReason : int {
  kInitial = 0,
  kAbsLayout = 1,
  kStretch = 2,
  kMultilineStretch = 3,
  kFlexLayout = 4,
  kMeasureChild = 5,
  kAbsMeasureChild = 6,
  kFlexMeasure = 7,
  COUNT
};

struct LayoutData {
  int layouts;
  int measures;
  uint32_t maxMeasureCache;
  int cachedLayouts;
  int cachedMeasures;
  int measureCallbacks;
  std::array<int, static_cast<uint8_t>(LayoutPassReason::COUNT)>
      measureCallbackReasonsCount;
};

const char* LayoutPassReasonToString(LayoutPassReason value);

struct FBYG_EXPORT Event {
  enum Type {
    NodeAllocation,
    NodeDeallocation,
    NodeLayout,
    LayoutPassStart,
    LayoutPassEnd,
    MeasureCallbackStart,
    MeasureCallbackEnd,
    NodeBaselineStart,
    NodeBaselineEnd,
  };
  class Data;
  using Subscriber = void(FBYGNodeConstRef, Type, Data);
  using Subscribers = std::vector<std::function<Subscriber>>;

  template <Type E>
  struct TypedData {};

  class Data {
    const void* data_;

   public:
    template <Type E>
    explicit Data(const TypedData<E>& data) : data_{&data} {}

    template <Type E>
    const TypedData<E>& get() const {
      return *static_cast<const TypedData<E>*>(data_);
    }
  };

  static void reset();

  static void subscribe(std::function<Subscriber>&& subscriber);

  template <Type E>
  static void publish(
      FBYGNodeConstRef node,
      const TypedData<E>& eventData = {}) {
    publish(node, E, Data{eventData});
  }

 private:
  static void publish(
      FBYGNodeConstRef /*node*/,
      Type /*eventType*/,
      const Data& /*eventData*/);
};

template <>
struct Event::TypedData<Event::NodeAllocation> {
  FBYGConfigConstRef config;
};

template <>
struct Event::TypedData<Event::NodeDeallocation> {
  FBYGConfigConstRef config;
};

template <>
struct Event::TypedData<Event::LayoutPassEnd> {
  LayoutData* layoutData;
};

template <>
struct Event::TypedData<Event::MeasureCallbackEnd> {
  float width;
  FBYGMeasureMode widthMeasureMode;
  float height;
  FBYGMeasureMode heightMeasureMode;
  float measuredWidth;
  float measuredHeight;
  const LayoutPassReason reason;
};

template <>
struct Event::TypedData<Event::NodeLayout> {
  LayoutType layoutType;
};

} // namespace facebookyg::yoga
