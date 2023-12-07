/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "YGNode.h"
#include <algorithm>
#include <iostream>
#include "Utils.h"

using namespace facebook;
using facebook::yoga::detail::CompactValue;

FBYGNode::FBYGNode(const FBYGConfigRef config) : config_{config} {
  FBYGAssert(
      config != nullptr, "Attempting to construct FBYGNode with null config");

  flags_.hasNewLayout = true;
  if (config->useWebDefaults()) {
    useWebDefaults();
  }
}

FBYGNode::FBYGNode(FBYGNode&& node) {
  context_ = node.context_;
  flags_ = node.flags_;
  measure_ = node.measure_;
  baseline_ = node.baseline_;
  print_ = node.print_;
  dirtied_ = node.dirtied_;
  style_ = node.style_;
  layout_ = node.layout_;
  lineIndex_ = node.lineIndex_;
  owner_ = node.owner_;
  children_ = std::move(node.children_);
  config_ = node.config_;
  resolvedDimensions_ = node.resolvedDimensions_;
  for (auto c : children_) {
    c->setOwner(this);
  }
}

void FBYGNode::print(void* printContext) {
  if (print_.noContext != nullptr) {
    if (flags_.printUsesContext) {
      print_.withContext(this, printContext);
    } else {
      print_.noContext(this);
    }
  }
}

CompactValue FBYGNode::computeEdgeValueForRow(
    const FBYGStyle::Edges& edges,
    FBYGEdge rowEdge,
    FBYGEdge edge,
    CompactValue defaultValue) {
  if (!edges[rowEdge].isUndefined()) {
    return edges[rowEdge];
  } else if (!edges[edge].isUndefined()) {
    return edges[edge];
  } else if (!edges[FBYGEdgeHorizontal].isUndefined()) {
    return edges[FBYGEdgeHorizontal];
  } else if (!edges[FBYGEdgeAll].isUndefined()) {
    return edges[FBYGEdgeAll];
  } else {
    return defaultValue;
  }
}

CompactValue FBYGNode::computeEdgeValueForColumn(
    const FBYGStyle::Edges& edges,
    FBYGEdge edge,
    CompactValue defaultValue) {
  if (!edges[edge].isUndefined()) {
    return edges[edge];
  } else if (!edges[FBYGEdgeVertical].isUndefined()) {
    return edges[FBYGEdgeVertical];
  } else if (!edges[FBYGEdgeAll].isUndefined()) {
    return edges[FBYGEdgeAll];
  } else {
    return defaultValue;
  }
}

CompactValue FBYGNode::computeRowGap(
    const FBYGStyle::Gutters& gutters,
    CompactValue defaultValue) {
  if (!gutters[FBYGGutterRow].isUndefined()) {
    return gutters[FBYGGutterRow];
  } else if (!gutters[FBYGGutterAll].isUndefined()) {
    return gutters[FBYGGutterAll];
  } else {
    return defaultValue;
  }
}

CompactValue FBYGNode::computeColumnGap(
    const FBYGStyle::Gutters& gutters,
    CompactValue defaultValue) {
  if (!gutters[FBYGGutterColumn].isUndefined()) {
    return gutters[FBYGGutterColumn];
  } else if (!gutters[FBYGGutterAll].isUndefined()) {
    return gutters[FBYGGutterAll];
  } else {
    return defaultValue;
  }
}

FBYGFloatOptional FBYGNode::getLeadingPosition(
    const FBYGFlexDirection axis,
    const float axisSize) const {
  auto leadingPosition = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.position(),
            FBYGEdgeStart,
            leading[axis],
            CompactValue::ofZero())
      : computeEdgeValueForColumn(
            style_.position(), leading[axis], CompactValue::ofZero());
  return FBYGResolveValue(leadingPosition, axisSize);
}

FBYGFloatOptional FBYGNode::getTrailingPosition(
    const FBYGFlexDirection axis,
    const float axisSize) const {
  auto trailingPosition = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.position(),
            FBYGEdgeEnd,
            trailing[axis],
            CompactValue::ofZero())
      : computeEdgeValueForColumn(
            style_.position(), trailing[axis], CompactValue::ofZero());
  return FBYGResolveValue(trailingPosition, axisSize);
}

