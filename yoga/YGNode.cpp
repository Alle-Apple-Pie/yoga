/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>

#include <yoga/algorithm/Cache.h>
#include <yoga/algorithm/CalculateLayout.h>
#include <yoga/debug/AssertFatal.h>
#include <yoga/debug/Log.h>
#include <yoga/event/event.h>
#include <yoga/node/Node.h>

using namespace facebookyg;
using namespace facebookyg::yoga;

FBYGNodeRef FBYGNodeNew(void) {
  return FBYGNodeNewWithConfig(FBYGConfigGetDefault());
}

FBYGNodeRef FBYGNodeNewWithConfig(const FBYGConfigConstRef config) {
  auto* node = new yoga::Node{resolveRef(config)};
  yoga::assertFatal(
      config != nullptr, "Tried to construct FBYGNode with null config");
  Event::publish<Event::NodeAllocation>(node, {config});

  return node;
}

FBYGNodeRef FBYGNodeClone(FBYGNodeConstRef oldNodeRef) {
  auto oldNode = resolveRef(oldNodeRef);
  const auto node = new yoga::Node(*oldNode);
  Event::publish<Event::NodeAllocation>(node, {node->getConfig()});
  node->setOwner(nullptr);
  return node;
}

void FBYGNodeFree(const FBYGNodeRef nodeRef) {
  const auto node = resolveRef(nodeRef);

  if (auto owner = node->getOwner()) {
    owner->removeChild(node);
    node->setOwner(nullptr);
  }

  const size_t childCount = node->getChildCount();
  for (size_t i = 0; i < childCount; i++) {
    auto child = node->getChild(i);
    child->setOwner(nullptr);
  }

  node->clearChildren();

  Event::publish<Event::NodeDeallocation>(node, {FBYGNodeGetConfig(node)});
  delete resolveRef(node);
}

void FBYGNodeFreeRecursive(FBYGNodeRef rootRef) {
  const auto root = resolveRef(rootRef);

  size_t skipped = 0;
  while (root->getChildCount() > skipped) {
    const auto child = root->getChild(skipped);
    if (child->getOwner() != root) {
      // Don't free shared nodes that we don't own.
      skipped += 1;
    } else {
      FBYGNodeRemoveChild(root, child);
      FBYGNodeFreeRecursive(child);
    }
  }
  FBYGNodeFree(root);
}

void FBYGNodeFinalize(const FBYGNodeRef node) {
  Event::publish<Event::NodeDeallocation>(node, {FBYGNodeGetConfig(node)});
  delete resolveRef(node);
}

void FBYGNodeReset(FBYGNodeRef node) {
  resolveRef(node)->reset();
}

void FBYGNodeCalculateLayout(
    const FBYGNodeRef node,
    const float ownerWidth,
    const float ownerHeight,
    const FBYGDirection ownerDirection) {
  yoga::calculateLayout(
      resolveRef(node), ownerWidth, ownerHeight, scopedEnum(ownerDirection));
}

bool FBYGNodeGetHasNewLayout(FBYGNodeConstRef node) {
  return resolveRef(node)->getHasNewLayout();
}

void FBYGNodeSetHasNewLayout(FBYGNodeRef node, bool hasNewLayout) {
  resolveRef(node)->setHasNewLayout(hasNewLayout);
}

bool FBYGNodeIsDirty(FBYGNodeConstRef node) {
  return resolveRef(node)->isDirty();
}

void FBYGNodeMarkDirty(const FBYGNodeRef nodeRef) {
  const auto node = resolveRef(nodeRef);

  yoga::assertFatalWithNode(
      node,
      node->hasMeasureFunc(),
      "Only leaf nodes with custom measure functions "
      "should manually mark themselves as dirty");

  node->markDirtyAndPropagate();
}

void FBYGNodeSetDirtiedFunc(FBYGNodeRef node, FBYGDirtiedFunc dirtiedFunc) {
  resolveRef(node)->setDirtiedFunc(dirtiedFunc);
}

