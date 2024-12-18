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

YG_EXTERN_C_BEGIN

YG_EXPORT void FBYGNodeCopyStyle(YGNodeRef dstNode, FBYGNodeConstRef srcNode);

YG_EXPORT void FBYGNodeStyleSetDirection(YGNodeRef node, FBYGDirection direction);
YG_EXPORT FBYGDirection FBYGNodeStyleGetDirection(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetFlexDirection(
    FBYGNodeRef node,
    FBYGFlexDirection flexDirection);
YG_EXPORT FBYGFlexDirection FBYGNodeStyleGetFlexDirection(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetJustifyContent(
    FBYGNodeRef node,
    FBYGJustify justifyContent);
YG_EXPORT FBYGJustify FBYGNodeStyleGetJustifyContent(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetAlignContent(YGNodeRef node, FBYGAlign alignContent);
YG_EXPORT FBYGAlign FBYGNodeStyleGetAlignContent(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetAlignItems(YGNodeRef node, FBYGAlign alignItems);
YG_EXPORT FBYGAlign FBYGNodeStyleGetAlignItems(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetAlignSelf(YGNodeRef node, FBYGAlign alignSelf);
YG_EXPORT FBYGAlign FBYGNodeStyleGetAlignSelf(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetPositionType(
    FBYGNodeRef node,
    FBYGPositionType positionType);
YG_EXPORT FBYGPositionType FBYGNodeStyleGetPositionType(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetFlexWrap(YGNodeRef node, FBYGWrap flexWrap);
YG_EXPORT FBYGWrap FBYGNodeStyleGetFlexWrap(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetOverflow(YGNodeRef node, FBYGOverflow overflow);
YG_EXPORT FBYGOverflow FBYGNodeStyleGetOverflow(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetDisplay(YGNodeRef node, FBYGDisplay display);
YG_EXPORT FBYGDisplay FBYGNodeStyleGetDisplay(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetFlex(YGNodeRef node, float flex);
YG_EXPORT float FBYGNodeStyleGetFlex(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetFlexGrow(YGNodeRef node, float flexGrow);
YG_EXPORT float FBYGNodeStyleGetFlexGrow(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetFlexShrink(YGNodeRef node, float flexShrink);
YG_EXPORT float FBYGNodeStyleGetFlexShrink(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetFlexBasis(YGNodeRef node, float flexBasis);
YG_EXPORT void FBYGNodeStyleSetFlexBasisPercent(YGNodeRef node, float flexBasis);
YG_EXPORT void FBYGNodeStyleSetFlexBasisAuto(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetFlexBasisMaxContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetFlexBasisFitContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetFlexBasisStretch(YGNodeRef node);
YG_EXPORT FBYGValue FBYGNodeStyleGetFlexBasis(YGNodeConstRef node);

YG_EXPORT void
YGNodeStyleSetPosition(YGNodeRef node, FBYGEdge edge, float position);
YG_EXPORT void
YGNodeStyleSetPositionPercent(YGNodeRef node, FBYGEdge edge, float position);
YG_EXPORT FBYGValue FBYGNodeStyleGetPosition(YGNodeConstRef node, FBYGEdge edge);
YG_EXPORT void FBYGNodeStyleSetPositionAuto(YGNodeRef node, FBYGEdge edge);

YG_EXPORT
void FBYGNodeStyleSetMargin(YGNodeRef node, FBYGEdge edge, float margin);
YG_EXPORT void
YGNodeStyleSetMarginPercent(YGNodeRef node, FBYGEdge edge, float margin);
YG_EXPORT void FBYGNodeStyleSetMarginAuto(YGNodeRef node, FBYGEdge edge);
YG_EXPORT FBYGValue FBYGNodeStyleGetMargin(YGNodeConstRef node, FBYGEdge edge);

YG_EXPORT void
YGNodeStyleSetPadding(YGNodeRef node, FBYGEdge edge, float padding);
YG_EXPORT void
YGNodeStyleSetPaddingPercent(YGNodeRef node, FBYGEdge edge, float padding);
YG_EXPORT FBYGValue FBYGNodeStyleGetPadding(YGNodeConstRef node, FBYGEdge edge);

YG_EXPORT void FBYGNodeStyleSetBorder(YGNodeRef node, FBYGEdge edge, float border);
YG_EXPORT float FBYGNodeStyleGetBorder(YGNodeConstRef node, FBYGEdge edge);

YG_EXPORT void
YGNodeStyleSetGap(YGNodeRef node, FBYGGutter gutter, float gapLength);
YG_EXPORT void
YGNodeStyleSetGapPercent(YGNodeRef node, FBYGGutter gutter, float gapLength);
YG_EXPORT FBYGValue FBYGNodeStyleGetGap(YGNodeConstRef node, FBYGGutter gutter);

YG_EXPORT void FBYGNodeStyleSetBoxSizing(YGNodeRef node, YGBoxSizing boxSizing);
YG_EXPORT YGBoxSizing FBYGNodeStyleGetBoxSizing(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetWidth(YGNodeRef node, float width);
YG_EXPORT void FBYGNodeStyleSetWidthPercent(YGNodeRef node, float width);
YG_EXPORT void FBYGNodeStyleSetWidthAuto(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetWidthMaxContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetWidthFitContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetWidthStretch(YGNodeRef node);
YG_EXPORT FBYGValue FBYGNodeStyleGetWidth(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetHeight(YGNodeRef node, float height);
YG_EXPORT void FBYGNodeStyleSetHeightPercent(YGNodeRef node, float height);
YG_EXPORT void FBYGNodeStyleSetHeightAuto(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetHeightMaxContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetHeightFitContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetHeightStretch(YGNodeRef node);
YG_EXPORT FBYGValue FBYGNodeStyleGetHeight(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetMinWidth(YGNodeRef node, float minWidth);
YG_EXPORT void FBYGNodeStyleSetMinWidthPercent(YGNodeRef node, float minWidth);
YG_EXPORT void FBYGNodeStyleSetMinWidthMaxContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetMinWidthFitContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetMinWidthStretch(YGNodeRef node);
YG_EXPORT FBYGValue FBYGNodeStyleGetMinWidth(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetMinHeight(YGNodeRef node, float minHeight);
YG_EXPORT void FBYGNodeStyleSetMinHeightPercent(YGNodeRef node, float minHeight);
YG_EXPORT void FBYGNodeStyleSetMinHeightMaxContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetMinHeightFitContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetMinHeightStretch(YGNodeRef node);
YG_EXPORT FBYGValue FBYGNodeStyleGetMinHeight(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetMaxWidth(YGNodeRef node, float maxWidth);
YG_EXPORT void FBYGNodeStyleSetMaxWidthPercent(YGNodeRef node, float maxWidth);
YG_EXPORT void FBYGNodeStyleSetMaxWidthMaxContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetMaxWidthFitContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetMaxWidthStretch(YGNodeRef node);
YG_EXPORT FBYGValue FBYGNodeStyleGetMaxWidth(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetMaxHeight(YGNodeRef node, float maxHeight);
YG_EXPORT void FBYGNodeStyleSetMaxHeightPercent(YGNodeRef node, float maxHeight);
YG_EXPORT void FBYGNodeStyleSetMaxHeightMaxContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetMaxHeightFitContent(YGNodeRef node);
YG_EXPORT void FBYGNodeStyleSetMaxHeightStretch(YGNodeRef node);
YG_EXPORT FBYGValue FBYGNodeStyleGetMaxHeight(YGNodeConstRef node);

YG_EXPORT void FBYGNodeStyleSetAspectRatio(YGNodeRef node, float aspectRatio);
YG_EXPORT float FBYGNodeStyleGetAspectRatio(YGNodeConstRef node);

YG_EXTERN_C_END