bool FBYGNode::isLeadingPositionDefined(const FBYGFlexDirection axis) const {
  auto leadingPosition = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.position(),
            FBYGEdgeStart,
            leading[axis],
            CompactValue::ofUndefined())
      : computeEdgeValueForColumn(
            style_.position(), leading[axis], CompactValue::ofUndefined());
  return !leadingPosition.isUndefined();
}

bool FBYGNode::isTrailingPosDefined(const FBYGFlexDirection axis) const {
  auto trailingPosition = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.position(),
            FBYGEdgeEnd,
            trailing[axis],
            CompactValue::ofUndefined())
      : computeEdgeValueForColumn(
            style_.position(), trailing[axis], CompactValue::ofUndefined());
  return !trailingPosition.isUndefined();
}

FBYGFloatOptional FBYGNode::getLeadingMargin(
    const FBYGFlexDirection axis,
    const float widthSize) const {
  auto leadingMargin = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.margin(), FBYGEdgeStart, leading[axis], CompactValue::ofZero())
      : computeEdgeValueForColumn(
            style_.margin(), leading[axis], CompactValue::ofZero());
  return FBYGResolveValueMargin(leadingMargin, widthSize);
}

FBYGFloatOptional FBYGNode::getTrailingMargin(
    const FBYGFlexDirection axis,
    const float widthSize) const {
  auto trailingMargin = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.margin(), FBYGEdgeEnd, trailing[axis], CompactValue::ofZero())
      : computeEdgeValueForColumn(
            style_.margin(), trailing[axis], CompactValue::ofZero());
  return FBYGResolveValueMargin(trailingMargin, widthSize);
}

FBYGFloatOptional FBYGNode::getMarginForAxis(
    const FBYGFlexDirection axis,
    const float widthSize) const {
  return getLeadingMargin(axis, widthSize) + getTrailingMargin(axis, widthSize);
}

FBYGFloatOptional FBYGNode::getGapForAxis(
    const FBYGFlexDirection axis,
    const float widthSize) const {
  auto gap = FBYGFlexDirectionIsRow(axis)
      ? computeColumnGap(style_.gap(), CompactValue::ofZero())
      : computeRowGap(style_.gap(), CompactValue::ofZero());
  return FBYGResolveValue(gap, widthSize);
}

FBYGSize FBYGNode::measure(
    float width,
    FBYGMeasureMode widthMode,
    float height,
    FBYGMeasureMode heightMode,
    void* layoutContext) {
  return flags_.measureUsesContext
      ? measure_.withContext(
            this, width, widthMode, height, heightMode, layoutContext)
      : measure_.noContext(this, width, widthMode, height, heightMode);
}

float FBYGNode::baseline(float width, float height, void* layoutContext) {
  return flags_.baselineUsesContext
      ? baseline_.withContext(this, width, height, layoutContext)
      : baseline_.noContext(this, width, height);
}

// Setters

void FBYGNode::setMeasureFunc(decltype(FBYGNode::measure_) measureFunc) {
  if (measureFunc.noContext == nullptr) {
    // TODO: t18095186 Move nodeType to opt-in function and mark appropriate
    // places in Litho
    setNodeType(FBYGNodeTypeDefault);
  } else {
    FBYGAssertWithNode(
        this,
        children_.size() == 0,
        "Cannot set measure function: Nodes with measure functions cannot have "
        "children.");
    // TODO: t18095186 Move nodeType to opt-in function and mark appropriate
    // places in Litho
    setNodeType(FBYGNodeTypeText);
  }

  measure_ = measureFunc;
}

void FBYGNode::setMeasureFunc(FBYGMeasureFunc measureFunc) {
  flags_.measureUsesContext = false;
  decltype(FBYGNode::measure_) m;
  m.noContext = measureFunc;
  setMeasureFunc(m);
}

YOGA_EXPORT void FBYGNode::setMeasureFunc(MeasureWithContextFn measureFunc) {
  flags_.measureUsesContext = true;
  decltype(FBYGNode::measure_) m;
  m.withContext = measureFunc;
  setMeasureFunc(m);
}

