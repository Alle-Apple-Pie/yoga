/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <yoga/YGEnums.h>
#include <yoga/YGMacros.h>
#include <yoga/YGValue.h>

FBYG_EXTERN_C_BEGIN

typedef struct FBYGSize {
  float width;
  float height;
} FBYGSize;

typedef struct FBYGConfig* FBYGConfigRef;

typedef struct FBYGNode* FBYGNodeRef;
typedef const struct FBYGNode* FBYGNodeConstRef;

typedef FBYGSize (*FBYGMeasureFunc)(
    FBYGNodeRef node,
    float width,
    FBYGMeasureMode widthMode,
    float height,
    FBYGMeasureMode heightMode);
typedef float (*FBYGBaselineFunc)(FBYGNodeRef node, float width, float height);
typedef void (*FBYGDirtiedFunc)(FBYGNodeRef node);
typedef void (*FBYGPrintFunc)(FBYGNodeRef node);
typedef void (*FBYGNodeCleanupFunc)(FBYGNodeRef node);
typedef int (*FBYGLogger)(
    FBYGConfigRef config,
    FBYGNodeRef node,
    FBYGLogLevel level,
    const char* format,
    va_list args);
typedef FBYGNodeRef (
    *FBYGCloneNodeFunc)(FBYGNodeRef oldNode, FBYGNodeRef owner, int childIndex);

// FBYGNode
WIN_EXPORT FBYGNodeRef FBYGNodeNew(void);
WIN_EXPORT FBYGNodeRef FBYGNodeNewWithConfig(FBYGConfigRef config);
WIN_EXPORT FBYGNodeRef FBYGNodeClone(FBYGNodeRef node);
WIN_EXPORT void FBYGNodeFree(FBYGNodeRef node);
WIN_EXPORT void FBYGNodeFreeRecursiveWithCleanupFunc(
    FBYGNodeRef node,
    FBYGNodeCleanupFunc cleanup);
WIN_EXPORT void FBYGNodeFreeRecursive(FBYGNodeRef node);
WIN_EXPORT void FBYGNodeReset(FBYGNodeRef node);

WIN_EXPORT void FBYGNodeInsertChild(
    FBYGNodeRef node,
    FBYGNodeRef child,
    uint32_t index);

WIN_EXPORT void FBYGNodeSwapChild(
    FBYGNodeRef node,
    FBYGNodeRef child,
    uint32_t index);

WIN_EXPORT void FBYGNodeRemoveChild(FBYGNodeRef node, FBYGNodeRef child);
WIN_EXPORT void FBYGNodeRemoveAllChildren(FBYGNodeRef node);
WIN_EXPORT FBYGNodeRef FBYGNodeGetChild(FBYGNodeRef node, uint32_t index);
WIN_EXPORT FBYGNodeRef FBYGNodeGetOwner(FBYGNodeRef node);
WIN_EXPORT FBYGNodeRef FBYGNodeGetParent(FBYGNodeRef node);
WIN_EXPORT uint32_t FBYGNodeGetChildCount(FBYGNodeRef node);
WIN_EXPORT void FBYGNodeSetChildren(
    FBYGNodeRef owner,
    const FBYGNodeRef* children,
    uint32_t count);

WIN_EXPORT void FBYGNodeSetIsReferenceBaseline(
    FBYGNodeRef node,
    bool isReferenceBaseline);

WIN_EXPORT bool FBYGNodeIsReferenceBaseline(FBYGNodeRef node);

WIN_EXPORT void FBYGNodeCalculateLayout(
    FBYGNodeRef node,
    float availableWidth,
    float availableHeight,
    FBYGDirection ownerDirection);

// Mark a node as dirty. Only valid for nodes with a custom measure function
// set.
//
// Yoga knows when to mark all other nodes as dirty but because nodes with
// measure functions depend on information not known to Yoga they must perform
// this dirty marking manually.
WIN_EXPORT void FBYGNodeMarkDirty(FBYGNodeRef node);

// Marks the current node and all its descendants as dirty.
//
// Intended to be used for Yoga benchmarks. Don't use in production, as calling
// `FBYGCalculateLayout` will cause the recalculation of each and every node.
WIN_EXPORT void FBYGNodeMarkDirtyAndPropagateToDescendants(FBYGNodeRef node);

WIN_EXPORT void FBYGNodePrint(FBYGNodeRef node, FBYGPrintOptions options);

WIN_EXPORT bool FBYGFloatIsUndefined(float value);

WIN_EXPORT bool FBYGNodeCanUseCachedMeasurement(
    FBYGMeasureMode widthMode,
    float width,
    FBYGMeasureMode heightMode,
    float height,
    FBYGMeasureMode lastWidthMode,
    float lastWidth,
    FBYGMeasureMode lastHeightMode,
    float lastHeight,
    float lastComputedWidth,
    float lastComputedHeight,
    float marginRow,
    float marginColumn,
    FBYGConfigRef config);

