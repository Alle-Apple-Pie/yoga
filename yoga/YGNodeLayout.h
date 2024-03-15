/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdbool.h>

#include <yoga/YGConfig.h>
#include <yoga/YGEnums.h>
#include <yoga/YGMacros.h>

FBYG_EXTERN_C_BEGIN

FBYG_EXPORT float FBYGNodeLayoutGetLeft(FBYGNodeConstRef node);
FBYG_EXPORT float FBYGNodeLayoutGetTop(FBYGNodeConstRef node);
FBYG_EXPORT float FBYGNodeLayoutGetRight(FBYGNodeConstRef node);
FBYG_EXPORT float FBYGNodeLayoutGetBottom(FBYGNodeConstRef node);
FBYG_EXPORT float FBYGNodeLayoutGetWidth(FBYGNodeConstRef node);
FBYG_EXPORT float FBYGNodeLayoutGetHeight(FBYGNodeConstRef node);
FBYG_EXPORT FBYGDirection FBYGNodeLayoutGetDirection(FBYGNodeConstRef node);
FBYG_EXPORT bool FBYGNodeLayoutGetHadOverflow(FBYGNodeConstRef node);

// Get the computed values for these nodes after performing layout. If they were
// set using point values then the returned value will be the same as
// FBYGNodeStyleGetXXX. However if they were set using a percentage value then
// the returned value is the computed value used during layout.
FBYG_EXPORT float FBYGNodeLayoutGetMargin(FBYGNodeConstRef node, FBYGEdge edge);
FBYG_EXPORT float FBYGNodeLayoutGetBorder(FBYGNodeConstRef node, FBYGEdge edge);
FBYG_EXPORT float FBYGNodeLayoutGetPadding(
    FBYGNodeConstRef node,
    FBYGEdge edge);

FBYG_EXTERN_C_END
