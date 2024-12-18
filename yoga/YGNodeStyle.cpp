/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>
#include <yoga/debug/AssertFatal.h>
#include <yoga/node/Node.h>

using namespace facebook;
using namespace facebook::yoga;

namespace {

template <auto GetterT, auto SetterT, typename ValueT>
void updateStyle(YGNodeRef node, ValueT value) {
  auto& style = resolveRef(node)->style();
  if ((style.*GetterT)() != value) {
    (style.*SetterT)(value);
    resolveRef(node)->markDirtyAndPropagate();
  }
}

template <auto GetterT, auto SetterT, typename IdxT, typename ValueT>
void updateStyle(YGNodeRef node, IdxT idx, ValueT value) {
  auto& style = resolveRef(node)->style();
  if ((style.*GetterT)(idx) != value) {
    (style.*SetterT)(idx, value);
    resolveRef(node)->markDirtyAndPropagate();
  }
}

} // namespace

void FBYGNodeCopyStyle(YGNodeRef dstNode, FBYGNodeConstRef srcNode) {
  auto dst = resolveRef(dstNode);
  auto src = resolveRef(srcNode);

  if (dst->style() != src->style()) {
    dst->setStyle(src->style());
    dst->markDirtyAndPropagate();
  }
}

void FBYGNodeStyleSetDirection(const FBYGNodeRef node, const FBYGDirection value) {
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

void FBYGNodeStyleSetAlignItems(const FBYGNodeRef node, const FBYGAlign alignItems) {
  updateStyle<&Style::alignItems, &Style::setAlignItems>(
      node, scopedEnum(alignItems));
}

FBYGAlign FBYGNodeStyleGetAlignItems(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().alignItems());
}

void FBYGNodeStyleSetAlignSelf(const FBYGNodeRef node, const FBYGAlign alignSelf) {
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

void FBYGNodeStyleSetOverflow(const FBYGNodeRef node, const FBYGOverflow overflow) {
  updateStyle<&Style::overflow, &Style::setOverflow>(
      node, scopedEnum(overflow));
}

FBYGOverflow FBYGNodeStyleGetOverflow(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().overflow());
}

void FBYGNodeStyleSetDisplay(const FBYGNodeRef node, const FBYGDisplay display) {
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

void FBYGNodeStyleSetFlexShrink(const FBYGNodeRef node, const float flexShrink) {
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
      node, StyleSizeLength::points(flexBasis));
}

void FBYGNodeStyleSetFlexBasisPercent(
    const FBYGNodeRef node,
    const float flexBasisPercent) {
  updateStyle<&Style::flexBasis, &Style::setFlexBasis>(
      node, StyleSizeLength::percent(flexBasisPercent));
}

void FBYGNodeStyleSetFlexBasisAuto(const FBYGNodeRef node) {
  updateStyle<&Style::flexBasis, &Style::setFlexBasis>(
      node, StyleSizeLength::ofAuto());
}

void FBYGNodeStyleSetFlexBasisMaxContent(const FBYGNodeRef node) {
  updateStyle<&Style::flexBasis, &Style::setFlexBasis>(
      node, StyleSizeLength::ofMaxContent());
}

void FBYGNodeStyleSetFlexBasisFitContent(const FBYGNodeRef node) {
  updateStyle<&Style::flexBasis, &Style::setFlexBasis>(
      node, StyleSizeLength::ofFitContent());
}

void FBYGNodeStyleSetFlexBasisStretch(const FBYGNodeRef node) {
  updateStyle<&Style::flexBasis, &Style::setFlexBasis>(
      node, StyleSizeLength::ofStretch());
}

FBYGValue FBYGNodeStyleGetFlexBasis(const FBYGNodeConstRef node) {
  return (YGValue)resolveRef(node)->style().flexBasis();
}

void FBYGNodeStyleSetPosition(YGNodeRef node, FBYGEdge edge, float points) {
  updateStyle<&Style::position, &Style::setPosition>(
      node, scopedEnum(edge), StyleLength::points(points));
}

void FBYGNodeStyleSetPositionPercent(YGNodeRef node, FBYGEdge edge, float percent) {
  updateStyle<&Style::position, &Style::setPosition>(
      node, scopedEnum(edge), StyleLength::percent(percent));
}

void FBYGNodeStyleSetPositionAuto(YGNodeRef node, FBYGEdge edge) {
  updateStyle<&Style::position, &Style::setPosition>(
      node, scopedEnum(edge), StyleLength::ofAuto());
}

FBYGValue FBYGNodeStyleGetPosition(YGNodeConstRef node, FBYGEdge edge) {
  return (YGValue)resolveRef(node)->style().position(scopedEnum(edge));
}

