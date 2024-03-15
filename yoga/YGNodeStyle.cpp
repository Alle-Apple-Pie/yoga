/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>
#include <yoga/debug/AssertFatal.h>
#include <yoga/node/Node.h>

using namespace facebookyg;
using namespace facebookyg::yoga;

namespace {

template <auto GetterT, auto SetterT, typename ValueT>
void updateStyle(FBYGNodeRef node, ValueT value) {
  auto& style = resolveRef(node)->style();
  if ((style.*GetterT)() != value) {
    (style.*SetterT)(value);
    resolveRef(node)->markDirtyAndPropagate();
  }
}

template <auto GetterT, auto SetterT, typename IdxT, typename ValueT>
void updateStyle(FBYGNodeRef node, IdxT idx, ValueT value) {
  auto& style = resolveRef(node)->style();
  if ((style.*GetterT)(idx) != value) {
    (style.*SetterT)(idx, value);
    resolveRef(node)->markDirtyAndPropagate();
  }
}

} // namespace

void FBYGNodeCopyStyle(FBYGNodeRef dstNode, FBYGNodeConstRef srcNode) {
  auto dst = resolveRef(dstNode);
  auto src = resolveRef(srcNode);

  if (dst->style() != src->style()) {
    dst->setStyle(src->style());
    dst->markDirtyAndPropagate();
  }
}

void FBYGNodeStyleSetDirection(
    const FBYGNodeRef node,
    const FBYGDirection value) {
  updateStyle<&Style::direction, &Style::setDirection>(node, scopedEnum(value));
}

FBYGDirection FBYGNodeStyleGetDirection(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().direction());
}

void FBYGNodeStyleSetFlexDirection(
    const FBYGNodeRef node,
    const FBYGFlexDirection flexDirection) {
  updateStyle<&Style::flexDirection, &Style::setFlexDirection>(
      node, scopedEnum(flexDirection));
}

FBYGFlexDirection FBYGNodeStyleGetFlexDirection(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().flexDirection());
}

void FBYGNodeStyleSetJustifyContent(
    const FBYGNodeRef node,
    const FBYGJustify justifyContent) {
  updateStyle<&Style::justifyContent, &Style::setJustifyContent>(
      node, scopedEnum(justifyContent));
}

FBYGJustify FBYGNodeStyleGetJustifyContent(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().justifyContent());
}

void FBYGNodeStyleSetAlignContent(
    const FBYGNodeRef node,
    const FBYGAlign alignContent) {
  updateStyle<&Style::alignContent, &Style::setAlignContent>(
      node, scopedEnum(alignContent));
}

FBYGAlign FBYGNodeStyleGetAlignContent(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().alignContent());
}

void FBYGNodeStyleSetAlignItems(
    const FBYGNodeRef node,
    const FBYGAlign alignItems) {
  updateStyle<&Style::alignItems, &Style::setAlignItems>(
      node, scopedEnum(alignItems));
}

FBYGAlign FBYGNodeStyleGetAlignItems(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().alignItems());
}

void FBYGNodeStyleSetAlignSelf(
    const FBYGNodeRef node,
    const FBYGAlign alignSelf) {
  updateStyle<&Style::alignSelf, &Style::setAlignSelf>(
      node, scopedEnum(alignSelf));
}

FBYGAlign FBYGNodeStyleGetAlignSelf(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().alignSelf());
}

void FBYGNodeStyleSetPositionType(
    const FBYGNodeRef node,
    const FBYGPositionType positionType) {
  updateStyle<&Style::positionType, &Style::setPositionType>(
      node, scopedEnum(positionType));
}

FBYGPositionType FBYGNodeStyleGetPositionType(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().positionType());
}

void FBYGNodeStyleSetFlexWrap(const FBYGNodeRef node, const FBYGWrap flexWrap) {
  updateStyle<&Style::flexWrap, &Style::setFlexWrap>(
      node, scopedEnum(flexWrap));
}

FBYGWrap FBYGNodeStyleGetFlexWrap(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().flexWrap());
}

void FBYGNodeStyleSetOverflow(
    const FBYGNodeRef node,
    const FBYGOverflow overflow) {
  updateStyle<&Style::overflow, &Style::setOverflow>(
      node, scopedEnum(overflow));
}

FBYGOverflow FBYGNodeStyleGetOverflow(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().overflow());
}

void FBYGNodeStyleSetDisplay(
    const FBYGNodeRef node,
    const FBYGDisplay display) {
  updateStyle<&Style::display, &Style::setDisplay>(node, scopedEnum(display));
}

FBYGDisplay FBYGNodeStyleGetDisplay(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().display());
}

