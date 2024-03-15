/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>

#include <yoga/YGConfig.h>
#include <yoga/YGEnums.h>
#include <yoga/YGMacros.h>

FBYG_EXTERN_C_BEGIN

/**
 * Handle to a mutable Yoga Node.
 */
typedef struct FBYGNode* FBYGNodeRef;

/**
 * Handle to an immutable Yoga Node.
 */
typedef const struct FBYGNode* FBYGNodeConstRef;

/**
 * Heap allocates and returns a new Yoga node using Yoga settings.
 */
FBYG_EXPORT FBYGNodeRef FBYGNodeNew(void);

/**
 * Heap allocates and returns a new Yoga node, with customized settings.
 */
FBYG_EXPORT FBYGNodeRef FBYGNodeNewWithConfig(FBYGConfigConstRef config);

/**
 * Returns a mutable copy of an existing node, with the same context and
 * children, but no owner set. Does not call the function set by
 * FBYGConfigSetCloneNodeFunc().
 */
FBYG_EXPORT FBYGNodeRef FBYGNodeClone(FBYGNodeConstRef node);

/**
 * Frees the Yoga node, disconnecting it from its owner and children.
 */
FBYG_EXPORT void FBYGNodeFree(FBYGNodeRef node);

/**
 * Frees the subtree of Yoga nodes rooted at the given node.
 */
FBYG_EXPORT void FBYGNodeFreeRecursive(FBYGNodeRef node);

/**
 * Frees the Yoga node without disconnecting it from its owner or children.
 * Allows garbage collecting Yoga nodes in parallel when the entire tree is
 * unrechable.
 */
FBYG_EXPORT void FBYGNodeFinalize(FBYGNodeRef node);

/**
 * Resets the node to its default state.
 */
FBYG_EXPORT void FBYGNodeReset(FBYGNodeRef node);

/**
 * Calculates the layout of the tree rooted at the given node.
 *
 * Layout results may be read after calling FBYGNodeCalculateLayout() using
 * functions like FBYGNodeLayoutGetLeft(), FBYGNodeLayoutGetTop(), etc.
 *
 * FBYGNodeGetHasNewLayout() may be read to know if the layout of the node or
 * its subtrees may have changed since the last time FBYGNodeCalculate() was
 * called.
 */
FBYG_EXPORT void FBYGNodeCalculateLayout(
    FBYGNodeRef node,
    float availableWidth,
    float availableHeight,
    FBYGDirection ownerDirection);

/**
 * Whether the given node may have new layout results. Must be reset by calling
 * FBYGNodeSetHasNewLayout().
 */
FBYG_EXPORT bool FBYGNodeGetHasNewLayout(FBYGNodeConstRef node);

/**
 * Sets whether a nodes layout is considered new.
 */
FBYG_EXPORT void FBYGNodeSetHasNewLayout(FBYGNodeRef node, bool hasNewLayout);

/**
 * Whether the node's layout results are dirty due to it or its children
 * changing.
 */
FBYG_EXPORT bool FBYGNodeIsDirty(FBYGNodeConstRef node);

/**
 * Marks a node with custom measure function as dirty.
 */
FBYG_EXPORT void FBYGNodeMarkDirty(FBYGNodeRef node);

typedef void (*FBYGDirtiedFunc)(FBYGNodeConstRef node);

/**
 * Called when a change is made to the Yoga tree which dirties this node.
 */
FBYG_EXPORT void FBYGNodeSetDirtiedFunc(
    FBYGNodeRef node,
    FBYGDirtiedFunc dirtiedFunc);

/**
 * Returns a dirtied func if set.
 */
FBYG_EXPORT FBYGDirtiedFunc FBYGNodeGetDirtiedFunc(FBYGNodeConstRef node);

/**
 * Inserts a child node at the given index.
 */
FBYG_EXPORT void
FBYGNodeInsertChild(FBYGNodeRef node, FBYGNodeRef child, size_t index);

/**
 * Replaces the child node at a given index with a new one.
 */
FBYG_EXPORT void
FBYGNodeSwapChild(FBYGNodeRef node, FBYGNodeRef child, size_t index);

/**
 * Removes the given child node.
 */
FBYG_EXPORT void FBYGNodeRemoveChild(FBYGNodeRef node, FBYGNodeRef child);

/**
 * Removes all children nodes.
 */
FBYG_EXPORT void FBYGNodeRemoveAllChildren(FBYGNodeRef node);

/**
 * Sets children according to the given list of nodes.
 */
FBYG_EXPORT void FBYGNodeSetChildren(
    FBYGNodeRef owner,
    const FBYGNodeRef* children,
    size_t count);

/**
 * Get the child node at a given index.
 */
FBYG_EXPORT FBYGNodeRef FBYGNodeGetChild(FBYGNodeRef node, size_t index);

/**
 * The number of child nodes.
 */
FBYG_EXPORT size_t FBYGNodeGetChildCount(FBYGNodeConstRef node);

/**
 * Get the parent/owner currently set for a node.
 */
FBYG_EXPORT FBYGNodeRef FBYGNodeGetOwner(FBYGNodeRef node);

