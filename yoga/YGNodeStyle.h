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

FBYG_EXPORT void FBYGNodeCopyStyle(
    FBYGNodeRef dstNode,
    FBYGNodeConstRef srcNode);

FBYG_EXPORT void FBYGNodeStyleSetDirection(
    FBYGNodeRef node,
    FBYGDirection direction);
FBYG_EXPORT FBYGDirection FBYGNodeStyleGetDirection(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexDirection(
    FBYGNodeRef node,
    FBYGFlexDirection flexDirection);
FBYG_EXPORT FBYGFlexDirection
FBYGNodeStyleGetFlexDirection(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetJustifyContent(
    FBYGNodeRef node,
    FBYGJustify justifyContent);
FBYG_EXPORT FBYGJustify FBYGNodeStyleGetJustifyContent(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetAlignContent(
    FBYGNodeRef node,
    FBYGAlign alignContent);
FBYG_EXPORT FBYGAlign FBYGNodeStyleGetAlignContent(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetAlignItems(
    FBYGNodeRef node,
    FBYGAlign alignItems);
FBYG_EXPORT FBYGAlign FBYGNodeStyleGetAlignItems(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetAlignSelf(
    FBYGNodeRef node,
    FBYGAlign alignSelf);
FBYG_EXPORT FBYGAlign FBYGNodeStyleGetAlignSelf(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetPositionType(
    FBYGNodeRef node,
    FBYGPositionType positionType);
FBYG_EXPORT FBYGPositionType
FBYGNodeStyleGetPositionType(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexWrap(FBYGNodeRef node, FBYGWrap flexWrap);
FBYG_EXPORT FBYGWrap FBYGNodeStyleGetFlexWrap(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetOverflow(
    FBYGNodeRef node,
    FBYGOverflow overflow);
FBYG_EXPORT FBYGOverflow FBYGNodeStyleGetOverflow(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetDisplay(FBYGNodeRef node, FBYGDisplay display);
FBYG_EXPORT FBYGDisplay FBYGNodeStyleGetDisplay(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlex(FBYGNodeRef node, float flex);
FBYG_EXPORT float FBYGNodeStyleGetFlex(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexGrow(FBYGNodeRef node, float flexGrow);
FBYG_EXPORT float FBYGNodeStyleGetFlexGrow(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexShrink(FBYGNodeRef node, float flexShrink);
FBYG_EXPORT float FBYGNodeStyleGetFlexShrink(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetFlexBasis(FBYGNodeRef node, float flexBasis);
FBYG_EXPORT void FBYGNodeStyleSetFlexBasisPercent(
    FBYGNodeRef node,
    float flexBasis);
FBYG_EXPORT void FBYGNodeStyleSetFlexBasisAuto(FBYGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetFlexBasis(FBYGNodeConstRef node);

FBYG_EXPORT void
FBYGNodeStyleSetPosition(FBYGNodeRef node, FBYGEdge edge, float position);
FBYG_EXPORT void FBYGNodeStyleSetPositionPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float position);
FBYG_EXPORT FBYGValue
FBYGNodeStyleGetPosition(FBYGNodeConstRef node, FBYGEdge edge);

FBYG_EXPORT void
FBYGNodeStyleSetMargin(FBYGNodeRef node, FBYGEdge edge, float margin);
FBYG_EXPORT void
FBYGNodeStyleSetMarginPercent(FBYGNodeRef node, FBYGEdge edge, float margin);
FBYG_EXPORT void FBYGNodeStyleSetMarginAuto(FBYGNodeRef node, FBYGEdge edge);
FBYG_EXPORT FBYGValue
FBYGNodeStyleGetMargin(FBYGNodeConstRef node, FBYGEdge edge);

FBYG_EXPORT void
FBYGNodeStyleSetPadding(FBYGNodeRef node, FBYGEdge edge, float padding);
FBYG_EXPORT void
FBYGNodeStyleSetPaddingPercent(FBYGNodeRef node, FBYGEdge edge, float padding);
FBYG_EXPORT FBYGValue
FBYGNodeStyleGetPadding(FBYGNodeConstRef node, FBYGEdge edge);

FBYG_EXPORT void
FBYGNodeStyleSetBorder(FBYGNodeRef node, FBYGEdge edge, float border);
FBYG_EXPORT float FBYGNodeStyleGetBorder(FBYGNodeConstRef node, FBYGEdge edge);

FBYG_EXPORT void
FBYGNodeStyleSetGap(FBYGNodeRef node, FBYGGutter gutter, float gapLength);
FBYG_EXPORT float FBYGNodeStyleGetGap(FBYGNodeConstRef node, FBYGGutter gutter);

FBYG_EXPORT void FBYGNodeStyleSetWidth(FBYGNodeRef node, float width);
FBYG_EXPORT void FBYGNodeStyleSetWidthPercent(FBYGNodeRef node, float width);
FBYG_EXPORT void FBYGNodeStyleSetWidthAuto(FBYGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetWidth(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetHeight(FBYGNodeRef node, float height);
FBYG_EXPORT void FBYGNodeStyleSetHeightPercent(FBYGNodeRef node, float height);
FBYG_EXPORT void FBYGNodeStyleSetHeightAuto(FBYGNodeRef node);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetHeight(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetMinWidth(FBYGNodeRef node, float minWidth);
FBYG_EXPORT void FBYGNodeStyleSetMinWidthPercent(
    FBYGNodeRef node,
    float minWidth);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMinWidth(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetMinHeight(FBYGNodeRef node, float minHeight);
FBYG_EXPORT void FBYGNodeStyleSetMinHeightPercent(
    FBYGNodeRef node,
    float minHeight);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMinHeight(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetMaxWidth(FBYGNodeRef node, float maxWidth);
FBYG_EXPORT void FBYGNodeStyleSetMaxWidthPercent(
    FBYGNodeRef node,
    float maxWidth);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMaxWidth(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetMaxHeight(FBYGNodeRef node, float maxHeight);
FBYG_EXPORT void FBYGNodeStyleSetMaxHeightPercent(
    FBYGNodeRef node,
    float maxHeight);
FBYG_EXPORT FBYGValue FBYGNodeStyleGetMaxHeight(FBYGNodeConstRef node);

FBYG_EXPORT void FBYGNodeStyleSetAspectRatio(
    FBYGNodeRef node,
    float aspectRatio);
FBYG_EXPORT float FBYGNodeStyleGetAspectRatio(FBYGNodeConstRef node);

FBYG_EXTERN_C_END