void FBYGNode::replaceChild(FBYGNodeRef child, uint32_t index) {
  children_[index] = child;
}

void FBYGNode::replaceChild(FBYGNodeRef oldChild, FBYGNodeRef newChild) {
  std::replace(children_.begin(), children_.end(), oldChild, newChild);
}

void FBYGNode::insertChild(FBYGNodeRef child, uint32_t index) {
  children_.insert(children_.begin() + index, child);
}

void FBYGNode::setConfig(FBYGConfigRef config) {
  FBYGAssert(config != nullptr, "Attempting to set a null config on a FBYGNode");
  FBYGAssertWithConfig(
      config,
      config->useWebDefaults() == config_->useWebDefaults(),
      "UseWebDefaults may not be changed after constructing a FBYGNode");

  if (yoga::configUpdateInvalidatesLayout(config_, config)) {
    markDirtyAndPropagate();
  }

  config_ = config;
}

void FBYGNode::setDirty(bool isDirty) {
  if (isDirty == flags_.isDirty) {
    return;
  }
  flags_.isDirty = isDirty;
  if (isDirty && dirtied_) {
    dirtied_(this);
  }
}

bool FBYGNode::removeChild(FBYGNodeRef child) {
  std::vector<FBYGNodeRef>::iterator p =
      std::find(children_.begin(), children_.end(), child);
  if (p != children_.end()) {
    children_.erase(p);
    return true;
  }
  return false;
}

void FBYGNode::removeChild(uint32_t index) {
  children_.erase(children_.begin() + index);
}

void FBYGNode::setLayoutDirection(FBYGDirection direction) {
  layout_.setDirection(direction);
}

void FBYGNode::setLayoutMargin(float margin, int index) {
  layout_.margin[index] = margin;
}

void FBYGNode::setLayoutBorder(float border, int index) {
  layout_.border[index] = border;
}

void FBYGNode::setLayoutPadding(float padding, int index) {
  layout_.padding[index] = padding;
}

void FBYGNode::setLayoutLastOwnerDirection(FBYGDirection direction) {
  layout_.lastOwnerDirection = direction;
}

void FBYGNode::setLayoutComputedFlexBasis(
    const FBYGFloatOptional computedFlexBasis) {
  layout_.computedFlexBasis = computedFlexBasis;
}

void FBYGNode::setLayoutPosition(float position, int index) {
  layout_.position[index] = position;
}

void FBYGNode::setLayoutComputedFlexBasisGeneration(
    uint32_t computedFlexBasisGeneration) {
  layout_.computedFlexBasisGeneration = computedFlexBasisGeneration;
}

void FBYGNode::setLayoutMeasuredDimension(float measuredDimension, int index) {
  layout_.measuredDimensions[index] = measuredDimension;
}

void FBYGNode::setLayoutHadOverflow(bool hadOverflow) {
  layout_.setHadOverflow(hadOverflow);
}

void FBYGNode::setLayoutDimension(float dimension, int index) {
  layout_.dimensions[index] = dimension;
}

// If both left and right are defined, then use left. Otherwise return +left or
// -right depending on which is defined.
FBYGFloatOptional FBYGNode::relativePosition(
    const FBYGFlexDirection axis,
    const float axisSize) const {
  if (isLeadingPositionDefined(axis)) {
    return getLeadingPosition(axis, axisSize);
  }

  FBYGFloatOptional trailingPosition = getTrailingPosition(axis, axisSize);
  if (!trailingPosition.isUndefined()) {
    trailingPosition = FBYGFloatOptional{-1 * trailingPosition.unwrap()};
  }
  return trailingPosition;
}

