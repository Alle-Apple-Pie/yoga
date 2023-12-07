/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>
#include <stdio.h>
#include "CompactValue.h"
#include "YGConfig.h"
#include "YGLayout.h"
#include "YGStyle.h"
#include "Yoga-internal.h"

FBYGConfigRef FBYGConfigGetDefault();

#pragma pack(push)
#pragma pack(1)
struct FBYGNodeFlags {
  bool hasNewLayout : 1;
  bool isReferenceBaseline : 1;
  bool isDirty : 1;
  uint8_t nodeType : 1;
  bool measureUsesContext : 1;
  bool baselineUsesContext : 1;
  bool printUsesContext : 1;
};
#pragma pack(pop)

struct YOGA_EXPORT FBYGNode {
  using MeasureWithContextFn =
      FBYGSize (*)(FBYGNode*, float, FBYGMeasureMode, float, FBYGMeasureMode, void*);
  using BaselineWithContextFn = float (*)(FBYGNode*, float, float, void*);
  using PrintWithContextFn = void (*)(FBYGNode*, void*);

private:
  void* context_ = nullptr;
  FBYGNodeFlags flags_ = {};
  union {
    FBYGMeasureFunc noContext;
    MeasureWithContextFn withContext;
  } measure_ = {nullptr};
  union {
    FBYGBaselineFunc noContext;
    BaselineWithContextFn withContext;
  } baseline_ = {nullptr};
  union {
    FBYGPrintFunc noContext;
    PrintWithContextFn withContext;
  } print_ = {nullptr};
  FBYGDirtiedFunc dirtied_ = nullptr;
  FBYGStyle style_ = {};
  FBYGLayout layout_ = {};
  uint32_t lineIndex_ = 0;
  FBYGNodeRef owner_ = nullptr;
  FBYGVector children_ = {};
  FBYGConfigRef config_;
  std::array<FBYGValue, 2> resolvedDimensions_ = {
      {FBYGValueUndefined, FBYGValueUndefined}};

  FBYGFloatOptional relativePosition(
      const FBYGFlexDirection axis,
      const float axisSize) const;

  void setMeasureFunc(decltype(measure_));
  void setBaselineFunc(decltype(baseline_));

  void useWebDefaults() {
    style_.flexDirection() = FBYGFlexDirectionRow;
    style_.alignContent() = FBYGAlignStretch;
  }

  // DANGER DANGER DANGER!
  // If the node assigned to has children, we'd either have to deallocate
  // them (potentially incorrect) or ignore them (danger of leaks). Only ever
  // use this after checking that there are no children.
  // DO NOT CHANGE THE VISIBILITY OF THIS METHOD!
  FBYGNode& operator=(FBYGNode&&) = default;

  using CompactValue = facebook::yoga::detail::CompactValue;

public:
  FBYGNode() : FBYGNode{FBYGConfigGetDefault()} { flags_.hasNewLayout = true; }
  explicit FBYGNode(const FBYGConfigRef config);
  ~FBYGNode() = default; // cleanup of owner/children relationships in FBYGNodeFree

  FBYGNode(FBYGNode&&);

  // Does not expose true value semantics, as children are not cloned eagerly.
  // Should we remove this?
  FBYGNode(const FBYGNode& node) = default;

  // assignment means potential leaks of existing children, or alternatively
  // freeing unowned memory, double free, or freeing stack memory.
  FBYGNode& operator=(const FBYGNode&) = delete;

  // Getters
  void* getContext() const { return context_; }

  void print(void*);

  bool getHasNewLayout() const { return flags_.hasNewLayout; }

  FBYGNodeType getNodeType() const {
    return static_cast<FBYGNodeType>(flags_.nodeType);
  }

  bool hasMeasureFunc() const noexcept { return measure_.noContext != nullptr; }

  FBYGSize measure(float, FBYGMeasureMode, float, FBYGMeasureMode, void*);

  bool hasBaselineFunc() const noexcept {
    return baseline_.noContext != nullptr;
  }

  float baseline(float width, float height, void* layoutContext);

  bool hasErrata(FBYGErrata errata) const { return config_->hasErrata(errata); }