void FBYGNodeStyleSetMargin(YGNodeRef node, FBYGEdge edge, float points) {
  updateStyle<&Style::margin, &Style::setMargin>(
      node, scopedEnum(edge), StyleLength::points(points));
}

void FBYGNodeStyleSetMarginPercent(YGNodeRef node, FBYGEdge edge, float percent) {
  updateStyle<&Style::margin, &Style::setMargin>(
      node, scopedEnum(edge), StyleLength::percent(percent));
}

void FBYGNodeStyleSetMarginAuto(YGNodeRef node, FBYGEdge edge) {
  updateStyle<&Style::margin, &Style::setMargin>(
      node, scopedEnum(edge), StyleLength::ofAuto());
}

FBYGValue FBYGNodeStyleGetMargin(YGNodeConstRef node, FBYGEdge edge) {
  return (YGValue)resolveRef(node)->style().margin(scopedEnum(edge));
}

void FBYGNodeStyleSetPadding(YGNodeRef node, FBYGEdge edge, float points) {
  updateStyle<&Style::padding, &Style::setPadding>(
      node, scopedEnum(edge), StyleLength::points(points));
}

void FBYGNodeStyleSetPaddingPercent(YGNodeRef node, FBYGEdge edge, float percent) {
  updateStyle<&Style::padding, &Style::setPadding>(
      node, scopedEnum(edge), StyleLength::percent(percent));
}

FBYGValue FBYGNodeStyleGetPadding(YGNodeConstRef node, FBYGEdge edge) {
  return (YGValue)resolveRef(node)->style().padding(scopedEnum(edge));
}

void FBYGNodeStyleSetBorder(
    const FBYGNodeRef node,
    const FBYGEdge edge,
    const float border) {
  updateStyle<&Style::border, &Style::setBorder>(
      node, scopedEnum(edge), StyleLength::points(border));
}

float FBYGNodeStyleGetBorder(const FBYGNodeConstRef node, const FBYGEdge edge) {
  auto border = resolveRef(node)->style().border(scopedEnum(edge));
  if (border.isUndefined() || border.isAuto()) {
    return FBYGUndefined;
  }

  return static_cast<YGValue>(border).value;
}

void FBYGNodeStyleSetGap(
    const FBYGNodeRef node,
    const FBYGGutter gutter,
    const float gapLength) {
  updateStyle<&Style::gap, &Style::setGap>(
      node, scopedEnum(gutter), StyleLength::points(gapLength));
}

void FBYGNodeStyleSetGapPercent(YGNodeRef node, FBYGGutter gutter, float percent) {
  updateStyle<&Style::gap, &Style::setGap>(
      node, scopedEnum(gutter), StyleLength::percent(percent));
}

FBYGValue FBYGNodeStyleGetGap(const FBYGNodeConstRef node, const FBYGGutter gutter) {
  return (YGValue)resolveRef(node)->style().gap(scopedEnum(gutter));
}

void FBYGNodeStyleSetAspectRatio(const FBYGNodeRef node, const float aspectRatio) {
  updateStyle<&Style::aspectRatio, &Style::setAspectRatio>(
      node, FloatOptional{aspectRatio});
}

float FBYGNodeStyleGetAspectRatio(const FBYGNodeConstRef node) {
  const FloatOptional op = resolveRef(node)->style().aspectRatio();
  return op.isUndefined() ? FBYGUndefined : op.unwrap();
}

void FBYGNodeStyleSetBoxSizing(YGNodeRef node, YGBoxSizing boxSizing) {
  updateStyle<&Style::boxSizing, &Style::setBoxSizing>(
      node, scopedEnum(boxSizing));
}

YGBoxSizing FBYGNodeStyleGetBoxSizing(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->style().boxSizing());
}

void FBYGNodeStyleSetWidth(YGNodeRef node, float points) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, StyleSizeLength::points(points));
}

void FBYGNodeStyleSetWidthPercent(YGNodeRef node, float percent) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, StyleSizeLength::percent(percent));
}

void FBYGNodeStyleSetWidthAuto(YGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, StyleSizeLength::ofAuto());
}

void FBYGNodeStyleSetWidthMaxContent(YGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, StyleSizeLength::ofMaxContent());
}

void FBYGNodeStyleSetWidthFitContent(YGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, StyleSizeLength::ofFitContent());
}

void FBYGNodeStyleSetWidthStretch(YGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Width, StyleSizeLength::ofStretch());
}

FBYGValue FBYGNodeStyleGetWidth(YGNodeConstRef node) {
  return (YGValue)resolveRef(node)->style().dimension(Dimension::Width);
}

void FBYGNodeStyleSetHeight(YGNodeRef node, float points) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, StyleSizeLength::points(points));
}

void FBYGNodeStyleSetHeightPercent(YGNodeRef node, float percent) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, StyleSizeLength::percent(percent));
}