void FBYGNode::setPosition(
    const FBYGDirection direction,
    const float mainSize,
    const float crossSize,
    const float ownerWidth) {
  /* Root nodes should be always layouted as LTR, so we don't return negative
   * values. */
  const FBYGDirection directionRespectingRoot =
      owner_ != nullptr ? direction : FBYGDirectionLTR;
  const FBYGFlexDirection mainAxis =
      FBYGResolveFlexDirection(style_.flexDirection(), directionRespectingRoot);
  const FBYGFlexDirection crossAxis =
      FBYGFlexDirectionCross(mainAxis, directionRespectingRoot);

  // Here we should check for `FBYGPositionTypeStatic` and in this case zero inset
  // properties (left, right, top, bottom, begin, end).
  // https://www.w3.org/TR/css-position-3/#valdef-position-static
  const FBYGFloatOptional relativePositionMain =
      relativePosition(mainAxis, mainSize);
  const FBYGFloatOptional relativePositionCross =
      relativePosition(crossAxis, crossSize);

  setLayoutPosition(
      (getLeadingMargin(mainAxis, ownerWidth) + relativePositionMain).unwrap(),
      leading[mainAxis]);
  setLayoutPosition(
      (getTrailingMargin(mainAxis, ownerWidth) + relativePositionMain).unwrap(),
      trailing[mainAxis]);
  setLayoutPosition(
      (getLeadingMargin(crossAxis, ownerWidth) + relativePositionCross)
          .unwrap(),
      leading[crossAxis]);
  setLayoutPosition(
      (getTrailingMargin(crossAxis, ownerWidth) + relativePositionCross)
          .unwrap(),
      trailing[crossAxis]);
}

FBYGValue FBYGNode::marginLeadingValue(const FBYGFlexDirection axis) const {
  if (FBYGFlexDirectionIsRow(axis) &&
      !style_.margin()[FBYGEdgeStart].isUndefined()) {
    return style_.margin()[FBYGEdgeStart];
  } else {
    return style_.margin()[leading[axis]];
  }
}

FBYGValue FBYGNode::marginTrailingValue(const FBYGFlexDirection axis) const {
  if (FBYGFlexDirectionIsRow(axis) && !style_.margin()[FBYGEdgeEnd].isUndefined()) {
    return style_.margin()[FBYGEdgeEnd];
  } else {
    return style_.margin()[trailing[axis]];
  }
}

FBYGValue FBYGNode::resolveFlexBasisPtr() const {
  FBYGValue flexBasis = style_.flexBasis();
  if (flexBasis.unit != FBYGUnitAuto && flexBasis.unit != FBYGUnitUndefined) {
    return flexBasis;
  }
  if (!style_.flex().isUndefined() && style_.flex().unwrap() > 0.0f) {
    return config_->useWebDefaults() ? FBYGValueAuto : FBYGValueZero;
  }
  return FBYGValueAuto;
}

void FBYGNode::resolveDimension() {
  using namespace yoga;
  const FBYGStyle& style = getStyle();
  for (auto dim : {FBYGDimensionWidth, FBYGDimensionHeight}) {
    if (!style.maxDimensions()[dim].isUndefined() &&
        FBYGValueEqual(style.maxDimensions()[dim], style.minDimensions()[dim])) {
      resolvedDimensions_[dim] = style.maxDimensions()[dim];
    } else {
      resolvedDimensions_[dim] = style.dimensions()[dim];
    }
  }
}

FBYGDirection FBYGNode::resolveDirection(const FBYGDirection ownerDirection) {
  if (style_.direction() == FBYGDirectionInherit) {
    return ownerDirection > FBYGDirectionInherit ? ownerDirection
                                               : FBYGDirectionLTR;
  } else {
    return style_.direction();
  }
}

YOGA_EXPORT void FBYGNode::clearChildren() {
  children_.clear();
  children_.shrink_to_fit();
}

// Other Methods

void FBYGNode::cloneChildrenIfNeeded(void* cloneContext) {
  iterChildrenAfterCloningIfNeeded([](FBYGNodeRef, void*) {}, cloneContext);
}

void FBYGNode::markDirtyAndPropagate() {
  if (!flags_.isDirty) {
    setDirty(true);
    setLayoutComputedFlexBasis(FBYGFloatOptional());
    if (owner_) {
      owner_->markDirtyAndPropagate();
    }
  }
}

