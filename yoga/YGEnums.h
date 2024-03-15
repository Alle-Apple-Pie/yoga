/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// @generated by enums.py
// clang-format off
#pragma once
#include <yoga/YGMacros.h>

FBYG_EXTERN_C_BEGIN

FBYG_ENUM_DECL(
    FBYGAlign,
    FBYGAlignAuto,
    FBYGAlignFlexStart,
    FBYGAlignCenter,
    FBYGAlignFlexEnd,
    FBYGAlignStretch,
    FBYGAlignBaseline,
    FBYGAlignSpaceBetween,
    FBYGAlignSpaceAround,
    FBYGAlignSpaceEvenly)

FBYG_ENUM_DECL(
    FBYGDimension,
    FBYGDimensionWidth,
    FBYGDimensionHeight)

FBYG_ENUM_DECL(
    FBYGDirection,
    FBYGDirectionInherit,
    FBYGDirectionLTR,
    FBYGDirectionRTL)

FBYG_ENUM_DECL(
    FBYGDisplay,
    FBYGDisplayFlex,
    FBYGDisplayNone)

FBYG_ENUM_DECL(
    FBYGEdge,
    FBYGEdgeLeft,
    FBYGEdgeTop,
    FBYGEdgeRight,
    FBYGEdgeBottom,
    FBYGEdgeStart,
    FBYGEdgeEnd,
    FBYGEdgeHorizontal,
    FBYGEdgeVertical,
    FBYGEdgeAll)

FBYG_ENUM_DECL(
    FBYGErrata,
    FBYGErrataNone = 0,
    FBYGErrataStretchFlexBasis = 1,
    FBYGErrataAbsolutePositioningIncorrect = 2,
    FBYGErrataAbsolutePercentAgainstInnerSize = 4,
    FBYGErrataAll = 2147483647,
    FBYGErrataClassic = 2147483646)
FBYG_DEFINE_ENUM_FLAG_OPERATORS(FBYGErrata)

FBYG_ENUM_DECL(
    FBYGExperimentalFeature,
    FBYGExperimentalFeatureWebFlexBasis)

FBYG_ENUM_DECL(
    FBYGFlexDirection,
    FBYGFlexDirectionColumn,
    FBYGFlexDirectionColumnReverse,
    FBYGFlexDirectionRow,
    FBYGFlexDirectionRowReverse)

FBYG_ENUM_DECL(
    FBYGGutter,
    FBYGGutterColumn,
    FBYGGutterRow,
    FBYGGutterAll)

FBYG_ENUM_DECL(
    FBYGJustify,
    FBYGJustifyFlexStart,
    FBYGJustifyCenter,
    FBYGJustifyFlexEnd,
    FBYGJustifySpaceBetween,
    FBYGJustifySpaceAround,
    FBYGJustifySpaceEvenly)

FBYG_ENUM_DECL(
    FBYGLogLevel,
    FBYGLogLevelError,
    FBYGLogLevelWarn,
    FBYGLogLevelInfo,
    FBYGLogLevelDebug,
    FBYGLogLevelVerbose,
    FBYGLogLevelFatal)

FBYG_ENUM_DECL(
    FBYGMeasureMode,
    FBYGMeasureModeUndefined,
    FBYGMeasureModeExactly,
    FBYGMeasureModeAtMost)

FBYG_ENUM_DECL(
    FBYGNodeType,
    FBYGNodeTypeDefault,
    FBYGNodeTypeText)

FBYG_ENUM_DECL(
    FBYGOverflow,
    FBYGOverflowVisible,
    FBYGOverflowHidden,
    FBYGOverflowScroll)

FBYG_ENUM_DECL(
    FBYGPositionType,
    FBYGPositionTypeStatic,
    FBYGPositionTypeRelative,
    FBYGPositionTypeAbsolute)

FBYG_ENUM_DECL(
    FBYGUnit,
    FBYGUnitUndefined,
    FBYGUnitPoint,
    FBYGUnitPercent,
    FBYGUnitAuto)

FBYG_ENUM_DECL(
    FBYGWrap,
    FBYGWrapNoWrap,
    FBYGWrapWrap,
    FBYGWrapWrapReverse)

FBYG_EXTERN_C_END