FBYGDirtiedFunc FBYGNodeGetDirtiedFunc(FBYGNodeConstRef node) {
  return resolveRef(node)->getDirtiedFunc();
}

void FBYGNodeInsertChild(
    const FBYGNodeRef ownerRef,
    const FBYGNodeRef childRef,
    const size_t index) {
  auto owner = resolveRef(ownerRef);
  auto child = resolveRef(childRef);

  yoga::assertFatalWithNode(
      owner,
      child->getOwner() == nullptr,
      "Child already has a owner, it must be removed first.");

  yoga::assertFatalWithNode(
      owner,
      !owner->hasMeasureFunc(),
      "Cannot add child: Nodes with measure functions cannot have children.");

  owner->insertChild(child, index);
  child->setOwner(owner);
  owner->markDirtyAndPropagate();
}

void FBYGNodeSwapChild(
    const FBYGNodeRef ownerRef,
    const FBYGNodeRef childRef,
    const size_t index) {
  auto owner = resolveRef(ownerRef);
  auto child = resolveRef(childRef);

  owner->replaceChild(child, index);
  child->setOwner(owner);
}

void FBYGNodeRemoveChild(
    const FBYGNodeRef ownerRef,
    const FBYGNodeRef excludedChildRef) {
  auto owner = resolveRef(ownerRef);
  auto excludedChild = resolveRef(excludedChildRef);

  if (owner->getChildCount() == 0) {
    // This is an empty set. Nothing to remove.
    return;
  }

  // Children may be shared between parents, which is indicated by not having an
  // owner. We only want to reset the child completely if it is owned
  // exclusively by one node.
  auto childOwner = excludedChild->getOwner();
  if (owner->removeChild(excludedChild)) {
    if (owner == childOwner) {
      excludedChild->setLayout({}); // layout is no longer valid
      excludedChild->setOwner(nullptr);
    }
    owner->markDirtyAndPropagate();
  }
}

void FBYGNodeRemoveAllChildren(const FBYGNodeRef ownerRef) {
  auto owner = resolveRef(ownerRef);

  const size_t childCount = owner->getChildCount();
  if (childCount == 0) {
    // This is an empty set already. Nothing to do.
    return;
  }
  auto* firstChild = owner->getChild(0);
  if (firstChild->getOwner() == owner) {
    // If the first child has this node as its owner, we assume that this child
    // set is unique.
    for (size_t i = 0; i < childCount; i++) {
      yoga::Node* oldChild = owner->getChild(i);
      oldChild->setLayout({}); // layout is no longer valid
      oldChild->setOwner(nullptr);
    }
    owner->clearChildren();
    owner->markDirtyAndPropagate();
    return;
  }
  // Otherwise, we are not the owner of the child set. We don't have to do
  // anything to clear it.
  owner->setChildren({});
  owner->markDirtyAndPropagate();
}

void FBYGNodeSetChildren(
    const FBYGNodeRef ownerRef,
    const FBYGNodeRef* childrenRefs,
    const size_t count) {
  auto owner = resolveRef(ownerRef);
  auto children = reinterpret_cast<yoga::Node* const*>(childrenRefs);

  if (!owner) {
    return;
  }

  const std::vector<yoga::Node*> childrenVector = {children, children + count};
  if (childrenVector.size() == 0) {
    if (owner->getChildCount() > 0) {
      for (auto* child : owner->getChildren()) {
        child->setLayout({});
        child->setOwner(nullptr);
      }
      owner->setChildren({});
      owner->markDirtyAndPropagate();
    }
  } else {
    if (owner->getChildCount() > 0) {
      for (auto* oldChild : owner->getChildren()) {
        // Our new children may have nodes in common with the old children. We
        // don't reset these common nodes.
        if (std::find(childrenVector.begin(), childrenVector.end(), oldChild) ==
            childrenVector.end()) {
          oldChild->setLayout({});
          oldChild->setOwner(nullptr);
        }
      }
    }
    owner->setChildren(childrenVector);
    for (yoga::Node* child : childrenVector) {
      child->setOwner(owner);
    }
    owner->markDirtyAndPropagate();
  }
}