/**
 * Get the parent/owner currently set for a node.
 */
FBYG_EXPORT FBYGNodeRef FBYGNodeGetParent(FBYGNodeRef node);

/**
 * Set a new config for the node after creation.
 */
FBYG_EXPORT void FBYGNodeSetConfig(FBYGNodeRef node, FBYGConfigRef config);

/**
 * Get the config currently set on the node.
 */
FBYG_EXPORT FBYGConfigConstRef FBYGNodeGetConfig(FBYGNodeRef node);

/**
 * Sets extra data on the Yoga node which may be read from during callbacks.
 */
FBYG_EXPORT void FBYGNodeSetContext(FBYGNodeRef node, void* context);

/**
 * Returns the context or NULL if no context has been set.
 */
FBYG_EXPORT void* FBYGNodeGetContext(FBYGNodeConstRef node);

typedef struct FBYGSize {
  float width;
  float height;
} FBYGSize;

/**
 * Returns the computed dimensions of the node, following the contraints of
 * `widthMode` and `heightMode`:
 *
 * FBYGMeasureModeUndefined: The parent has not imposed any constraint on the
 * child. It can be whatever size it wants.
 *
 * FBYGMeasureModeAtMost: The child can be as large as it wants up to the
 * specified size.
 *
 * FBYGMeasureModeExactly: The parent has determined an exact size for the
 * child. The child is going to be given those bounds regardless of how big it
 * wants to be.
 *
 * @returns the size of the leaf node, measured under the given contraints.
 */
typedef FBYGSize (*FBYGMeasureFunc)(
    FBYGNodeConstRef node,
    float width,
    FBYGMeasureMode widthMode,
    float height,
    FBYGMeasureMode heightMode);

/**
 * Allows providing custom measurements for a Yoga leaf node (usually for
 * measuring text). FBYGNodeMarkDirty() must be set if content effecting the
 * measurements of the node changes.
 */
FBYG_EXPORT void FBYGNodeSetMeasureFunc(
    FBYGNodeRef node,
    FBYGMeasureFunc measureFunc);

/**
 * Whether a measure function is set.
 */
FBYG_EXPORT bool FBYGNodeHasMeasureFunc(FBYGNodeConstRef node);

/**
 * @returns a defined offet to baseline (ascent).
 */
typedef float (
    *FBYGBaselineFunc)(FBYGNodeConstRef node, float width, float height);

/**
 * Set a custom function for determining the text baseline for use in baseline
 * alignment.
 */
FBYG_EXPORT void FBYGNodeSetBaselineFunc(
    FBYGNodeRef node,
    FBYGBaselineFunc baselineFunc);

/**
 * Whether a baseline function is set.
 */
FBYG_EXPORT bool FBYGNodeHasBaselineFunc(FBYGNodeConstRef node);

/**
 * Sets this node should be considered the reference baseline among siblings.
 */
FBYG_EXPORT void FBYGNodeSetIsReferenceBaseline(
    FBYGNodeRef node,
    bool isReferenceBaseline);

/**
 * Whether this node is set as the reference baseline.
 */
FBYG_EXPORT bool FBYGNodeIsReferenceBaseline(FBYGNodeConstRef node);

/**
 * Sets whether a leaf node's layout results may be truncated during layout
 * rounding.
 */
FBYG_EXPORT void FBYGNodeSetNodeType(FBYGNodeRef node, FBYGNodeType nodeType);

/**
 * Wwhether a leaf node's layout results may be truncated during layout
 * rounding.
 */
FBYG_EXPORT FBYGNodeType FBYGNodeGetNodeType(FBYGNodeConstRef node);

/**
 * Make it so that this node will always form a containing block for any
 * descendant nodes. This is useful for when a node has a property outside of
 * of Yoga that will form a containing block. For example, transforms or some of
 * the others listed in
 * https://developer.mozilla.org/en-US/docs/Web/CSS/Containing_block
 */
FBYG_EXPORT void FBYGNodeSetAlwaysFormsContainingBlock(
    FBYGNodeRef node,
    bool alwaysFormsContainingBlock);

/**
 * Whether the node will always form a containing block for any descendant. This
 * can happen in situation where the client implements something like a
 * transform that can affect containing blocks but is not handled by Yoga
 * directly.
 */
FBYG_EXPORT bool FBYGNodeGetAlwaysFormsContainingBlock(FBYGNodeConstRef node);

/**
 * @deprecated
 */
FBYG_DEPRECATED(
    "FBYGNodeCanUseCachedMeasurement may be removed in a future version of Yoga")
FBYG_EXPORT bool FBYGNodeCanUseCachedMeasurement(
    FBYGMeasureMode widthMode,
    float availableWidth,
    FBYGMeasureMode heightMode,
    float availableHeight,
    FBYGMeasureMode lastWidthMode,
    float lastAvailableWidth,
    FBYGMeasureMode lastHeightMode,
    float lastAvailableHeight,
    float lastComputedWidth,
    float lastComputedHeight,
    float marginRow,
    float marginColumn,
    FBYGConfigRef config);

FBYG_EXTERN_C_END