WIN_EXPORT void FBYGNodeCopyStyle(FBYGNodeRef dstNode, FBYGNodeRef srcNode);

WIN_EXPORT void* FBYGNodeGetContext(FBYGNodeRef node);
WIN_EXPORT void FBYGNodeSetContext(FBYGNodeRef node, void* context);

WIN_EXPORT FBYGConfigRef FBYGNodeGetConfig(FBYGNodeRef node);
WIN_EXPORT void FBYGNodeSetConfig(FBYGNodeRef node, FBYGConfigRef config);

void FBYGConfigSetPrintTreeFlag(FBYGConfigRef config, bool enabled);
bool FBYGNodeHasMeasureFunc(FBYGNodeRef node);
WIN_EXPORT void FBYGNodeSetMeasureFunc(FBYGNodeRef node, FBYGMeasureFunc measureFunc);
bool FBYGNodeHasBaselineFunc(FBYGNodeRef node);
void FBYGNodeSetBaselineFunc(FBYGNodeRef node, FBYGBaselineFunc baselineFunc);
FBYGDirtiedFunc FBYGNodeGetDirtiedFunc(FBYGNodeRef node);
void FBYGNodeSetDirtiedFunc(FBYGNodeRef node, FBYGDirtiedFunc dirtiedFunc);
void FBYGNodeSetPrintFunc(FBYGNodeRef node, FBYGPrintFunc printFunc);
WIN_EXPORT bool FBYGNodeGetHasNewLayout(FBYGNodeRef node);
WIN_EXPORT void FBYGNodeSetHasNewLayout(FBYGNodeRef node, bool hasNewLayout);
FBYGNodeType FBYGNodeGetNodeType(FBYGNodeRef node);
void FBYGNodeSetNodeType(FBYGNodeRef node, FBYGNodeType nodeType);
WIN_EXPORT bool FBYGNodeIsDirty(FBYGNodeRef node);