FBYGNodeRef FBYGNodeGetChild(const FBYGNodeRef nodeRef, const size_t index) {
  const auto node = resolveRef(nodeRef);

  if (index < node->getChildren().size()) {
    return node->getChild(index);
  }
  return nullptr;
}

size_t FBYGNodeGetChildCount(const FBYGNodeConstRef node) {
  return resolveRef(node)->getChildren().size();
}

FBYGNodeRef FBYGNodeGetOwner(const FBYGNodeRef node) {
  return resolveRef(node)->getOwner();
}

FBYGNodeRef FBYGNodeGetParent(const FBYGNodeRef node) {
  return resolveRef(node)->getOwner();
}

void FBYGNodeSetConfig(FBYGNodeRef node, FBYGConfigRef config) {
  resolveRef(node)->setConfig(resolveRef(config));
}

FBYGConfigConstRef FBYGNodeGetConfig(FBYGNodeRef node) {
  return resolveRef(node)->getConfig();
}

void FBYGNodeSetContext(FBYGNodeRef node, void* context) {
  return resolveRef(node)->setContext(context);
}

void* FBYGNodeGetContext(FBYGNodeConstRef node) {
  return resolveRef(node)->getContext();
}

void FBYGNodeSetMeasureFunc(FBYGNodeRef node, FBYGMeasureFunc measureFunc) {
  resolveRef(node)->setMeasureFunc(measureFunc);
}

bool FBYGNodeHasMeasureFunc(FBYGNodeConstRef node) {
  return resolveRef(node)->hasMeasureFunc();
}

void FBYGNodeSetBaselineFunc(FBYGNodeRef node, FBYGBaselineFunc baselineFunc) {
  resolveRef(node)->setBaselineFunc(baselineFunc);
}

bool FBYGNodeHasBaselineFunc(FBYGNodeConstRef node) {
  return resolveRef(node)->hasBaselineFunc();
}

void FBYGNodeSetIsReferenceBaseline(
    FBYGNodeRef nodeRef,
    bool isReferenceBaseline) {
  const auto node = resolveRef(nodeRef);
  if (node->isReferenceBaseline() != isReferenceBaseline) {
    node->setIsReferenceBaseline(isReferenceBaseline);
    node->markDirtyAndPropagate();
  }
}

bool FBYGNodeIsReferenceBaseline(FBYGNodeConstRef node) {
  return resolveRef(node)->isReferenceBaseline();
}

void FBYGNodeSetNodeType(FBYGNodeRef node, FBYGNodeType nodeType) {
  return resolveRef(node)->setNodeType(scopedEnum(nodeType));
}

FBYGNodeType FBYGNodeGetNodeType(FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->getNodeType());
}

void FBYGNodeSetAlwaysFormsContainingBlock(
    FBYGNodeRef node,
    bool alwaysFormsContainingBlock) {
  resolveRef(node)->setAlwaysFormsContainingBlock(alwaysFormsContainingBlock);
}

bool FBYGNodeGetAlwaysFormsContainingBlock(FBYGNodeConstRef node) {
  return resolveRef(node)->alwaysFormsContainingBlock();
}

// TODO: This leaks internal details to the public API. Remove after removing
// ComponentKit usage of it.
bool FBYGNodeCanUseCachedMeasurement(
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
    FBYGConfigRef config) {
  return yoga::canUseCachedMeasurement(
      sizingMode(scopedEnum(widthMode)),
      availableWidth,
      sizingMode(scopedEnum(heightMode)),
      availableHeight,
      sizingMode(scopedEnum(lastWidthMode)),
      lastAvailableWidth,
      sizingMode(scopedEnum(lastHeightMode)),
      lastAvailableHeight,
      lastComputedWidth,
      lastComputedHeight,
      marginRow,
      marginColumn,
      resolveRef(config));
}