void FBYGNode::markDirtyAndPropagateDownwards() {
  flags_.isDirty = true;
  for_each(children_.begin(), children_.end(), [](FBYGNodeRef childNode) {
    childNode->markDirtyAndPropagateDownwards();
  });
}

float FBYGNode::resolveFlexGrow() const {
  // Root nodes flexGrow should always be 0
  if (owner_ == nullptr) {
    return 0.0;
  }
  if (!style_.flexGrow().isUndefined()) {
    return style_.flexGrow().unwrap();
  }
  if (!style_.flex().isUndefined() && style_.flex().unwrap() > 0.0f) {
    return style_.flex().unwrap();
  }
  return kDefaultFlexGrow;
}

float FBYGNode::resolveFlexShrink() const {
  if (owner_ == nullptr) {
    return 0.0;
  }
  if (!style_.flexShrink().isUndefined()) {
    return style_.flexShrink().unwrap();
  }
  if (!config_->useWebDefaults() && !style_.flex().isUndefined() &&
      style_.flex().unwrap() < 0.0f) {
    return -style_.flex().unwrap();
  }
  return config_->useWebDefaults() ? kWebDefaultFlexShrink : kDefaultFlexShrink;
}

bool FBYGNode::isNodeFlexible() {
  return (
      (style_.positionType() != FBYGPositionTypeAbsolute) &&
      (resolveFlexGrow() != 0 || resolveFlexShrink() != 0));
}

float FBYGNode::getLeadingBorder(const FBYGFlexDirection axis) const {
  FBYGValue leadingBorder = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.border(), FBYGEdgeStart, leading[axis], CompactValue::ofZero())
      : computeEdgeValueForColumn(
            style_.border(), leading[axis], CompactValue::ofZero());
  return fmaxf(leadingBorder.value, 0.0f);
}

float FBYGNode::getTrailingBorder(const FBYGFlexDirection axis) const {
  FBYGValue trailingBorder = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.border(), FBYGEdgeEnd, trailing[axis], CompactValue::ofZero())
      : computeEdgeValueForColumn(
            style_.border(), trailing[axis], CompactValue::ofZero());
  return fmaxf(trailingBorder.value, 0.0f);
}

FBYGFloatOptional FBYGNode::getLeadingPadding(
    const FBYGFlexDirection axis,
    const float widthSize) const {
  auto leadingPadding = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.padding(),
            FBYGEdgeStart,
            leading[axis],
            CompactValue::ofZero())
      : computeEdgeValueForColumn(
            style_.padding(), leading[axis], CompactValue::ofZero());
  return FBYGFloatOptionalMax(
      FBYGResolveValue(leadingPadding, widthSize), FBYGFloatOptional(0.0f));
}

FBYGFloatOptional FBYGNode::getTrailingPadding(
    const FBYGFlexDirection axis,
    const float widthSize) const {
  auto trailingPadding = FBYGFlexDirectionIsRow(axis)
      ? computeEdgeValueForRow(
            style_.padding(), FBYGEdgeEnd, trailing[axis], CompactValue::ofZero())
      : computeEdgeValueForColumn(
            style_.padding(), trailing[axis], CompactValue::ofZero());
  return FBYGFloatOptionalMax(
      FBYGResolveValue(trailingPadding, widthSize), FBYGFloatOptional(0.0f));
}

FBYGFloatOptional FBYGNode::getLeadingPaddingAndBorder(
    const FBYGFlexDirection axis,
    const float widthSize) const {
  return getLeadingPadding(axis, widthSize) +
      FBYGFloatOptional(getLeadingBorder(axis));
}

FBYGFloatOptional FBYGNode::getTrailingPaddingAndBorder(
    const FBYGFlexDirection axis,
    const float widthSize) const {
  return getTrailingPadding(axis, widthSize) +
      FBYGFloatOptional(getTrailingBorder(axis));
}

void FBYGNode::reset() {
  FBYGAssertWithNode(
      this,
      children_.size() == 0,
      "Cannot reset a node which still has children attached");
  FBYGAssertWithNode(
      this, owner_ == nullptr, "Cannot reset a node still attached to a owner");

  *this = FBYGNode{getConfig()};
}