  FBYGDirtiedFunc getDirtied() const { return dirtied_; }

  // For Performance reasons passing as reference.
  FBYGStyle& getStyle() { return style_; }

  const FBYGStyle& getStyle() const { return style_; }

  // For Performance reasons passing as reference.
  FBYGLayout& getLayout() { return layout_; }

  const FBYGLayout& getLayout() const { return layout_; }

  uint32_t getLineIndex() const { return lineIndex_; }

  bool isReferenceBaseline() { return flags_.isReferenceBaseline; }

  // returns the FBYGNodeRef that owns this FBYGNode. An owner is used to identify
  // the YogaTree that a FBYGNode belongs to. This method will return the parent
  // of the FBYGNode when a FBYGNode only belongs to one YogaTree or nullptr when
  // the FBYGNode is shared between two or more YogaTrees.
  FBYGNodeRef getOwner() const { return owner_; }

  // Deprecated, use getOwner() instead.
  FBYGNodeRef getParent() const { return getOwner(); }

  const FBYGVector& getChildren() const { return children_; }

  // Applies a callback to all children, after cloning them if they are not
  // owned.
  template <typename T>
  void iterChildrenAfterCloningIfNeeded(T callback, void* cloneContext) {
    int i = 0;
    for (FBYGNodeRef& child : children_) {
      if (child->getOwner() != this) {
        child = config_->cloneNode(child, this, i, cloneContext);
        child->setOwner(this);
      }
      i += 1;

      callback(child, cloneContext);
    }
  }

  FBYGNodeRef getChild(uint32_t index) const { return children_.at(index); }

  FBYGConfigRef getConfig() const { return config_; }

  bool isDirty() const { return flags_.isDirty; }

  std::array<FBYGValue, 2> getResolvedDimensions() const {
    return resolvedDimensions_;
  }

  FBYGValue getResolvedDimension(int index) const {
    return resolvedDimensions_[index];
  }

  static CompactValue computeEdgeValueForColumn(
      const FBYGStyle::Edges& edges,
      FBYGEdge edge,
      CompactValue defaultValue);

  static CompactValue computeEdgeValueForRow(
      const FBYGStyle::Edges& edges,
      FBYGEdge rowEdge,
      FBYGEdge edge,
      CompactValue defaultValue);

  static CompactValue computeRowGap(
      const FBYGStyle::Gutters& gutters,
      CompactValue defaultValue);

  static CompactValue computeColumnGap(
      const FBYGStyle::Gutters& gutters,
      CompactValue defaultValue);

  // Methods related to positions, margin, padding and border
  FBYGFloatOptional getLeadingPosition(
      const FBYGFlexDirection axis,
      const float axisSize) const;
  bool isLeadingPositionDefined(const FBYGFlexDirection axis) const;
  bool isTrailingPosDefined(const FBYGFlexDirection axis) const;
  FBYGFloatOptional getTrailingPosition(
      const FBYGFlexDirection axis,
      const float axisSize) const;
  FBYGFloatOptional getLeadingMargin(
      const FBYGFlexDirection axis,
      const float widthSize) const;
  FBYGFloatOptional getTrailingMargin(
      const FBYGFlexDirection axis,
      const float widthSize) const;
  float getLeadingBorder(const FBYGFlexDirection flexDirection) const;
  float getTrailingBorder(const FBYGFlexDirection flexDirection) const;
  FBYGFloatOptional getLeadingPadding(
      const FBYGFlexDirection axis,
      const float widthSize) const;
  FBYGFloatOptional getTrailingPadding(
      const FBYGFlexDirection axis,
      const float widthSize) const;
  FBYGFloatOptional getLeadingPaddingAndBorder(
      const FBYGFlexDirection axis,
      const float widthSize) const;
  FBYGFloatOptional getTrailingPaddingAndBorder(
      const FBYGFlexDirection axis,
      const float widthSize) const;
  FBYGFloatOptional getMarginForAxis(
      const FBYGFlexDirection axis,
      const float widthSize) const;
  FBYGFloatOptional getGapForAxis(
      const FBYGFlexDirection axis,
      const float widthSize) const;
  // Setters