WIN_EXPORT void FBYGNodeStyleSetDirection(FBYGNodeRef node, FBYGDirection direction);
WIN_EXPORT FBYGDirection FBYGNodeStyleGetDirection(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetFlexDirection(
    FBYGNodeRef node,
    FBYGFlexDirection flexDirection);
WIN_EXPORT FBYGFlexDirection FBYGNodeStyleGetFlexDirection(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetJustifyContent(
    FBYGNodeRef node,
    FBYGJustify justifyContent);
WIN_EXPORT FBYGJustify FBYGNodeStyleGetJustifyContent(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetAlignContent(
    FBYGNodeRef node,
    FBYGAlign alignContent);
WIN_EXPORT FBYGAlign FBYGNodeStyleGetAlignContent(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetAlignItems(FBYGNodeRef node, FBYGAlign alignItems);
WIN_EXPORT FBYGAlign FBYGNodeStyleGetAlignItems(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetAlignSelf(FBYGNodeRef node, FBYGAlign alignSelf);
WIN_EXPORT FBYGAlign FBYGNodeStyleGetAlignSelf(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetPositionType(
    FBYGNodeRef node,
    FBYGPositionType positionType);
WIN_EXPORT FBYGPositionType FBYGNodeStyleGetPositionType(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetFlexWrap(FBYGNodeRef node, FBYGWrap flexWrap);
WIN_EXPORT FBYGWrap FBYGNodeStyleGetFlexWrap(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetOverflow(FBYGNodeRef node, FBYGOverflow overflow);
WIN_EXPORT FBYGOverflow FBYGNodeStyleGetOverflow(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetDisplay(FBYGNodeRef node, FBYGDisplay display);
WIN_EXPORT FBYGDisplay FBYGNodeStyleGetDisplay(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetFlex(FBYGNodeRef node, float flex);
WIN_EXPORT float FBYGNodeStyleGetFlex(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetFlexGrow(FBYGNodeRef node, float flexGrow);
WIN_EXPORT float FBYGNodeStyleGetFlexGrow(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetFlexShrink(FBYGNodeRef node, float flexShrink);
WIN_EXPORT float FBYGNodeStyleGetFlexShrink(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetFlexBasis(FBYGNodeRef node, float flexBasis);
WIN_EXPORT void FBYGNodeStyleSetFlexBasisPercent(FBYGNodeRef node, float flexBasis);
WIN_EXPORT void FBYGNodeStyleSetFlexBasisAuto(FBYGNodeRef node);
WIN_EXPORT FBYGValue FBYGNodeStyleGetFlexBasis(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetPosition(
    FBYGNodeRef node,
    FBYGEdge edge,
    float position);
WIN_EXPORT void FBYGNodeStyleSetPositionPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float position);
WIN_EXPORT FBYGValue FBYGNodeStyleGetPosition(FBYGNodeConstRef node, FBYGEdge edge);

WIN_EXPORT void FBYGNodeStyleSetMargin(FBYGNodeRef node, FBYGEdge edge, float margin);
WIN_EXPORT void FBYGNodeStyleSetMarginPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float margin);
WIN_EXPORT void FBYGNodeStyleSetMarginAuto(FBYGNodeRef node, FBYGEdge edge);
WIN_EXPORT FBYGValue FBYGNodeStyleGetMargin(FBYGNodeConstRef node, FBYGEdge edge);

WIN_EXPORT void FBYGNodeStyleSetPadding(
    FBYGNodeRef node,
    FBYGEdge edge,
    float padding);
WIN_EXPORT void FBYGNodeStyleSetPaddingPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float padding);
WIN_EXPORT FBYGValue FBYGNodeStyleGetPadding(FBYGNodeConstRef node, FBYGEdge edge);

WIN_EXPORT void FBYGNodeStyleSetBorder(FBYGNodeRef node, FBYGEdge edge, float border);
WIN_EXPORT float FBYGNodeStyleGetBorder(FBYGNodeConstRef node, FBYGEdge edge);

WIN_EXPORT void FBYGNodeStyleSetGap(
    FBYGNodeRef node,
    FBYGGutter gutter,
    float gapLength);
WIN_EXPORT float FBYGNodeStyleGetGap(FBYGNodeConstRef node, FBYGGutter gutter);

WIN_EXPORT void FBYGNodeStyleSetWidth(FBYGNodeRef node, float width);
WIN_EXPORT void FBYGNodeStyleSetWidthPercent(FBYGNodeRef node, float width);
WIN_EXPORT void FBYGNodeStyleSetWidthAuto(FBYGNodeRef node);
WIN_EXPORT FBYGValue FBYGNodeStyleGetWidth(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetHeight(FBYGNodeRef node, float height);
WIN_EXPORT void FBYGNodeStyleSetHeightPercent(FBYGNodeRef node, float height);
WIN_EXPORT void FBYGNodeStyleSetHeightAuto(FBYGNodeRef node);
WIN_EXPORT FBYGValue FBYGNodeStyleGetHeight(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetMinWidth(FBYGNodeRef node, float minWidth);
WIN_EXPORT void FBYGNodeStyleSetMinWidthPercent(FBYGNodeRef node, float minWidth);
WIN_EXPORT FBYGValue FBYGNodeStyleGetMinWidth(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetMinHeight(FBYGNodeRef node, float minHeight);
WIN_EXPORT void FBYGNodeStyleSetMinHeightPercent(FBYGNodeRef node, float minHeight);
WIN_EXPORT FBYGValue FBYGNodeStyleGetMinHeight(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetMaxWidth(FBYGNodeRef node, float maxWidth);
WIN_EXPORT void FBYGNodeStyleSetMaxWidthPercent(FBYGNodeRef node, float maxWidth);
WIN_EXPORT FBYGValue FBYGNodeStyleGetMaxWidth(FBYGNodeConstRef node);

WIN_EXPORT void FBYGNodeStyleSetMaxHeight(FBYGNodeRef node, float maxHeight);
WIN_EXPORT void FBYGNodeStyleSetMaxHeightPercent(FBYGNodeRef node, float maxHeight);
WIN_EXPORT FBYGValue FBYGNodeStyleGetMaxHeight(FBYGNodeConstRef node);

// Yoga specific properties, not compatible with flexbox specification Aspect
// ratio control the size of the undefined dimension of a node. Aspect ratio is
// encoded as a floating point value width/height. e.g. A value of 2 leads to a
// node with a width twice the size of its height while a value of 0.5 gives the
// opposite effect.
//
// - On a node with a set width/height aspect ratio control the size of the
//   unset dimension
// - On a node with a set flex basis aspect ratio controls the size of the node
//   in the cross axis if unset
// - On a node with a measure function aspect ratio works as though the measure
//   function measures the flex basis
// - On a node with flex grow/shrink aspect ratio controls the size of the node
//   in the cross axis if unset
// - Aspect ratio takes min/max dimensions into account
WIN_EXPORT void FBYGNodeStyleSetAspectRatio(FBYGNodeRef node, float aspectRatio);
WIN_EXPORT float FBYGNodeStyleGetAspectRatio(FBYGNodeConstRef node);

WIN_EXPORT float FBYGNodeLayoutGetLeft(FBYGNodeRef node);
WIN_EXPORT float FBYGNodeLayoutGetTop(FBYGNodeRef node);
WIN_EXPORT float FBYGNodeLayoutGetRight(FBYGNodeRef node);
WIN_EXPORT float FBYGNodeLayoutGetBottom(FBYGNodeRef node);
WIN_EXPORT float FBYGNodeLayoutGetWidth(FBYGNodeRef node);
WIN_EXPORT float FBYGNodeLayoutGetHeight(FBYGNodeRef node);
WIN_EXPORT FBYGDirection FBYGNodeLayoutGetDirection(FBYGNodeRef node);
WIN_EXPORT bool FBYGNodeLayoutGetHadOverflow(FBYGNodeRef node);

// Get the computed values for these nodes after performing layout. If they were
// set using point values then the returned value will be the same as
// FBYGNodeStyleGetXXX. However if they were set using a percentage value then the
// returned value is the computed value used during layout.
WIN_EXPORT float FBYGNodeLayoutGetMargin(FBYGNodeRef node, FBYGEdge edge);
WIN_EXPORT float FBYGNodeLayoutGetBorder(FBYGNodeRef node, FBYGEdge edge);
WIN_EXPORT float FBYGNodeLayoutGetPadding(FBYGNodeRef node, FBYGEdge edge);

WIN_EXPORT void FBYGConfigSetLogger(FBYGConfigRef config, FBYGLogger logger);
WIN_EXPORT void FBYGAssert(bool condition, const char* message);
WIN_EXPORT void FBYGAssertWithNode(
    FBYGNodeRef node,
    bool condition,
    const char* message);
WIN_EXPORT void FBYGAssertWithConfig(
    FBYGConfigRef config,
    bool condition,
    const char* message);
// Set this to number of pixels in 1 point to round calculation results If you
// want to avoid rounding - set PointScaleFactor to 0
WIN_EXPORT void FBYGConfigSetPointScaleFactor(
    FBYGConfigRef config,
    float pixelsInPoint);
WIN_EXPORT float FBYGConfigGetPointScaleFactor(FBYGConfigRef config);

// Yoga previously had an error where containers would take the maximum space
// possible instead of the minimum like they are supposed to. In practice this
// resulted in implicit behaviour similar to align-self: stretch; Because this
// was such a long-standing bug we must allow legacy users to switch back to
// this behaviour.
WIN_EXPORT FBYG_DEPRECATED(
    "Please use "
    "\"FBYGConfigGetErrata()\"") bool FBYGConfigGetUseLegacyStretchBehaviour(FBYGConfigRef
                                                                             config);
WIN_EXPORT
FBYG_DEPRECATED(
    "\"FBYGConfigSetUseLegacyStretchBehaviour\" will be removed in the next "
    "release. Usage should be replaced with \"FBYGConfigSetErrata(FBYGErrataAll)\" "
    "to opt out of all future breaking conformance fixes, or "
    "\"FBYGConfigSetErrata(FBYGErrataStretchFlexBasis)\" to opt out of the "
    "specific conformance fix previously disabled by "
    "\"UseLegacyStretchBehaviour\".")
void FBYGConfigSetUseLegacyStretchBehaviour(
    FBYGConfigRef config,
    bool useLegacyStretchBehaviour);

// FBYGConfig
WIN_EXPORT FBYGConfigRef FBYGConfigNew(void);
WIN_EXPORT void FBYGConfigFree(FBYGConfigRef config);
WIN_EXPORT void FBYGConfigCopy(FBYGConfigRef dest, FBYGConfigRef src);
WIN_EXPORT int32_t FBYGConfigGetInstanceCount(void);

WIN_EXPORT void FBYGConfigSetExperimentalFeatureEnabled(
    FBYGConfigRef config,
    FBYGExperimentalFeature feature,
    bool enabled);
WIN_EXPORT bool FBYGConfigIsExperimentalFeatureEnabled(
    FBYGConfigRef config,
    FBYGExperimentalFeature feature);

// Using the web defaults is the preferred configuration for new projects. Usage
// of non web defaults should be considered as legacy.
WIN_EXPORT void FBYGConfigSetUseWebDefaults(FBYGConfigRef config, bool enabled);
WIN_EXPORT bool FBYGConfigGetUseWebDefaults(FBYGConfigRef config);

WIN_EXPORT void FBYGConfigSetCloneNodeFunc(
    FBYGConfigRef config,
    FBYGCloneNodeFunc callback);

// Export only for C#
WIN_EXPORT FBYGConfigRef FBYGConfigGetDefault(void);

WIN_EXPORT void FBYGConfigSetContext(FBYGConfigRef config, void* context);
WIN_EXPORT void* FBYGConfigGetContext(FBYGConfigRef config);

WIN_EXPORT void FBYGConfigSetErrata(FBYGConfigRef config, FBYGErrata errata);
WIN_EXPORT FBYGErrata FBYGConfigGetErrata(FBYGConfigRef config);

WIN_EXPORT float FBYGRoundValueToPixelGrid(
    double value,
    double pointScaleFactor,
    bool forceCeil,
    bool forceFloor);

FBYG_EXTERN_C_END