void FBYGNodeStyleSetHeightAuto(YGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, StyleSizeLength::ofAuto());
}

void FBYGNodeStyleSetHeightMaxContent(YGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, StyleSizeLength::ofMaxContent());
}

void FBYGNodeStyleSetHeightFitContent(YGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, StyleSizeLength::ofFitContent());
}

void FBYGNodeStyleSetHeightStretch(YGNodeRef node) {
  updateStyle<&Style::dimension, &Style::setDimension>(
      node, Dimension::Height, StyleSizeLength::ofStretch());
}

FBYGValue FBYGNodeStyleGetHeight(YGNodeConstRef node) {
  return (YGValue)resolveRef(node)->style().dimension(Dimension::Height);
}

void FBYGNodeStyleSetMinWidth(const FBYGNodeRef node, const float minWidth) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Width, StyleSizeLength::points(minWidth));
}

void FBYGNodeStyleSetMinWidthPercent(const FBYGNodeRef node, const float minWidth) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Width, StyleSizeLength::percent(minWidth));
}

void FBYGNodeStyleSetMinWidthMaxContent(const FBYGNodeRef node) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Width, StyleSizeLength::ofMaxContent());
}

void FBYGNodeStyleSetMinWidthFitContent(const FBYGNodeRef node) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Width, StyleSizeLength::ofFitContent());
}

void FBYGNodeStyleSetMinWidthStretch(const FBYGNodeRef node) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Width, StyleSizeLength::ofStretch());
}

FBYGValue FBYGNodeStyleGetMinWidth(const FBYGNodeConstRef node) {
  return (YGValue)resolveRef(node)->style().minDimension(Dimension::Width);
}

void FBYGNodeStyleSetMinHeight(const FBYGNodeRef node, const float minHeight) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Height, StyleSizeLength::points(minHeight));
}

void FBYGNodeStyleSetMinHeightPercent(
    const FBYGNodeRef node,
    const float minHeight) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Height, StyleSizeLength::percent(minHeight));
}

void FBYGNodeStyleSetMinHeightMaxContent(const FBYGNodeRef node) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Height, StyleSizeLength::ofMaxContent());
}

void FBYGNodeStyleSetMinHeightFitContent(const FBYGNodeRef node) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Height, StyleSizeLength::ofFitContent());
}

void FBYGNodeStyleSetMinHeightStretch(const FBYGNodeRef node) {
  updateStyle<&Style::minDimension, &Style::setMinDimension>(
      node, Dimension::Height, StyleSizeLength::ofStretch());
}

FBYGValue FBYGNodeStyleGetMinHeight(const FBYGNodeConstRef node) {
  return (YGValue)resolveRef(node)->style().minDimension(Dimension::Height);
}

void FBYGNodeStyleSetMaxWidth(const FBYGNodeRef node, const float maxWidth) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Width, StyleSizeLength::points(maxWidth));
}

void FBYGNodeStyleSetMaxWidthPercent(const FBYGNodeRef node, const float maxWidth) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Width, StyleSizeLength::percent(maxWidth));
}

void FBYGNodeStyleSetMaxWidthMaxContent(const FBYGNodeRef node) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Width, StyleSizeLength::ofMaxContent());
}

void FBYGNodeStyleSetMaxWidthFitContent(const FBYGNodeRef node) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Width, StyleSizeLength::ofFitContent());
}

void FBYGNodeStyleSetMaxWidthStretch(const FBYGNodeRef node) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Width, StyleSizeLength::ofStretch());
}

FBYGValue FBYGNodeStyleGetMaxWidth(const FBYGNodeConstRef node) {
  return (YGValue)resolveRef(node)->style().maxDimension(Dimension::Width);
}

void FBYGNodeStyleSetMaxHeight(const FBYGNodeRef node, const float maxHeight) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Height, StyleSizeLength::points(maxHeight));
}

void FBYGNodeStyleSetMaxHeightPercent(
    const FBYGNodeRef node,
    const float maxHeight) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Height, StyleSizeLength::percent(maxHeight));
}

void FBYGNodeStyleSetMaxHeightMaxContent(const FBYGNodeRef node) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Height, StyleSizeLength::ofMaxContent());
}

void FBYGNodeStyleSetMaxHeightFitContent(const FBYGNodeRef node) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Height, StyleSizeLength::ofFitContent());
}

void FBYGNodeStyleSetMaxHeightStretch(const FBYGNodeRef node) {
  updateStyle<&Style::maxDimension, &Style::setMaxDimension>(
      node, Dimension::Height, StyleSizeLength::ofStretch());
}

FBYGValue FBYGNodeStyleGetMaxHeight(const FBYGNodeConstRef node) {
  return (YGValue)resolveRef(node)->style().maxDimension(Dimension::Height);
}