  void setContext(void* context) { context_ = context; }

  void setPrintFunc(FBYGPrintFunc printFunc) {
    print_.noContext = printFunc;
    flags_.printUsesContext = false;
  }
  void setPrintFunc(PrintWithContextFn printFunc) {
    print_.withContext = printFunc;
    flags_.printUsesContext = true;
  }
  void setPrintFunc(std::nullptr_t) { setPrintFunc(FBYGPrintFunc{nullptr}); }

  void setHasNewLayout(bool hasNewLayout) {
    flags_.hasNewLayout = hasNewLayout;
  }

  void setNodeType(FBYGNodeType nodeType) {
    flags_.nodeType = static_cast<uint8_t>(nodeType);
  }

  void setMeasureFunc(FBYGMeasureFunc measureFunc);
  void setMeasureFunc(MeasureWithContextFn);
  void setMeasureFunc(std::nullptr_t) {
    return setMeasureFunc(FBYGMeasureFunc{nullptr});
  }

  void setBaselineFunc(FBYGBaselineFunc baseLineFunc) {
    flags_.baselineUsesContext = false;
    baseline_.noContext = baseLineFunc;
  }
  void setBaselineFunc(BaselineWithContextFn baseLineFunc) {
    flags_.baselineUsesContext = true;
    baseline_.withContext = baseLineFunc;
  }
  void setBaselineFunc(std::nullptr_t) {
    return setBaselineFunc(FBYGBaselineFunc{nullptr});
  }

  void setDirtiedFunc(FBYGDirtiedFunc dirtiedFunc) { dirtied_ = dirtiedFunc; }

  void setStyle(const FBYGStyle& style) { style_ = style; }

  void setLayout(const FBYGLayout& layout) { layout_ = layout; }

  void setLineIndex(uint32_t lineIndex) { lineIndex_ = lineIndex; }

  void setIsReferenceBaseline(bool isReferenceBaseline) {
    flags_.isReferenceBaseline = isReferenceBaseline;
  }

  void setOwner(FBYGNodeRef owner) { owner_ = owner; }

  void setChildren(const FBYGVector& children) { children_ = children; }

  // TODO: rvalue override for setChildren

  void setConfig(FBYGConfigRef config);

  void setDirty(bool isDirty);
  void setLayoutLastOwnerDirection(FBYGDirection direction);
  void setLayoutComputedFlexBasis(const FBYGFloatOptional computedFlexBasis);
  void setLayoutComputedFlexBasisGeneration(
      uint32_t computedFlexBasisGeneration);
  void setLayoutMeasuredDimension(float measuredDimension, int index);
  void setLayoutHadOverflow(bool hadOverflow);
  void setLayoutDimension(float dimension, int index);
  void setLayoutDirection(FBYGDirection direction);
  void setLayoutMargin(float margin, int index);
  void setLayoutBorder(float border, int index);
  void setLayoutPadding(float padding, int index);
  void setLayoutPosition(float position, int index);
  void setPosition(
      const FBYGDirection direction,
      const float mainSize,
      const float crossSize,
      const float ownerWidth);
  void markDirtyAndPropagateDownwards();

  // Other methods
  FBYGValue marginLeadingValue(const FBYGFlexDirection axis) const;
  FBYGValue marginTrailingValue(const FBYGFlexDirection axis) const;
  FBYGValue resolveFlexBasisPtr() const;
  void resolveDimension();
  FBYGDirection resolveDirection(const FBYGDirection ownerDirection);
  void clearChildren();
  /// Replaces the occurrences of oldChild with newChild
  void replaceChild(FBYGNodeRef oldChild, FBYGNodeRef newChild);
  void replaceChild(FBYGNodeRef child, uint32_t index);
  void insertChild(FBYGNodeRef child, uint32_t index);
  /// Removes the first occurrence of child
  bool removeChild(FBYGNodeRef child);
  void removeChild(uint32_t index);

  void cloneChildrenIfNeeded(void*);
  void markDirtyAndPropagate();
  float resolveFlexGrow() const;
  float resolveFlexShrink() const;
  bool isNodeFlexible();
  void reset();
};
