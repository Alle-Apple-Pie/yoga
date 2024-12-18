/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stddef.h>

#include <yoga/YGNode.h>
#include <yoga/YGValue.h>

FBYG_EXTERN_C_BEGIN

FBYG_EXPORT void FBYGNodeCopyStyle(YGNodeRef dstNode, FBYGNodeConstRef srcNode);

FBYG_EXPORT void FBYGNodeStyleSetDirection(YGNodeRef node, FBYGDirection direction);
FBYG_EXPORT FBYGDirection FBYGNodeStyleGetDirection(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexDirection(
    FBYGNodeRef node,
    FBYGFlexDirection flexDirection);
FBYG_EXPORT FBYGFlexDirection FBYGNodeStyleGetFlexDirection(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetJustifyContent(
    FBYGNodeRef node,
    FBYGJustify justifyContent);
FBYG_EXPORT FBYGJustify FBYGNodeStyleGetJustifyContent(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetAlignContent(YGNodeRef node, FBYGAlign alignContent);
FBYG_EXPORT FBYGAlign FBYGNodeStyleGetAlignContent(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetAlignItems(YGNodeRef node, FBYGAlign alignItems);
FBYG_EXPORT FBYGAlign FBYGNodeStyleGetAlignItems(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetAlignSelf(YGNodeRef node, FBYGAlign alignSelf);
FBYG_EXPORT FBYGAlign FBYGNodeStyleGetAlignSelf(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetPositionType(
    FBYGNodeRef node,
    FBYGPositionType positionType);
FBYG_EXPORT FBYGPositionType FBYGNodeStyleGetPositionType(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexWrap(YGNodeRef node, FBYGWrap flexWrap);
FBYG_EXPORT FBYGWrap FBYGNodeStyleGetFlexWrap(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetOverflow(YGNodeRef node, FBYGOverflow overflow);
FBYG_EXPORT FBYGOverflow FBYGNodeStyleGetOverflow(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetDisplay(YGNodeRef node, FBYGDisplay display);
FBYG_EXPORT FBYGDisplay FBYGNodeStyleGetDisplay(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlex(YGNodeRef node, float flex);
FBYG_EXPORT float FBYGNodeStyleGetFlex(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexGrow(YGNodeRef node, float flexGrow);
FBYG_EXPORT float FBYGNodeStyleGetFlexGrow(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexShrink(YGNodeRef node, float flexShrink);
FBYG_EXPORT float FBYGNodeStyleGetFlexShrink(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexBasis(YGNodeRef node, float flexBasis);
FBYG_EXPORT void FBYGNodeStyleSetFlexBasisPercent(YGNodeRef node, float flexBasis);
FBYG_EXPORT void FBYGNodeStyleSetFlexBasisAuto(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetFlexBasisMaxContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetFlexBasisFitContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetFlexBasisStretch(YGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetFlexBasis(YGNodeConstRef node);

FBYG_EXPORT void
FBYGNodeStyleSetPosition(YGNodeRef node, FBYGEdge edge, float position);
FBYG_EXPORT void
FBYGNodeStyleSetPositionPercent(YGNodeRef node, FBYGEdge edge, float position);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetPosition(YGNodeConstRef node, FBYGEdge edge);
FBYG_EXPORT void FBYGNodeStyleSetPositionAuto(YGNodeRef node, FBYGEdge edge);

FBYG_EXPORT
void FBYGNodeStyleSetMargin(YGNodeRef node, FBYGEdge edge, float margin);
FBYG_EXPORT void
FBYGNodeStyleSetMarginPercent(YGNodeRef node, FBYGEdge edge, float margin);
FBYG_EXPORT void FBYGNodeStyleSetMarginAuto(YGNodeRef node, FBYGEdge edge);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMargin(YGNodeConstRef node, FBYGEdge edge);

FBYG_EXPORT void
FBYGNodeStyleSetPadding(YGNodeRef node, FBYGEdge edge, float padding);
FBYG_EXPORT void
FBYGNodeStyleSetPaddingPercent(YGNodeRef node, FBYGEdge edge, float padding);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetPadding(YGNodeConstRef node, FBYGEdge edge);

FBYG_EXPORT void FBYGNodeStyleSetBorder(YGNodeRef node, FBYGEdge edge, float border);
FBYG_EXPORT float FBYGNodeStyleGetBorder(YGNodeConstRef node, FBYGEdge edge);

FBYG_EXPORT void
FBYGNodeStyleSetGap(YGNodeRef node, FBYGGutter gutter, float gapLength);
FBYG_EXPORT void
FBYGNodeStyleSetGapPercent(YGNodeRef node, FBYGGutter gutter, float gapLength);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetGap(YGNodeConstRef node, FBYGGutter gutter);

FBYG_EXPORT void FBYGNodeStyleSetBoxSizing(YGNodeRef node, YGBoxSizing boxSizing);
FBYG_EXPORT YGBoxSizing FBYGNodeStyleGetBoxSizing(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetWidth(YGNodeRef node, float width);
FBYG_EXPORT void FBYGNodeStyleSetWidthPercent(YGNodeRef node, float width);
FBYG_EXPORT void FBYGNodeStyleSetWidthAuto(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetWidthMaxContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetWidthFitContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetWidthStretch(YGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetWidth(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetHeight(YGNodeRef node, float height);
FBYG_EXPORT void FBYGNodeStyleSetHeightPercent(YGNodeRef node, float height);
FBYG_EXPORT void FBYGNodeStyleSetHeightAuto(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetHeightMaxContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetHeightFitContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetHeightStretch(YGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetHeight(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetMinWidth(YGNodeRef node, float minWidth);
FBYG_EXPORT void FBYGNodeStyleSetMinWidthPercent(YGNodeRef node, float minWidth);
FBYG_EXPORT void FBYGNodeStyleSetMinWidthMaxContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetMinWidthFitContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetMinWidthStretch(YGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMinWidth(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetMinHeight(YGNodeRef node, float minHeight);
FBYG_EXPORT void FBYGNodeStyleSetMinHeightPercent(YGNodeRef node, float minHeight);
FBYG_EXPORT void FBYGNodeStyleSetMinHeightMaxContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetMinHeightFitContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetMinHeightStretch(YGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMinHeight(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetMaxWidth(YGNodeRef node, float maxWidth);
FBYG_EXPORT void FBYGNodeStyleSetMaxWidthPercent(YGNodeRef node, float maxWidth);
FBYG_EXPORT void FBYGNodeStyleSetMaxWidthMaxContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetMaxWidthFitContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetMaxWidthStretch(YGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMaxWidth(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetMaxHeight(YGNodeRef node, float maxHeight);
FBYG_EXPORT void FBYGNodeStyleSetMaxHeightPercent(YGNodeRef node, float maxHeight);
FBYG_EXPORT void FBYGNodeStyleSetMaxHeightMaxContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetMaxHeightFitContent(YGNodeRef node);
FBYG_EXPORT void FBYGNodeStyleSetMaxHeightStretch(YGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMaxHeight(YGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetAspectRatio(YGNodeRef node, float aspectRatio);
FBYG_EXPORT float FBYGNodeStyleGetAspectRatio(YGNodeConstRef node);

FBYG_EXTERN_C_END