void FBYGNodeStyleSetFlex(const FBYGNodeRef node, const float flex) {
  updateStyle<&Style::flex, &Style::setFlex>(node, FloatOptional{flex});
}

float FBYGNodeStyleGetFlex(const FBYGNodeConstRef nodeRef) {
  const auto node = resolveRef(nodeRef);
  return node->style().flex().isUndefined() ? FBYGUndefined
                                            : node->style().flex().unwrap();
}

void FBYGNodeStyleSetFlexGrow(const FBYGNodeRef node, const float flexGrow) {
  updateStyle<&Style::flexGrow, &Style::setFlexGrow>(
      node, FloatOptional{flexGrow});
}

float FBYGNodeStyleGetFlexGrow(const FBYGNodeConstRef nodeRef) {
  const auto node = resolveRef(nodeRef);
  return node->style().flexGrow().isUndefined()
      ? Style::DefaultFlexGrow
      : node->style().flexGrow().unwrap();
}

void FBYGNodeStyleSetFlexShrink(
    const FBYGNodeRef node,
    const float flexShrink) {
  updateStyle<&Style::flexShrink, &Style::setFlexShrink>(
      node, FloatOptional{flexShrink});
}

float FBYGNodeStyleGetFlexShrink(const FBYGNodeConstRef nodeRef) {
  const auto node = resolveRef(nodeRef);
  return node->style().flexShrink().isUndefined()
      ? (node->getConfig()->useWebDefaults() ? Style::WebDefaultFlexShrink
                                             : Style::DefaultFlexShrink)
      : node->style().flexShrink().unwrap();
}

void FBYGNodeStyleSetFlexBasis(const FBYGNodeRef node, const float flexBasis) {
  updateStyle<&Style::flexBasis, &Style::setFlexBasis>(
      node, value::points(flexBasis));
}

void FBYGNodeStyleSetFlexBasisPercent(
    const FBYGNodeRef node,
    const float flexBasisPercent) {
  updateStyle<&Style::flexBasis, &Style::setFlexBasis>(
      node, value::percent(flexBasisPercent));
}

void FBYGNodeStyleSetFlexBasisAuto(const FBYGNodeRef node) {
  updateStyle<&Style::flexBasis, &Style::setFlexBasis>(node, value::ofAuto());
}

FBYGValue FBYGNodeStyleGetFlexBasis(const FBYGNodeConstRef node) {
  return (FBYGValue)resolveRef(node)->style().flexBasis();
}

void FBYGNodeStyleSetPosition(FBYGNodeRef node, FBYGEdge edge, float points) {
  updateStyle<&Style::position, &Style::setPosition>(
      node, scopedEnum(edge), value::points(points));
}

void FBYGNodeStyleSetPositionPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float percent) {
  updateStyle<&Style::position, &Style::setPosition>(
      node, scopedEnum(edge), value::percent(percent));
}

FBYGValue FBYGNodeStyleGetPosition(FBYGNodeConstRef node, FBYGEdge edge) {
  return (FBYGValue)resolveRef(node)->style().position(scopedEnum(edge));
}

void FBYGNodeStyleSetMargin(FBYGNodeRef node, FBYGEdge edge, float points) {
  updateStyle<&Style::margin, &Style::setMargin>(
      node, scopedEnum(edge), value::points(points));
}

void FBYGNodeStyleSetMarginPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float percent) {
  updateStyle<&Style::margin, &Style::setMargin>(
      node, scopedEnum(edge), value::percent(percent));
}

void FBYGNodeStyleSetMarginAuto(FBYGNodeRef node, FBYGEdge edge) {
  updateStyle<&Style::margin, &Style::setMargin>(
      node, scopedEnum(edge), value::ofAuto());
}

FBYGValue FBYGNodeStyleGetMargin(FBYGNodeConstRef node, FBYGEdge edge) {
  return (FBYGValue)resolveRef(node)->style().margin(scopedEnum(edge));
}

void FBYGNodeStyleSetPadding(FBYGNodeRef node, FBYGEdge edge, float points) {
  updateStyle<&Style::padding, &Style::setPadding>(
      node, scopedEnum(edge), value::points(points));
}

void FBYGNodeStyleSetPaddingPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float percent) {
  updateStyle<&Style::padding, &Style::setPadding>(
      node, scopedEnum(edge), value::percent(percent));
}

FBYGValue FBYGNodeStyleGetPadding(FBYGNodeConstRef node, FBYGEdge edge) {
  return (FBYGValue)resolveRef(node)->style().padding(scopedEnum(edge));
}

void FBYGNodeStyleSetBorder(
    const FBYGNodeRef node,
    const FBYGEdge edge,
    const float border) {
  updateStyle<&Style::border, &Style::setBorder>(
      node, scopedEnum(edge), value::points(border));
}

float FBYGNodeStyleGetBorder(const FBYGNodeConstRef node, const FBYGEdge edge) {
  auto border = resolveRef(node)->style().border(scopedEnum(edge));
  if (border.isUndefined() || border.isAuto()) {
    return FBYGUndefined;
  }

  return static_cast<FBYGValue>(border).value;
}

void FBYGNodeStyleSetGap(
    const FBYGNodeRef node,
    const FBYGGutter gutter,
    const float gapLength) {
  updateStyle<&Style::gap, &Style::setGap>(
      node, scopedEnum(gutter), value::points(gapLength));
}

float FBYGNodeStyleGetGap(
    const FBYGNodeConstRef node,
    const FBYGGutter gutter) {
  auto gapLength = resolveRef(node)->style().gap(scopedEnum(gutter));
  if (gapLength.isUndefined() || gapLength.isAuto()) {
    return FBYGUndefined;
  }

  return static_cast<FBYGValue>(gapLength).value;
}

void FBYGNodeStyleSetAspectRatio(
    const FBYGNodeRef node,
    const float aspectRatio) {
  updateStyle<&Style::aspectRatio, &Style::setAspectRatio>(
      node, FloatOptional{aspectRatio});
}

float FBYGNodeStyleGetAspectRatio(const FBYGNodeConstRef node) {
  const FloatOptional op = resolveRef(node)->style().aspectRatio();
  return op.isUndefined() ? FBYGUndefined : op.unwrap();
}

void FBYGNodeStyleSetWidth(FBYGNodeRef node, float points) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, value::points(points));
}

void FBYGNodeStyleSetWidthPercent(FBYGNodeRef node, float percent) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, value::percent(percent));
}

void FBYGNodeStyleSetWidthAuto(FBYGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, value::ofAuto());
}

FBYGValue FBYGNodeStyleGetWidth(FBYGNodeConstRef node) {
  return (FBYGValue)resolveRef(node)->style().dimension(Dimension::Width);
}

void FBYGNodeStyleSetHeight(FBYGNodeRef node, float points) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, value::points(points));
}

void FBYGNodeStyleSetHeightPercent(FBYGNodeRef node, float percent) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, value::percent(percent));
}

void FBYGNodeStyleSetHeightAuto(FBYGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, value::ofAuto());
}

FBYGValue FBYGNodeStyleGetHeight(FBYGNodeConstRef node) {
  return (FBYGValue)resolveRef(node)->style().dimension(Dimension::Height);
}

void FBYGNodeStyleSetMinWidth(const FBYGNodeRef node, const float minWidth) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Width, value::points(minWidth));
}

void FBYGNodeStyleSetMinWidthPercent(
    const FBYGNodeRef node,
    const float minWidth) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Width, value::percent(minWidth));
}

FBYGValue FBYGNodeStyleGetMinWidth(const FBYGNodeConstRef node) {
  return (FBYGValue)resolveRef(node)->style().minDimension(Dimension::Width);
}

void FBYGNodeStyleSetMinHeight(const FBYGNodeRef node, const float minHeight) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Height, value::points(minHeight));
}

void FBYGNodeStyleSetMinHeightPercent(
    const FBYGNodeRef node,
    const float minHeight) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Height, value::percent(minHeight));
}

FBYGValue FBYGNodeStyleGetMinHeight(const FBYGNodeConstRef node) {
  return (FBYGValue)resolveRef(node)->style().minDimension(Dimension::Height);
}

void FBYGNodeStyleSetMaxWidth(const FBYGNodeRef node, const float maxWidth) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Width, value::points(maxWidth));
}

void FBYGNodeStyleSetMaxWidthPercent(
    const FBYGNodeRef node,
    const float maxWidth) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Width, value::percent(maxWidth));
}

FBYGValue FBYGNodeStyleGetMaxWidth(const FBYGNodeConstRef node) {
  return (FBYGValue)resolveRef(node)->style().maxDimension(Dimension::Width);
}

void FBYGNodeStyleSetMaxHeight(const FBYGNodeRef node, const float maxHeight) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Height, value::points(maxHeight));
}

void FBYGNodeStyleSetMaxHeightPercent(
    const FBYGNodeRef node,
    const float maxHeight) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Height, value::percent(maxHeight));
}

FBYGValue FBYGNodeStyleGetMaxHeight(const FBYGNodeConstRef node) {
  return (FBYGValue)resolveRef(node)->style().maxDimension(Dimension::Height);
}
