/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <float.h>
#include <string.h>
#include <algorithm>
#include <atomic>
#include <memory>

#include <yoga/Yoga.h>

#include "log.h"
#include "Utils.h"
#include "YGNode.h"
#include "YGNodePrint.h"
#include "Yoga-internal.h"
#include "event/event.h"

using namespace facebook::yoga;
using detail::Log;

#ifdef ANDROID
static int FBYGAndroidLog(
    const FBYGConfigRef config,
    const FBYGNodeRef node,
    FBYGLogLevel level,
    const char* format,
    va_list args);
#else
static int FBYGDefaultLog(
    const FBYGConfigRef config,
    const FBYGNodeRef node,
    FBYGLogLevel level,
    const char* format,
    va_list args);
#endif

#ifdef ANDROID
#include <android/log.h>
static int FBYGAndroidLog(
    const FBYGConfigRef /*config*/,
    const FBYGNodeRef /*node*/,
    FBYGLogLevel level,
    const char* format,
    va_list args) {
  int androidLevel = FBYGLogLevelDebug;
  switch (level) {
    case FBYGLogLevelFatal:
      androidLevel = ANDROID_LOG_FATAL;
      break;
    case FBYGLogLevelError:
      androidLevel = ANDROID_LOG_ERROR;
      break;
    case FBYGLogLevelWarn:
      androidLevel = ANDROID_LOG_WARN;
      break;
    case FBYGLogLevelInfo:
      androidLevel = ANDROID_LOG_INFO;
      break;
    case FBYGLogLevelDebug:
      androidLevel = ANDROID_LOG_DEBUG;
      break;
    case FBYGLogLevelVerbose:
      androidLevel = ANDROID_LOG_VERBOSE;
      break;
  }
  const int result = __android_log_vprint(androidLevel, "yoga", format, args);
  return result;
}
#else
#define FBYG_UNUSED(x) (void) (x);

static int FBYGDefaultLog(
    const FBYGConfigRef config,
    const FBYGNodeRef node,
    FBYGLogLevel level,
    const char* format,
    va_list args) {
  FBYG_UNUSED(config);
  FBYG_UNUSED(node);
  switch (level) {
    case FBYGLogLevelError:
    case FBYGLogLevelFatal:
      return vfprintf(stderr, format, args);
    case FBYGLogLevelWarn:
    case FBYGLogLevelInfo:
    case FBYGLogLevelDebug:
    case FBYGLogLevelVerbose:
    default:
      return vprintf(format, args);
  }
}

#undef FBYG_UNUSED
#endif

static inline bool FBYGDoubleIsUndefined(const double value) {
  return facebook::yoga::isUndefined(value);
}

YOGA_EXPORT bool FBYGFloatIsUndefined(const float value) {
  return facebook::yoga::isUndefined(value);
}

YOGA_EXPORT void* FBYGNodeGetContext(FBYGNodeRef node) {
  return node->getContext();
}

YOGA_EXPORT void FBYGNodeSetContext(FBYGNodeRef node, void* context) {
  return node->setContext(context);
}

YOGA_EXPORT FBYGConfigRef FBYGNodeGetConfig(FBYGNodeRef node) {
  return node->getConfig();
}

YOGA_EXPORT void FBYGNodeSetConfig(FBYGNodeRef node, FBYGConfigRef config) {
  node->setConfig(config);
}

YOGA_EXPORT bool FBYGNodeHasMeasureFunc(FBYGNodeRef node) {
  return node->hasMeasureFunc();
}

YOGA_EXPORT void FBYGNodeSetMeasureFunc(
    FBYGNodeRef node,
    FBYGMeasureFunc measureFunc) {
  node->setMeasureFunc(measureFunc);
}

YOGA_EXPORT bool FBYGNodeHasBaselineFunc(FBYGNodeRef node) {
  return node->hasBaselineFunc();
}

YOGA_EXPORT void FBYGNodeSetBaselineFunc(
    FBYGNodeRef node,
    FBYGBaselineFunc baselineFunc) {
  node->setBaselineFunc(baselineFunc);
}

YOGA_EXPORT FBYGDirtiedFunc FBYGNodeGetDirtiedFunc(FBYGNodeRef node) {
  return node->getDirtied();
}

YOGA_EXPORT void FBYGNodeSetDirtiedFunc(
    FBYGNodeRef node,
    FBYGDirtiedFunc dirtiedFunc) {
  node->setDirtiedFunc(dirtiedFunc);
}

YOGA_EXPORT void FBYGNodeSetPrintFunc(FBYGNodeRef node, FBYGPrintFunc printFunc) {
  node->setPrintFunc(printFunc);
}

YOGA_EXPORT bool FBYGNodeGetHasNewLayout(FBYGNodeRef node) {
  return node->getHasNewLayout();
}

YOGA_EXPORT void FBYGConfigSetPrintTreeFlag(FBYGConfigRef config, bool enabled) {
  config->setShouldPrintTree(enabled);
}

YOGA_EXPORT void FBYGNodeSetHasNewLayout(FBYGNodeRef node, bool hasNewLayout) {
  node->setHasNewLayout(hasNewLayout);
}

YOGA_EXPORT FBYGNodeType FBYGNodeGetNodeType(FBYGNodeRef node) {
  return node->getNodeType();
}

YOGA_EXPORT void FBYGNodeSetNodeType(FBYGNodeRef node, FBYGNodeType nodeType) {
  return node->setNodeType(nodeType);
}

YOGA_EXPORT bool FBYGNodeIsDirty(FBYGNodeRef node) {
  return node->isDirty();
}

YOGA_EXPORT void FBYGNodeMarkDirtyAndPropagateToDescendants(
    const FBYGNodeRef node) {
  return node->markDirtyAndPropagateDownwards();
}

int32_t gConfigInstanceCount = 0;

YOGA_EXPORT WIN_EXPORT FBYGNodeRef FBYGNodeNewWithConfig(const FBYGConfigRef config) {
  const FBYGNodeRef node = new FBYGNode{config};
  FBYGAssert(config != nullptr, "Tried to construct FBYGNode with null config");
  FBYGAssertWithConfig(
      config, node != nullptr, "Could not allocate memory for node");
  Event::publish<Event::NodeAllocation>(node, {config});

  return node;
}

YOGA_EXPORT FBYGConfigRef FBYGConfigGetDefault() {
  static FBYGConfigRef defaultConfig = FBYGConfigNew();
  return defaultConfig;
}

YOGA_EXPORT FBYGNodeRef FBYGNodeNew(void) {
  return FBYGNodeNewWithConfig(FBYGConfigGetDefault());
}

YOGA_EXPORT FBYGNodeRef FBYGNodeClone(FBYGNodeRef oldNode) {
  FBYGNodeRef node = new FBYGNode(*oldNode);
  FBYGAssertWithConfig(
      oldNode->getConfig(),
      node != nullptr,
      "Could not allocate memory for node");
  Event::publish<Event::NodeAllocation>(node, {node->getConfig()});
  node->setOwner(nullptr);
  return node;
}

YOGA_EXPORT void FBYGNodeFree(const FBYGNodeRef node) {
  if (FBYGNodeRef owner = node->getOwner()) {
    owner->removeChild(node);
    node->setOwner(nullptr);
  }

  const uint32_t childCount = FBYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    const FBYGNodeRef child = FBYGNodeGetChild(node, i);
    child->setOwner(nullptr);
  }

  node->clearChildren();
  FBYGNodeDeallocate(node);
}

YOGA_EXPORT void FBYGNodeDeallocate(const FBYGNodeRef node) {
  Event::publish<Event::NodeDeallocation>(node, {node->getConfig()});
  delete node;
}

YOGA_EXPORT void FBYGNodeFreeRecursiveWithCleanupFunc(
    const FBYGNodeRef root,
    FBYGNodeCleanupFunc cleanup) {
  uint32_t skipped = 0;
  while (FBYGNodeGetChildCount(root) > skipped) {
    const FBYGNodeRef child = FBYGNodeGetChild(root, skipped);
    if (child->getOwner() != root) {
      // Don't free shared nodes that we don't own.
      skipped += 1;
    } else {
      FBYGNodeRemoveChild(root, child);
      FBYGNodeFreeRecursive(child);
    }
  }
  if (cleanup != nullptr) {
    cleanup(root);
  }
  FBYGNodeFree(root);
}

YOGA_EXPORT void FBYGNodeFreeRecursive(const FBYGNodeRef root) {
  return FBYGNodeFreeRecursiveWithCleanupFunc(root, nullptr);
}

YOGA_EXPORT void FBYGNodeReset(FBYGNodeRef node) {
  node->reset();
}

YOGA_EXPORT int32_t FBYGConfigGetInstanceCount(void) {
  return gConfigInstanceCount;
}

YOGA_EXPORT FBYGConfigRef FBYGConfigNew(void) {
#ifdef ANDROID
  const FBYGConfigRef config = new FBYGConfig(FBYGAndroidLog);
#else
  const FBYGConfigRef config = new FBYGConfig(FBYGDefaultLog);
#endif
  gConfigInstanceCount++;
  return config;
}

YOGA_EXPORT void FBYGConfigFree(const FBYGConfigRef config) {
  delete config;
  gConfigInstanceCount--;
}

void FBYGConfigCopy(const FBYGConfigRef dest, const FBYGConfigRef src) {
  memcpy(dest, src, sizeof(FBYGConfig));
}

YOGA_EXPORT void FBYGNodeSetIsReferenceBaseline(
    FBYGNodeRef node,
    bool isReferenceBaseline) {
  if (node->isReferenceBaseline() != isReferenceBaseline) {
    node->setIsReferenceBaseline(isReferenceBaseline);
    node->markDirtyAndPropagate();
  }
}

YOGA_EXPORT bool FBYGNodeIsReferenceBaseline(FBYGNodeRef node) {
  return node->isReferenceBaseline();
}

YOGA_EXPORT void FBYGNodeInsertChild(
    const FBYGNodeRef owner,
    const FBYGNodeRef child,
    const uint32_t index) {
  FBYGAssertWithNode(
      owner,
      child->getOwner() == nullptr,
      "Child already has a owner, it must be removed first.");

  FBYGAssertWithNode(
      owner,
      !owner->hasMeasureFunc(),
      "Cannot add child: Nodes with measure functions cannot have children.");

  owner->insertChild(child, index);
  child->setOwner(owner);
  owner->markDirtyAndPropagate();
}

YOGA_EXPORT void FBYGNodeSwapChild(
    const FBYGNodeRef owner,
    const FBYGNodeRef child,
    const uint32_t index) {
  owner->replaceChild(child, index);
  child->setOwner(owner);
}

YOGA_EXPORT void FBYGNodeRemoveChild(
    const FBYGNodeRef owner,
    const FBYGNodeRef excludedChild) {
  if (FBYGNodeGetChildCount(owner) == 0) {
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

YOGA_EXPORT void FBYGNodeRemoveAllChildren(const FBYGNodeRef owner) {
  const uint32_t childCount = FBYGNodeGetChildCount(owner);
  if (childCount == 0) {
    // This is an empty set already. Nothing to do.
    return;
  }
  const FBYGNodeRef firstChild = FBYGNodeGetChild(owner, 0);
  if (firstChild->getOwner() == owner) {
    // If the first child has this node as its owner, we assume that this child
    // set is unique.
    for (uint32_t i = 0; i < childCount; i++) {
      const FBYGNodeRef oldChild = FBYGNodeGetChild(owner, i);
      oldChild->setLayout(FBYGNode().getLayout()); // layout is no longer valid
      oldChild->setOwner(nullptr);
    }
    owner->clearChildren();
    owner->markDirtyAndPropagate();
    return;
  }
  // Otherwise, we are not the owner of the child set. We don't have to do
  // anything to clear it.
  owner->setChildren(FBYGVector());
  owner->markDirtyAndPropagate();
}

YOGA_EXPORT void FBYGNodeSetChildren(
    const FBYGNodeRef owner,
    const FBYGNodeRef* children,
    const uint32_t count) {
  if (!owner) {
    return;
  }

  const FBYGVector childrenVector = {children, children + count};
  if (childrenVector.size() == 0) {
    if (FBYGNodeGetChildCount(owner) > 0) {
      for (FBYGNodeRef const child : owner->getChildren()) {
        child->setLayout(FBYGLayout());
        child->setOwner(nullptr);
      }
      owner->setChildren(FBYGVector());
      owner->markDirtyAndPropagate();
    }
  } else {
    if (FBYGNodeGetChildCount(owner) > 0) {
      for (FBYGNodeRef const oldChild : owner->getChildren()) {
        // Our new children may have nodes in common with the old children. We
        // don't reset these common nodes.
        if (std::find(childrenVector.begin(), childrenVector.end(), oldChild) ==
            childrenVector.end()) {
          oldChild->setLayout(FBYGLayout());
          oldChild->setOwner(nullptr);
        }
      }
    }
    owner->setChildren(childrenVector);
    for (FBYGNodeRef child : childrenVector) {
      child->setOwner(owner);
    }
    owner->markDirtyAndPropagate();
  }
}

YOGA_EXPORT FBYGNodeRef
FBYGNodeGetChild(const FBYGNodeRef node, const uint32_t index) {
  if (index < node->getChildren().size()) {
    return node->getChild(index);
  }
  return nullptr;
}

YOGA_EXPORT uint32_t FBYGNodeGetChildCount(const FBYGNodeRef node) {
  return static_cast<uint32_t>(node->getChildren().size());
}

YOGA_EXPORT FBYGNodeRef FBYGNodeGetOwner(const FBYGNodeRef node) {
  return node->getOwner();
}

YOGA_EXPORT FBYGNodeRef FBYGNodeGetParent(const FBYGNodeRef node) {
  return node->getOwner();
}

YOGA_EXPORT void FBYGNodeMarkDirty(const FBYGNodeRef node) {
  FBYGAssertWithNode(
      node,
      node->hasMeasureFunc(),
      "Only leaf nodes with custom measure functions "
      "should manually mark themselves as dirty");

  node->markDirtyAndPropagate();
}

YOGA_EXPORT void FBYGNodeCopyStyle(
    const FBYGNodeRef dstNode,
    const FBYGNodeRef srcNode) {
  if (!(dstNode->getStyle() == srcNode->getStyle())) {
    dstNode->setStyle(srcNode->getStyle());
    dstNode->markDirtyAndPropagate();
  }
}

YOGA_EXPORT float FBYGNodeStyleGetFlexGrow(const FBYGNodeConstRef node) {
  return node->getStyle().flexGrow().isUndefined()
      ? kDefaultFlexGrow
      : node->getStyle().flexGrow().unwrap();
}

YOGA_EXPORT float FBYGNodeStyleGetFlexShrink(const FBYGNodeConstRef node) {
  return node->getStyle().flexShrink().isUndefined()
      ? (node->getConfig()->useWebDefaults() ? kWebDefaultFlexShrink
                                             : kDefaultFlexShrink)
      : node->getStyle().flexShrink().unwrap();
}

namespace {

template <typename T, typename NeedsUpdate, typename Update>
void updateStyle(
    FBYGNode* node,
    T value,
    NeedsUpdate&& needsUpdate,
    Update&& update) {
  if (needsUpdate(node->getStyle(), value)) {
    update(node->getStyle(), value);
    node->markDirtyAndPropagate();
  }
}

template <typename Ref, typename T>
void updateStyle(FBYGNode* node, Ref (FBYGStyle::*prop)(), T value) {
  updateStyle(
      node,
      value,
      [prop](FBYGStyle& s, T x) { return (s.*prop)() != x; },
      [prop](FBYGStyle& s, T x) { (s.*prop)() = x; });
}

template <typename Ref, typename Idx>
void updateIndexedStyleProp(
    FBYGNode* node,
    Ref (FBYGStyle::*prop)(),
    Idx idx,
    detail::CompactValue value) {
  using detail::CompactValue;
  updateStyle(
      node,
      value,
      [idx, prop](FBYGStyle& s, CompactValue x) { return (s.*prop)()[idx] != x; },
      [idx, prop](FBYGStyle& s, CompactValue x) { (s.*prop)()[idx] = x; });
}

} // namespace

// MSVC has trouble inferring the return type of pointer to member functions
// with const and non-const overloads, instead of preferring the non-const
// overload like clang and GCC. For the purposes of updateStyle(), we can help
// MSVC by specifying that return type explicitly. In combination with
// decltype, MSVC will prefer the non-const version.
#define MSVC_HINT(PROP) decltype(FBYGStyle{}.PROP())

YOGA_EXPORT void FBYGNodeStyleSetDirection(
    const FBYGNodeRef node,
    const FBYGDirection value) {
  updateStyle<MSVC_HINT(direction)>(node, &FBYGStyle::direction, value);
}
YOGA_EXPORT FBYGDirection FBYGNodeStyleGetDirection(const FBYGNodeConstRef node) {
  return node->getStyle().direction();
}

YOGA_EXPORT void FBYGNodeStyleSetFlexDirection(
    const FBYGNodeRef node,
    const FBYGFlexDirection flexDirection) {
  updateStyle<MSVC_HINT(flexDirection)>(
      node, &FBYGStyle::flexDirection, flexDirection);
}
YOGA_EXPORT FBYGFlexDirection
FBYGNodeStyleGetFlexDirection(const FBYGNodeConstRef node) {
  return node->getStyle().flexDirection();
}

YOGA_EXPORT void FBYGNodeStyleSetJustifyContent(
    const FBYGNodeRef node,
    const FBYGJustify justifyContent) {
  updateStyle<MSVC_HINT(justifyContent)>(
      node, &FBYGStyle::justifyContent, justifyContent);
}
YOGA_EXPORT FBYGJustify FBYGNodeStyleGetJustifyContent(const FBYGNodeConstRef node) {
  return node->getStyle().justifyContent();
}

YOGA_EXPORT void FBYGNodeStyleSetAlignContent(
    const FBYGNodeRef node,
    const FBYGAlign alignContent) {
  updateStyle<MSVC_HINT(alignContent)>(
      node, &FBYGStyle::alignContent, alignContent);
}
YOGA_EXPORT FBYGAlign FBYGNodeStyleGetAlignContent(const FBYGNodeConstRef node) {
  return node->getStyle().alignContent();
}

YOGA_EXPORT void FBYGNodeStyleSetAlignItems(
    const FBYGNodeRef node,
    const FBYGAlign alignItems) {
  updateStyle<MSVC_HINT(alignItems)>(node, &FBYGStyle::alignItems, alignItems);
}
YOGA_EXPORT FBYGAlign FBYGNodeStyleGetAlignItems(const FBYGNodeConstRef node) {
  return node->getStyle().alignItems();
}

YOGA_EXPORT void FBYGNodeStyleSetAlignSelf(
    const FBYGNodeRef node,
    const FBYGAlign alignSelf) {
  updateStyle<MSVC_HINT(alignSelf)>(node, &FBYGStyle::alignSelf, alignSelf);
}
YOGA_EXPORT FBYGAlign FBYGNodeStyleGetAlignSelf(const FBYGNodeConstRef node) {
  return node->getStyle().alignSelf();
}

YOGA_EXPORT void FBYGNodeStyleSetPositionType(
    const FBYGNodeRef node,
    const FBYGPositionType positionType) {
  updateStyle<MSVC_HINT(positionType)>(
      node, &FBYGStyle::positionType, positionType);
}
YOGA_EXPORT FBYGPositionType
FBYGNodeStyleGetPositionType(const FBYGNodeConstRef node) {
  return node->getStyle().positionType();
}

YOGA_EXPORT void FBYGNodeStyleSetFlexWrap(
    const FBYGNodeRef node,
    const FBYGWrap flexWrap) {
  updateStyle<MSVC_HINT(flexWrap)>(node, &FBYGStyle::flexWrap, flexWrap);
}
YOGA_EXPORT FBYGWrap FBYGNodeStyleGetFlexWrap(const FBYGNodeConstRef node) {
  return node->getStyle().flexWrap();
}

YOGA_EXPORT void FBYGNodeStyleSetOverflow(
    const FBYGNodeRef node,
    const FBYGOverflow overflow) {
  updateStyle<MSVC_HINT(overflow)>(node, &FBYGStyle::overflow, overflow);
}
YOGA_EXPORT FBYGOverflow FBYGNodeStyleGetOverflow(const FBYGNodeConstRef node) {
  return node->getStyle().overflow();
}

YOGA_EXPORT void FBYGNodeStyleSetDisplay(
    const FBYGNodeRef node,
    const FBYGDisplay display) {
  updateStyle<MSVC_HINT(display)>(node, &FBYGStyle::display, display);
}
YOGA_EXPORT FBYGDisplay FBYGNodeStyleGetDisplay(const FBYGNodeConstRef node) {
  return node->getStyle().display();
}

// TODO(T26792433): Change the API to accept FBYGFloatOptional.
YOGA_EXPORT void FBYGNodeStyleSetFlex(const FBYGNodeRef node, const float flex) {
  updateStyle<MSVC_HINT(flex)>(node, &FBYGStyle::flex, FBYGFloatOptional{flex});
}

// TODO(T26792433): Change the API to accept FBYGFloatOptional.
YOGA_EXPORT float FBYGNodeStyleGetFlex(const FBYGNodeConstRef node) {
  return node->getStyle().flex().isUndefined()
      ? FBYGUndefined
      : node->getStyle().flex().unwrap();
}

// TODO(T26792433): Change the API to accept FBYGFloatOptional.
YOGA_EXPORT void FBYGNodeStyleSetFlexGrow(
    const FBYGNodeRef node,
    const float flexGrow) {
  updateStyle<MSVC_HINT(flexGrow)>(
      node, &FBYGStyle::flexGrow, FBYGFloatOptional{flexGrow});
}

// TODO(T26792433): Change the API to accept FBYGFloatOptional.
YOGA_EXPORT void FBYGNodeStyleSetFlexShrink(
    const FBYGNodeRef node,
    const float flexShrink) {
  updateStyle<MSVC_HINT(flexShrink)>(
      node, &FBYGStyle::flexShrink, FBYGFloatOptional{flexShrink});
}

YOGA_EXPORT FBYGValue FBYGNodeStyleGetFlexBasis(const FBYGNodeConstRef node) {
  FBYGValue flexBasis = node->getStyle().flexBasis();
  if (flexBasis.unit == FBYGUnitUndefined || flexBasis.unit == FBYGUnitAuto) {
    // TODO(T26792433): Get rid off the use of FBYGUndefined at client side
    flexBasis.value = FBYGUndefined;
  }
  return flexBasis;
}

YOGA_EXPORT void FBYGNodeStyleSetFlexBasis(
    const FBYGNodeRef node,
    const float flexBasis) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(flexBasis);
  updateStyle<MSVC_HINT(flexBasis)>(node, &FBYGStyle::flexBasis, value);
}

YOGA_EXPORT void FBYGNodeStyleSetFlexBasisPercent(
    const FBYGNodeRef node,
    const float flexBasisPercent) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(flexBasisPercent);
  updateStyle<MSVC_HINT(flexBasis)>(node, &FBYGStyle::flexBasis, value);
}

YOGA_EXPORT void FBYGNodeStyleSetFlexBasisAuto(const FBYGNodeRef node) {
  updateStyle<MSVC_HINT(flexBasis)>(
      node, &FBYGStyle::flexBasis, detail::CompactValue::ofAuto());
}

YOGA_EXPORT void FBYGNodeStyleSetPosition(
    FBYGNodeRef node,
    FBYGEdge edge,
    float points) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(points);
  updateIndexedStyleProp<MSVC_HINT(position)>(
      node, &FBYGStyle::position, edge, value);
}
YOGA_EXPORT void FBYGNodeStyleSetPositionPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float percent) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(percent);
  updateIndexedStyleProp<MSVC_HINT(position)>(
      node, &FBYGStyle::position, edge, value);
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetPosition(FBYGNodeConstRef node, FBYGEdge edge) {
  return node->getStyle().position()[edge];
}

YOGA_EXPORT void FBYGNodeStyleSetMargin(
    FBYGNodeRef node,
    FBYGEdge edge,
    float points) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(points);
  updateIndexedStyleProp<MSVC_HINT(margin)>(
      node, &FBYGStyle::margin, edge, value);
}
YOGA_EXPORT void FBYGNodeStyleSetMarginPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float percent) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(percent);
  updateIndexedStyleProp<MSVC_HINT(margin)>(
      node, &FBYGStyle::margin, edge, value);
}
YOGA_EXPORT void FBYGNodeStyleSetMarginAuto(FBYGNodeRef node, FBYGEdge edge) {
  updateIndexedStyleProp<MSVC_HINT(margin)>(
      node, &FBYGStyle::margin, edge, detail::CompactValue::ofAuto());
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetMargin(FBYGNodeConstRef node, FBYGEdge edge) {
  return node->getStyle().margin()[edge];
}

YOGA_EXPORT void FBYGNodeStyleSetPadding(
    FBYGNodeRef node,
    FBYGEdge edge,
    float points) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(points);
  updateIndexedStyleProp<MSVC_HINT(padding)>(
      node, &FBYGStyle::padding, edge, value);
}
YOGA_EXPORT void FBYGNodeStyleSetPaddingPercent(
    FBYGNodeRef node,
    FBYGEdge edge,
    float percent) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(percent);
  updateIndexedStyleProp<MSVC_HINT(padding)>(
      node, &FBYGStyle::padding, edge, value);
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetPadding(FBYGNodeConstRef node, FBYGEdge edge) {
  return node->getStyle().padding()[edge];
}

// TODO(T26792433): Change the API to accept FBYGFloatOptional.
YOGA_EXPORT void FBYGNodeStyleSetBorder(
    const FBYGNodeRef node,
    const FBYGEdge edge,
    const float border) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(border);
  updateIndexedStyleProp<MSVC_HINT(border)>(
      node, &FBYGStyle::border, edge, value);
}

YOGA_EXPORT float FBYGNodeStyleGetBorder(
    const FBYGNodeConstRef node,
    const FBYGEdge edge) {
  auto border = node->getStyle().border()[edge];
  if (border.isUndefined() || border.isAuto()) {
    // TODO(T26792433): Rather than returning FBYGUndefined, change the api to
    // return FBYGFloatOptional.
    return FBYGUndefined;
  }

  return static_cast<FBYGValue>(border).value;
}

YOGA_EXPORT void FBYGNodeStyleSetGap(
    const FBYGNodeRef node,
    const FBYGGutter gutter,
    const float gapLength) {
  auto length = detail::CompactValue::ofMaybe<FBYGUnitPoint>(gapLength);
  updateIndexedStyleProp<MSVC_HINT(gap)>(node, &FBYGStyle::gap, gutter, length);
}

YOGA_EXPORT float FBYGNodeStyleGetGap(
    const FBYGNodeConstRef node,
    const FBYGGutter gutter) {
  auto gapLength = node->getStyle().gap()[gutter];
  if (gapLength.isUndefined() || gapLength.isAuto()) {
    // TODO(T26792433): Rather than returning FBYGUndefined, change the api to
    // return FBYGFloatOptional.
    return FBYGUndefined;
  }

  return static_cast<FBYGValue>(gapLength).value;
}

// Yoga specific properties, not compatible with flexbox specification

// TODO(T26792433): Change the API to accept FBYGFloatOptional.
YOGA_EXPORT float FBYGNodeStyleGetAspectRatio(const FBYGNodeConstRef node) {
  const FBYGFloatOptional op = node->getStyle().aspectRatio();
  return op.isUndefined() ? FBYGUndefined : op.unwrap();
}

// TODO(T26792433): Change the API to accept FBYGFloatOptional.
YOGA_EXPORT void FBYGNodeStyleSetAspectRatio(
    const FBYGNodeRef node,
    const float aspectRatio) {
  updateStyle<MSVC_HINT(aspectRatio)>(
      node, &FBYGStyle::aspectRatio, FBYGFloatOptional{aspectRatio});
}

YOGA_EXPORT void FBYGNodeStyleSetWidth(FBYGNodeRef node, float points) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(points);
  updateIndexedStyleProp<MSVC_HINT(dimensions)>(
      node, &FBYGStyle::dimensions, FBYGDimensionWidth, value);
}
YOGA_EXPORT void FBYGNodeStyleSetWidthPercent(FBYGNodeRef node, float percent) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(percent);
  updateIndexedStyleProp<MSVC_HINT(dimensions)>(
      node, &FBYGStyle::dimensions, FBYGDimensionWidth, value);
}
YOGA_EXPORT void FBYGNodeStyleSetWidthAuto(FBYGNodeRef node) {
  updateIndexedStyleProp<MSVC_HINT(dimensions)>(
      node,
      &FBYGStyle::dimensions,
      FBYGDimensionWidth,
      detail::CompactValue::ofAuto());
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetWidth(FBYGNodeConstRef node) {
  return node->getStyle().dimensions()[FBYGDimensionWidth];
}

YOGA_EXPORT void FBYGNodeStyleSetHeight(FBYGNodeRef node, float points) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(points);
  updateIndexedStyleProp<MSVC_HINT(dimensions)>(
      node, &FBYGStyle::dimensions, FBYGDimensionHeight, value);
}
YOGA_EXPORT void FBYGNodeStyleSetHeightPercent(FBYGNodeRef node, float percent) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(percent);
  updateIndexedStyleProp<MSVC_HINT(dimensions)>(
      node, &FBYGStyle::dimensions, FBYGDimensionHeight, value);
}
YOGA_EXPORT void FBYGNodeStyleSetHeightAuto(FBYGNodeRef node) {
  updateIndexedStyleProp<MSVC_HINT(dimensions)>(
      node,
      &FBYGStyle::dimensions,
      FBYGDimensionHeight,
      detail::CompactValue::ofAuto());
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetHeight(FBYGNodeConstRef node) {
  return node->getStyle().dimensions()[FBYGDimensionHeight];
}

YOGA_EXPORT void FBYGNodeStyleSetMinWidth(
    const FBYGNodeRef node,
    const float minWidth) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(minWidth);
  updateIndexedStyleProp<MSVC_HINT(minDimensions)>(
      node, &FBYGStyle::minDimensions, FBYGDimensionWidth, value);
}
YOGA_EXPORT void FBYGNodeStyleSetMinWidthPercent(
    const FBYGNodeRef node,
    const float minWidth) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(minWidth);
  updateIndexedStyleProp<MSVC_HINT(minDimensions)>(
      node, &FBYGStyle::minDimensions, FBYGDimensionWidth, value);
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetMinWidth(const FBYGNodeConstRef node) {
  return node->getStyle().minDimensions()[FBYGDimensionWidth];
}

YOGA_EXPORT void FBYGNodeStyleSetMinHeight(
    const FBYGNodeRef node,
    const float minHeight) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(minHeight);
  updateIndexedStyleProp<MSVC_HINT(minDimensions)>(
      node, &FBYGStyle::minDimensions, FBYGDimensionHeight, value);
}
YOGA_EXPORT void FBYGNodeStyleSetMinHeightPercent(
    const FBYGNodeRef node,
    const float minHeight) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(minHeight);
  updateIndexedStyleProp<MSVC_HINT(minDimensions)>(
      node, &FBYGStyle::minDimensions, FBYGDimensionHeight, value);
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetMinHeight(const FBYGNodeConstRef node) {
  return node->getStyle().minDimensions()[FBYGDimensionHeight];
}

YOGA_EXPORT void FBYGNodeStyleSetMaxWidth(
    const FBYGNodeRef node,
    const float maxWidth) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(maxWidth);
  updateIndexedStyleProp<MSVC_HINT(maxDimensions)>(
      node, &FBYGStyle::maxDimensions, FBYGDimensionWidth, value);
}
YOGA_EXPORT void FBYGNodeStyleSetMaxWidthPercent(
    const FBYGNodeRef node,
    const float maxWidth) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(maxWidth);
  updateIndexedStyleProp<MSVC_HINT(maxDimensions)>(
      node, &FBYGStyle::maxDimensions, FBYGDimensionWidth, value);
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetMaxWidth(const FBYGNodeConstRef node) {
  return node->getStyle().maxDimensions()[FBYGDimensionWidth];
}

YOGA_EXPORT void FBYGNodeStyleSetMaxHeight(
    const FBYGNodeRef node,
    const float maxHeight) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPoint>(maxHeight);
  updateIndexedStyleProp<MSVC_HINT(maxDimensions)>(
      node, &FBYGStyle::maxDimensions, FBYGDimensionHeight, value);
}
YOGA_EXPORT void FBYGNodeStyleSetMaxHeightPercent(
    const FBYGNodeRef node,
    const float maxHeight) {
  auto value = detail::CompactValue::ofMaybe<FBYGUnitPercent>(maxHeight);
  updateIndexedStyleProp<MSVC_HINT(maxDimensions)>(
      node, &FBYGStyle::maxDimensions, FBYGDimensionHeight, value);
}
YOGA_EXPORT FBYGValue FBYGNodeStyleGetMaxHeight(const FBYGNodeConstRef node) {
  return node->getStyle().maxDimensions()[FBYGDimensionHeight];
}

#define FBYG_NODE_LAYOUT_PROPERTY_IMPL(type, name, instanceName)   \
  YOGA_EXPORT type FBYGNodeLayoutGet##name(const FBYGNodeRef node) { \
    return node->getLayout().instanceName;                       \
  }

#define FBYG_NODE_LAYOUT_RESOLVED_PROPERTY_IMPL(type, name, instanceName) \
  YOGA_EXPORT type FBYGNodeLayoutGet##name(                               \
      const FBYGNodeRef node, const FBYGEdge edge) {                        \
    FBYGAssertWithNode(                                                   \
        node,                                                           \
        edge <= FBYGEdgeEnd,                                              \
        "Cannot get layout properties of multi-edge shorthands");       \
                                                                        \
    if (edge == FBYGEdgeStart) {                                          \
      if (node->getLayout().direction() == FBYGDirectionRTL) {            \
        return node->getLayout().instanceName[FBYGEdgeRight];             \
      } else {                                                          \
        return node->getLayout().instanceName[FBYGEdgeLeft];              \
      }                                                                 \
    }                                                                   \
                                                                        \
    if (edge == FBYGEdgeEnd) {                                            \
      if (node->getLayout().direction() == FBYGDirectionRTL) {            \
        return node->getLayout().instanceName[FBYGEdgeLeft];              \
      } else {                                                          \
        return node->getLayout().instanceName[FBYGEdgeRight];             \
      }                                                                 \
    }                                                                   \
                                                                        \
    return node->getLayout().instanceName[edge];                        \
  }

FBYG_NODE_LAYOUT_PROPERTY_IMPL(float, Left, position[FBYGEdgeLeft])
FBYG_NODE_LAYOUT_PROPERTY_IMPL(float, Top, position[FBYGEdgeTop])
FBYG_NODE_LAYOUT_PROPERTY_IMPL(float, Right, position[FBYGEdgeRight])
FBYG_NODE_LAYOUT_PROPERTY_IMPL(float, Bottom, position[FBYGEdgeBottom])
FBYG_NODE_LAYOUT_PROPERTY_IMPL(float, Width, dimensions[FBYGDimensionWidth])
FBYG_NODE_LAYOUT_PROPERTY_IMPL(float, Height, dimensions[FBYGDimensionHeight])
FBYG_NODE_LAYOUT_PROPERTY_IMPL(FBYGDirection, Direction, direction())
FBYG_NODE_LAYOUT_PROPERTY_IMPL(bool, HadOverflow, hadOverflow())

FBYG_NODE_LAYOUT_RESOLVED_PROPERTY_IMPL(float, Margin, margin)
FBYG_NODE_LAYOUT_RESOLVED_PROPERTY_IMPL(float, Border, border)
FBYG_NODE_LAYOUT_RESOLVED_PROPERTY_IMPL(float, Padding, padding)

std::atomic<uint32_t> gCurrentGenerationCount(0);

bool FBYGLayoutNodeInternal(
    const FBYGNodeRef node,
    const float availableWidth,
    const float availableHeight,
    const FBYGDirection ownerDirection,
    const FBYGMeasureMode widthMeasureMode,
    const FBYGMeasureMode heightMeasureMode,
    const float ownerWidth,
    const float ownerHeight,
    const bool performLayout,
    const LayoutPassReason reason,
    const FBYGConfigRef config,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    const uint32_t depth,
    const uint32_t generationCount);

#ifdef DEBUG
static void FBYGNodePrintInternal(
    const FBYGNodeRef node,
    const FBYGPrintOptions options) {
  std::string str;
  facebook::yoga::FBYGNodeToString(str, node, options, 0);
  Log::log(node, FBYGLogLevelDebug, nullptr, str.c_str());
}

YOGA_EXPORT void FBYGNodePrint(
    const FBYGNodeRef node,
    const FBYGPrintOptions options) {
  FBYGNodePrintInternal(node, options);
}
#endif

const std::array<FBYGEdge, 4> leading = {
    {FBYGEdgeTop, FBYGEdgeBottom, FBYGEdgeLeft, FBYGEdgeRight}};

const std::array<FBYGEdge, 4> trailing = {
    {FBYGEdgeBottom, FBYGEdgeTop, FBYGEdgeRight, FBYGEdgeLeft}};
static const std::array<FBYGEdge, 4> pos = {{
    FBYGEdgeTop,
    FBYGEdgeBottom,
    FBYGEdgeLeft,
    FBYGEdgeRight,
}};

static const std::array<FBYGDimension, 4> dim = {
    {FBYGDimensionHeight, FBYGDimensionHeight, FBYGDimensionWidth, FBYGDimensionWidth}};

static inline float FBYGNodePaddingAndBorderForAxis(
    const FBYGNodeConstRef node,
    const FBYGFlexDirection axis,
    const float widthSize) {
  return (node->getLeadingPaddingAndBorder(axis, widthSize) +
          node->getTrailingPaddingAndBorder(axis, widthSize))
      .unwrap();
}

static inline FBYGAlign FBYGNodeAlignItem(const FBYGNode* node, const FBYGNode* child) {
  const FBYGAlign align = child->getStyle().alignSelf() == FBYGAlignAuto
      ? node->getStyle().alignItems()
      : child->getStyle().alignSelf();
  if (align == FBYGAlignBaseline &&
      FBYGFlexDirectionIsColumn(node->getStyle().flexDirection())) {
    return FBYGAlignFlexStart;
  }
  return align;
}

static float FBYGBaseline(const FBYGNodeRef node, void* layoutContext) {
  if (node->hasBaselineFunc()) {

    Event::publish<Event::NodeBaselineStart>(node);

    const float baseline = node->baseline(
        node->getLayout().measuredDimensions[FBYGDimensionWidth],
        node->getLayout().measuredDimensions[FBYGDimensionHeight],
        layoutContext);

    Event::publish<Event::NodeBaselineEnd>(node);

    FBYGAssertWithNode(
        node,
        !FBYGFloatIsUndefined(baseline),
        "Expect custom baseline function to not return NaN");
    return baseline;
  }

  FBYGNodeRef baselineChild = nullptr;
  const uint32_t childCount = FBYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    const FBYGNodeRef child = FBYGNodeGetChild(node, i);
    if (child->getLineIndex() > 0) {
      break;
    }
    if (child->getStyle().positionType() == FBYGPositionTypeAbsolute) {
      continue;
    }
    if (FBYGNodeAlignItem(node, child) == FBYGAlignBaseline ||
        child->isReferenceBaseline()) {
      baselineChild = child;
      break;
    }

    if (baselineChild == nullptr) {
      baselineChild = child;
    }
  }

  if (baselineChild == nullptr) {
    return node->getLayout().measuredDimensions[FBYGDimensionHeight];
  }

  const float baseline = FBYGBaseline(baselineChild, layoutContext);
  return baseline + baselineChild->getLayout().position[FBYGEdgeTop];
}

static bool FBYGIsBaselineLayout(const FBYGNodeRef node) {
  if (FBYGFlexDirectionIsColumn(node->getStyle().flexDirection())) {
    return false;
  }
  if (node->getStyle().alignItems() == FBYGAlignBaseline) {
    return true;
  }
  const uint32_t childCount = FBYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    const FBYGNodeRef child = FBYGNodeGetChild(node, i);
    if (child->getStyle().positionType() != FBYGPositionTypeAbsolute &&
        child->getStyle().alignSelf() == FBYGAlignBaseline) {
      return true;
    }
  }

  return false;
}

static inline float FBYGNodeDimWithMargin(
    const FBYGNodeRef node,
    const FBYGFlexDirection axis,
    const float widthSize) {
  return node->getLayout().measuredDimensions[dim[axis]] +
      (node->getLeadingMargin(axis, widthSize) +
       node->getTrailingMargin(axis, widthSize))
          .unwrap();
}

static inline bool FBYGNodeIsStyleDimDefined(
    const FBYGNodeRef node,
    const FBYGFlexDirection axis,
    const float ownerSize) {
  bool isUndefined =
      FBYGFloatIsUndefined(node->getResolvedDimension(dim[axis]).value);
  return !(
      node->getResolvedDimension(dim[axis]).unit == FBYGUnitAuto ||
      node->getResolvedDimension(dim[axis]).unit == FBYGUnitUndefined ||
      (node->getResolvedDimension(dim[axis]).unit == FBYGUnitPoint &&
       !isUndefined && node->getResolvedDimension(dim[axis]).value < 0.0f) ||
      (node->getResolvedDimension(dim[axis]).unit == FBYGUnitPercent &&
       !isUndefined &&
       (node->getResolvedDimension(dim[axis]).value < 0.0f ||
        FBYGFloatIsUndefined(ownerSize))));
}

static inline bool FBYGNodeIsLayoutDimDefined(
    const FBYGNodeRef node,
    const FBYGFlexDirection axis) {
  const float value = node->getLayout().measuredDimensions[dim[axis]];
  return !FBYGFloatIsUndefined(value) && value >= 0.0f;
}

static FBYGFloatOptional FBYGNodeBoundAxisWithinMinAndMax(
    const FBYGNodeConstRef node,
    const FBYGFlexDirection axis,
    const FBYGFloatOptional value,
    const float axisSize) {
  FBYGFloatOptional min;
  FBYGFloatOptional max;

  if (FBYGFlexDirectionIsColumn(axis)) {
    min = FBYGResolveValue(
        node->getStyle().minDimensions()[FBYGDimensionHeight], axisSize);
    max = FBYGResolveValue(
        node->getStyle().maxDimensions()[FBYGDimensionHeight], axisSize);
  } else if (FBYGFlexDirectionIsRow(axis)) {
    min = FBYGResolveValue(
        node->getStyle().minDimensions()[FBYGDimensionWidth], axisSize);
    max = FBYGResolveValue(
        node->getStyle().maxDimensions()[FBYGDimensionWidth], axisSize);
  }

  if (max >= FBYGFloatOptional{0} && value > max) {
    return max;
  }

  if (min >= FBYGFloatOptional{0} && value < min) {
    return min;
  }

  return value;
}

// Like FBYGNodeBoundAxisWithinMinAndMax but also ensures that the value doesn't
// go below the padding and border amount.
static inline float FBYGNodeBoundAxis(
    const FBYGNodeRef node,
    const FBYGFlexDirection axis,
    const float value,
    const float axisSize,
    const float widthSize) {
  return FBYGFloatMax(
      FBYGNodeBoundAxisWithinMinAndMax(
          node, axis, FBYGFloatOptional{value}, axisSize)
          .unwrap(),
      FBYGNodePaddingAndBorderForAxis(node, axis, widthSize));
}

static void FBYGNodeSetChildTrailingPosition(
    const FBYGNodeRef node,
    const FBYGNodeRef child,
    const FBYGFlexDirection axis) {
  const float size = child->getLayout().measuredDimensions[dim[axis]];
  child->setLayoutPosition(
      node->getLayout().measuredDimensions[dim[axis]] - size -
          child->getLayout().position[pos[axis]],
      trailing[axis]);
}

static void FBYGConstrainMaxSizeForMode(
    const FBYGNodeConstRef node,
    const enum FBYGFlexDirection axis,
    const float ownerAxisSize,
    const float ownerWidth,
    FBYGMeasureMode* mode,
    float* size) {
  const FBYGFloatOptional maxSize =
      FBYGResolveValue(
          node->getStyle().maxDimensions()[dim[axis]], ownerAxisSize) +
      FBYGFloatOptional(node->getMarginForAxis(axis, ownerWidth));
  switch (*mode) {
    case FBYGMeasureModeExactly:
    case FBYGMeasureModeAtMost:
      *size = (maxSize.isUndefined() || *size < maxSize.unwrap())
          ? *size
          : maxSize.unwrap();
      break;
    case FBYGMeasureModeUndefined:
      if (!maxSize.isUndefined()) {
        *mode = FBYGMeasureModeAtMost;
        *size = maxSize.unwrap();
      }
      break;
  }
}

static void FBYGNodeComputeFlexBasisForChild(
    const FBYGNodeRef node,
    const FBYGNodeRef child,
    const float width,
    const FBYGMeasureMode widthMode,
    const float height,
    const float ownerWidth,
    const float ownerHeight,
    const FBYGMeasureMode heightMode,
    const FBYGDirection direction,
    const FBYGConfigRef config,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    const uint32_t depth,
    const uint32_t generationCount) {
  const FBYGFlexDirection mainAxis =
      FBYGResolveFlexDirection(node->getStyle().flexDirection(), direction);
  const bool isMainAxisRow = FBYGFlexDirectionIsRow(mainAxis);
  const float mainAxisSize = isMainAxisRow ? width : height;
  const float mainAxisownerSize = isMainAxisRow ? ownerWidth : ownerHeight;

  float childWidth;
  float childHeight;
  FBYGMeasureMode childWidthMeasureMode;
  FBYGMeasureMode childHeightMeasureMode;

  const FBYGFloatOptional resolvedFlexBasis =
      FBYGResolveValue(child->resolveFlexBasisPtr(), mainAxisownerSize);
  const bool isRowStyleDimDefined =
      FBYGNodeIsStyleDimDefined(child, FBYGFlexDirectionRow, ownerWidth);
  const bool isColumnStyleDimDefined =
      FBYGNodeIsStyleDimDefined(child, FBYGFlexDirectionColumn, ownerHeight);

  if (!resolvedFlexBasis.isUndefined() && !FBYGFloatIsUndefined(mainAxisSize)) {
    if (child->getLayout().computedFlexBasis.isUndefined() ||
        (child->getConfig()->isExperimentalFeatureEnabled(
             FBYGExperimentalFeatureWebFlexBasis) &&
         child->getLayout().computedFlexBasisGeneration != generationCount)) {
      const FBYGFloatOptional paddingAndBorder = FBYGFloatOptional(
          FBYGNodePaddingAndBorderForAxis(child, mainAxis, ownerWidth));
      child->setLayoutComputedFlexBasis(
          FBYGFloatOptionalMax(resolvedFlexBasis, paddingAndBorder));
    }
  } else if (isMainAxisRow && isRowStyleDimDefined) {
    // The width is definite, so use that as the flex basis.
    const FBYGFloatOptional paddingAndBorder = FBYGFloatOptional(
        FBYGNodePaddingAndBorderForAxis(child, FBYGFlexDirectionRow, ownerWidth));

    child->setLayoutComputedFlexBasis(FBYGFloatOptionalMax(
        FBYGResolveValue(
            child->getResolvedDimensions()[FBYGDimensionWidth], ownerWidth),
        paddingAndBorder));
  } else if (!isMainAxisRow && isColumnStyleDimDefined) {
    // The height is definite, so use that as the flex basis.
    const FBYGFloatOptional paddingAndBorder =
        FBYGFloatOptional(FBYGNodePaddingAndBorderForAxis(
            child, FBYGFlexDirectionColumn, ownerWidth));
    child->setLayoutComputedFlexBasis(FBYGFloatOptionalMax(
        FBYGResolveValue(
            child->getResolvedDimensions()[FBYGDimensionHeight], ownerHeight),
        paddingAndBorder));
  } else {
    // Compute the flex basis and hypothetical main size (i.e. the clamped flex
    // basis).
    childWidth = FBYGUndefined;
    childHeight = FBYGUndefined;
    childWidthMeasureMode = FBYGMeasureModeUndefined;
    childHeightMeasureMode = FBYGMeasureModeUndefined;

    auto marginRow =
        child->getMarginForAxis(FBYGFlexDirectionRow, ownerWidth).unwrap();
    auto marginColumn =
        child->getMarginForAxis(FBYGFlexDirectionColumn, ownerWidth).unwrap();

    if (isRowStyleDimDefined) {
      childWidth =
          FBYGResolveValue(
              child->getResolvedDimensions()[FBYGDimensionWidth], ownerWidth)
              .unwrap() +
          marginRow;
      childWidthMeasureMode = FBYGMeasureModeExactly;
    }
    if (isColumnStyleDimDefined) {
      childHeight =
          FBYGResolveValue(
              child->getResolvedDimensions()[FBYGDimensionHeight], ownerHeight)
              .unwrap() +
          marginColumn;
      childHeightMeasureMode = FBYGMeasureModeExactly;
    }

    // The W3C spec doesn't say anything about the 'overflow' property, but all
    // major browsers appear to implement the following logic.
    if ((!isMainAxisRow && node->getStyle().overflow() == FBYGOverflowScroll) ||
        node->getStyle().overflow() != FBYGOverflowScroll) {
      if (FBYGFloatIsUndefined(childWidth) && !FBYGFloatIsUndefined(width)) {
        childWidth = width;
        childWidthMeasureMode = FBYGMeasureModeAtMost;
      }
    }

    if ((isMainAxisRow && node->getStyle().overflow() == FBYGOverflowScroll) ||
        node->getStyle().overflow() != FBYGOverflowScroll) {
      if (FBYGFloatIsUndefined(childHeight) && !FBYGFloatIsUndefined(height)) {
        childHeight = height;
        childHeightMeasureMode = FBYGMeasureModeAtMost;
      }
    }

    const auto& childStyle = child->getStyle();
    if (!childStyle.aspectRatio().isUndefined()) {
      if (!isMainAxisRow && childWidthMeasureMode == FBYGMeasureModeExactly) {
        childHeight = marginColumn +
            (childWidth - marginRow) / childStyle.aspectRatio().unwrap();
        childHeightMeasureMode = FBYGMeasureModeExactly;
      } else if (
          isMainAxisRow && childHeightMeasureMode == FBYGMeasureModeExactly) {
        childWidth = marginRow +
            (childHeight - marginColumn) * childStyle.aspectRatio().unwrap();
        childWidthMeasureMode = FBYGMeasureModeExactly;
      }
    }

    // If child has no defined size in the cross axis and is set to stretch, set
    // the cross axis to be measured exactly with the available inner width

    const bool hasExactWidth =
        !FBYGFloatIsUndefined(width) && widthMode == FBYGMeasureModeExactly;
    const bool childWidthStretch =
        FBYGNodeAlignItem(node, child) == FBYGAlignStretch &&
        childWidthMeasureMode != FBYGMeasureModeExactly;
    if (!isMainAxisRow && !isRowStyleDimDefined && hasExactWidth &&
        childWidthStretch) {
      childWidth = width;
      childWidthMeasureMode = FBYGMeasureModeExactly;
      if (!childStyle.aspectRatio().isUndefined()) {
        childHeight =
            (childWidth - marginRow) / childStyle.aspectRatio().unwrap();
        childHeightMeasureMode = FBYGMeasureModeExactly;
      }
    }

    const bool hasExactHeight =
        !FBYGFloatIsUndefined(height) && heightMode == FBYGMeasureModeExactly;
    const bool childHeightStretch =
        FBYGNodeAlignItem(node, child) == FBYGAlignStretch &&
        childHeightMeasureMode != FBYGMeasureModeExactly;
    if (isMainAxisRow && !isColumnStyleDimDefined && hasExactHeight &&
        childHeightStretch) {
      childHeight = height;
      childHeightMeasureMode = FBYGMeasureModeExactly;

      if (!childStyle.aspectRatio().isUndefined()) {
        childWidth =
            (childHeight - marginColumn) * childStyle.aspectRatio().unwrap();
        childWidthMeasureMode = FBYGMeasureModeExactly;
      }
    }

    FBYGConstrainMaxSizeForMode(
        child,
        FBYGFlexDirectionRow,
        ownerWidth,
        ownerWidth,
        &childWidthMeasureMode,
        &childWidth);
    FBYGConstrainMaxSizeForMode(
        child,
        FBYGFlexDirectionColumn,
        ownerHeight,
        ownerWidth,
        &childHeightMeasureMode,
        &childHeight);

    // Measure the child
    FBYGLayoutNodeInternal(
        child,
        childWidth,
        childHeight,
        direction,
        childWidthMeasureMode,
        childHeightMeasureMode,
        ownerWidth,
        ownerHeight,
        false,
        LayoutPassReason::kMeasureChild,
        config,
        layoutMarkerData,
        layoutContext,
        depth,
        generationCount);

    child->setLayoutComputedFlexBasis(FBYGFloatOptional(FBYGFloatMax(
        child->getLayout().measuredDimensions[dim[mainAxis]],
        FBYGNodePaddingAndBorderForAxis(child, mainAxis, ownerWidth))));
  }
  child->setLayoutComputedFlexBasisGeneration(generationCount);
}

static void FBYGNodeAbsoluteLayoutChild(
    const FBYGNodeRef node,
    const FBYGNodeRef child,
    const float width,
    const FBYGMeasureMode widthMode,
    const float height,
    const FBYGDirection direction,
    const FBYGConfigRef config,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    const uint32_t depth,
    const uint32_t generationCount) {
  const FBYGFlexDirection mainAxis =
      FBYGResolveFlexDirection(node->getStyle().flexDirection(), direction);
  const FBYGFlexDirection crossAxis = FBYGFlexDirectionCross(mainAxis, direction);
  const bool isMainAxisRow = FBYGFlexDirectionIsRow(mainAxis);

  float childWidth = FBYGUndefined;
  float childHeight = FBYGUndefined;
  FBYGMeasureMode childWidthMeasureMode = FBYGMeasureModeUndefined;
  FBYGMeasureMode childHeightMeasureMode = FBYGMeasureModeUndefined;

  auto marginRow = child->getMarginForAxis(FBYGFlexDirectionRow, width).unwrap();
  auto marginColumn =
      child->getMarginForAxis(FBYGFlexDirectionColumn, width).unwrap();

  if (FBYGNodeIsStyleDimDefined(child, FBYGFlexDirectionRow, width)) {
    childWidth =
        FBYGResolveValue(child->getResolvedDimensions()[FBYGDimensionWidth], width)
            .unwrap() +
        marginRow;
  } else {
    // If the child doesn't have a specified width, compute the width based on
    // the left/right offsets if they're defined.
    if (child->isLeadingPositionDefined(FBYGFlexDirectionRow) &&
        child->isTrailingPosDefined(FBYGFlexDirectionRow)) {
      childWidth = node->getLayout().measuredDimensions[FBYGDimensionWidth] -
          (node->getLeadingBorder(FBYGFlexDirectionRow) +
           node->getTrailingBorder(FBYGFlexDirectionRow)) -
          (child->getLeadingPosition(FBYGFlexDirectionRow, width) +
           child->getTrailingPosition(FBYGFlexDirectionRow, width))
              .unwrap();
      childWidth =
          FBYGNodeBoundAxis(child, FBYGFlexDirectionRow, childWidth, width, width);
    }
  }

  if (FBYGNodeIsStyleDimDefined(child, FBYGFlexDirectionColumn, height)) {
    childHeight = FBYGResolveValue(
                      child->getResolvedDimensions()[FBYGDimensionHeight], height)
                      .unwrap() +
        marginColumn;
  } else {
    // If the child doesn't have a specified height, compute the height based on
    // the top/bottom offsets if they're defined.
    if (child->isLeadingPositionDefined(FBYGFlexDirectionColumn) &&
        child->isTrailingPosDefined(FBYGFlexDirectionColumn)) {
      childHeight = node->getLayout().measuredDimensions[FBYGDimensionHeight] -
          (node->getLeadingBorder(FBYGFlexDirectionColumn) +
           node->getTrailingBorder(FBYGFlexDirectionColumn)) -
          (child->getLeadingPosition(FBYGFlexDirectionColumn, height) +
           child->getTrailingPosition(FBYGFlexDirectionColumn, height))
              .unwrap();
      childHeight = FBYGNodeBoundAxis(
          child, FBYGFlexDirectionColumn, childHeight, height, width);
    }
  }

  // Exactly one dimension needs to be defined for us to be able to do aspect
  // ratio calculation. One dimension being the anchor and the other being
  // flexible.
  const auto& childStyle = child->getStyle();
  if (FBYGFloatIsUndefined(childWidth) ^ FBYGFloatIsUndefined(childHeight)) {
    if (!childStyle.aspectRatio().isUndefined()) {
      if (FBYGFloatIsUndefined(childWidth)) {
        childWidth = marginRow +
            (childHeight - marginColumn) * childStyle.aspectRatio().unwrap();
      } else if (FBYGFloatIsUndefined(childHeight)) {
        childHeight = marginColumn +
            (childWidth - marginRow) / childStyle.aspectRatio().unwrap();
      }
    }
  }

  // If we're still missing one or the other dimension, measure the content.
  if (FBYGFloatIsUndefined(childWidth) || FBYGFloatIsUndefined(childHeight)) {
    childWidthMeasureMode = FBYGFloatIsUndefined(childWidth)
        ? FBYGMeasureModeUndefined
        : FBYGMeasureModeExactly;
    childHeightMeasureMode = FBYGFloatIsUndefined(childHeight)
        ? FBYGMeasureModeUndefined
        : FBYGMeasureModeExactly;

    // If the size of the owner is defined then try to constrain the absolute
    // child to that size as well. This allows text within the absolute child to
    // wrap to the size of its owner. This is the same behavior as many browsers
    // implement.
    if (!isMainAxisRow && FBYGFloatIsUndefined(childWidth) &&
        widthMode != FBYGMeasureModeUndefined && !FBYGFloatIsUndefined(width) &&
        width > 0) {
      childWidth = width;
      childWidthMeasureMode = FBYGMeasureModeAtMost;
    }

    FBYGLayoutNodeInternal(
        child,
        childWidth,
        childHeight,
        direction,
        childWidthMeasureMode,
        childHeightMeasureMode,
        childWidth,
        childHeight,
        false,
        LayoutPassReason::kAbsMeasureChild,
        config,
        layoutMarkerData,
        layoutContext,
        depth,
        generationCount);
    childWidth = child->getLayout().measuredDimensions[FBYGDimensionWidth] +
        child->getMarginForAxis(FBYGFlexDirectionRow, width).unwrap();
    childHeight = child->getLayout().measuredDimensions[FBYGDimensionHeight] +
        child->getMarginForAxis(FBYGFlexDirectionColumn, width).unwrap();
  }

  FBYGLayoutNodeInternal(
      child,
      childWidth,
      childHeight,
      direction,
      FBYGMeasureModeExactly,
      FBYGMeasureModeExactly,
      childWidth,
      childHeight,
      true,
      LayoutPassReason::kAbsLayout,
      config,
      layoutMarkerData,
      layoutContext,
      depth,
      generationCount);

  if (child->isTrailingPosDefined(mainAxis) &&
      !child->isLeadingPositionDefined(mainAxis)) {
    child->setLayoutPosition(
        node->getLayout().measuredDimensions[dim[mainAxis]] -
            child->getLayout().measuredDimensions[dim[mainAxis]] -
            node->getTrailingBorder(mainAxis) -
            child->getTrailingMargin(mainAxis, isMainAxisRow ? width : height)
                .unwrap() -
            child->getTrailingPosition(mainAxis, isMainAxisRow ? width : height)
                .unwrap(),
        leading[mainAxis]);
  } else if (
      !child->isLeadingPositionDefined(mainAxis) &&
      node->getStyle().justifyContent() == FBYGJustifyCenter) {
    child->setLayoutPosition(
        (node->getLayout().measuredDimensions[dim[mainAxis]] -
         child->getLayout().measuredDimensions[dim[mainAxis]]) /
            2.0f,
        leading[mainAxis]);
  } else if (
      !child->isLeadingPositionDefined(mainAxis) &&
      node->getStyle().justifyContent() == FBYGJustifyFlexEnd) {
    child->setLayoutPosition(
        (node->getLayout().measuredDimensions[dim[mainAxis]] -
         child->getLayout().measuredDimensions[dim[mainAxis]]),
        leading[mainAxis]);
  } else if (
      node->getConfig()->isExperimentalFeatureEnabled(
          FBYGExperimentalFeatureAbsolutePercentageAgainstPaddingEdge) &&
      child->isLeadingPositionDefined(mainAxis)) {
    child->setLayoutPosition(
        child->getLeadingPosition(
                 mainAxis, node->getLayout().measuredDimensions[dim[mainAxis]])
                .unwrap() +
            node->getLeadingBorder(mainAxis) +
            child
                ->getLeadingMargin(
                    mainAxis,
                    node->getLayout().measuredDimensions[dim[mainAxis]])
                .unwrap(),
        leading[mainAxis]);
  }

  if (child->isTrailingPosDefined(crossAxis) &&
      !child->isLeadingPositionDefined(crossAxis)) {
    child->setLayoutPosition(
        node->getLayout().measuredDimensions[dim[crossAxis]] -
            child->getLayout().measuredDimensions[dim[crossAxis]] -
            node->getTrailingBorder(crossAxis) -
            child->getTrailingMargin(crossAxis, isMainAxisRow ? height : width)
                .unwrap() -
            child
                ->getTrailingPosition(crossAxis, isMainAxisRow ? height : width)
                .unwrap(),
        leading[crossAxis]);

  } else if (
      !child->isLeadingPositionDefined(crossAxis) &&
      FBYGNodeAlignItem(node, child) == FBYGAlignCenter) {
    child->setLayoutPosition(
        (node->getLayout().measuredDimensions[dim[crossAxis]] -
         child->getLayout().measuredDimensions[dim[crossAxis]]) /
            2.0f,
        leading[crossAxis]);
  } else if (
      !child->isLeadingPositionDefined(crossAxis) &&
      ((FBYGNodeAlignItem(node, child) == FBYGAlignFlexEnd) ^
       (node->getStyle().flexWrap() == FBYGWrapWrapReverse))) {
    child->setLayoutPosition(
        (node->getLayout().measuredDimensions[dim[crossAxis]] -
         child->getLayout().measuredDimensions[dim[crossAxis]]),
        leading[crossAxis]);
  } else if (
      node->getConfig()->isExperimentalFeatureEnabled(
          FBYGExperimentalFeatureAbsolutePercentageAgainstPaddingEdge) &&
      child->isLeadingPositionDefined(crossAxis)) {
    child->setLayoutPosition(
        child->getLeadingPosition(
                 crossAxis,
                 node->getLayout().measuredDimensions[dim[crossAxis]])
                .unwrap() +
            node->getLeadingBorder(crossAxis) +
            child
                ->getLeadingMargin(
                    crossAxis,
                    node->getLayout().measuredDimensions[dim[crossAxis]])
                .unwrap(),
        leading[crossAxis]);
  }
}

static void FBYGNodeWithMeasureFuncSetMeasuredDimensions(
    const FBYGNodeRef node,
    float availableWidth,
    float availableHeight,
    const FBYGMeasureMode widthMeasureMode,
    const FBYGMeasureMode heightMeasureMode,
    const float ownerWidth,
    const float ownerHeight,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    const LayoutPassReason reason) {
  FBYGAssertWithNode(
      node,
      node->hasMeasureFunc(),
      "Expected node to have custom measure function");

  if (widthMeasureMode == FBYGMeasureModeUndefined) {
    availableWidth = FBYGUndefined;
  }
  if (heightMeasureMode == FBYGMeasureModeUndefined) {
    availableHeight = FBYGUndefined;
  }

  const auto& padding = node->getLayout().padding;
  const auto& border = node->getLayout().border;
  const float paddingAndBorderAxisRow = padding[FBYGEdgeLeft] +
      padding[FBYGEdgeRight] + border[FBYGEdgeLeft] + border[FBYGEdgeRight];
  const float paddingAndBorderAxisColumn = padding[FBYGEdgeTop] +
      padding[FBYGEdgeBottom] + border[FBYGEdgeTop] + border[FBYGEdgeBottom];

  // We want to make sure we don't call measure with negative size
  const float innerWidth = FBYGFloatIsUndefined(availableWidth)
      ? availableWidth
      : FBYGFloatMax(0, availableWidth - paddingAndBorderAxisRow);
  const float innerHeight = FBYGFloatIsUndefined(availableHeight)
      ? availableHeight
      : FBYGFloatMax(0, availableHeight - paddingAndBorderAxisColumn);

  if (widthMeasureMode == FBYGMeasureModeExactly &&
      heightMeasureMode == FBYGMeasureModeExactly) {
    // Don't bother sizing the text if both dimensions are already defined.
    node->setLayoutMeasuredDimension(
        FBYGNodeBoundAxis(
            node, FBYGFlexDirectionRow, availableWidth, ownerWidth, ownerWidth),
        FBYGDimensionWidth);
    node->setLayoutMeasuredDimension(
        FBYGNodeBoundAxis(
            node,
            FBYGFlexDirectionColumn,
            availableHeight,
            ownerHeight,
            ownerWidth),
        FBYGDimensionHeight);
  } else {
    Event::publish<Event::MeasureCallbackStart>(node);

    // Measure the text under the current constraints.
    const FBYGSize measuredSize = node->measure(
        innerWidth,
        widthMeasureMode,
        innerHeight,
        heightMeasureMode,
        layoutContext);

    layoutMarkerData.measureCallbacks += 1;
    layoutMarkerData.measureCallbackReasonsCount[static_cast<size_t>(reason)] +=
        1;

    Event::publish<Event::MeasureCallbackEnd>(
        node,
        {layoutContext,
         innerWidth,
         widthMeasureMode,
         innerHeight,
         heightMeasureMode,
         measuredSize.width,
         measuredSize.height,
         reason});

    node->setLayoutMeasuredDimension(
        FBYGNodeBoundAxis(
            node,
            FBYGFlexDirectionRow,
            (widthMeasureMode == FBYGMeasureModeUndefined ||
             widthMeasureMode == FBYGMeasureModeAtMost)
                ? measuredSize.width + paddingAndBorderAxisRow
                : availableWidth,
            ownerWidth,
            ownerWidth),
        FBYGDimensionWidth);

    node->setLayoutMeasuredDimension(
        FBYGNodeBoundAxis(
            node,
            FBYGFlexDirectionColumn,
            (heightMeasureMode == FBYGMeasureModeUndefined ||
             heightMeasureMode == FBYGMeasureModeAtMost)
                ? measuredSize.height + paddingAndBorderAxisColumn
                : availableHeight,
            ownerHeight,
            ownerWidth),
        FBYGDimensionHeight);
  }
}

// For nodes with no children, use the available values if they were provided,
// or the minimum size as indicated by the padding and border sizes.
static void FBYGNodeEmptyContainerSetMeasuredDimensions(
    const FBYGNodeRef node,
    const float availableWidth,
    const float availableHeight,
    const FBYGMeasureMode widthMeasureMode,
    const FBYGMeasureMode heightMeasureMode,
    const float ownerWidth,
    const float ownerHeight) {
  const auto& padding = node->getLayout().padding;
  const auto& border = node->getLayout().border;

  float width = availableWidth;
  if (widthMeasureMode == FBYGMeasureModeUndefined ||
      widthMeasureMode == FBYGMeasureModeAtMost) {
    width = padding[FBYGEdgeLeft] + padding[FBYGEdgeRight] + border[FBYGEdgeLeft] +
        border[FBYGEdgeRight];
  }
  node->setLayoutMeasuredDimension(
      FBYGNodeBoundAxis(node, FBYGFlexDirectionRow, width, ownerWidth, ownerWidth),
      FBYGDimensionWidth);

  float height = availableHeight;
  if (heightMeasureMode == FBYGMeasureModeUndefined ||
      heightMeasureMode == FBYGMeasureModeAtMost) {
    height = padding[FBYGEdgeTop] + padding[FBYGEdgeBottom] + border[FBYGEdgeTop] +
        border[FBYGEdgeBottom];
  }
  node->setLayoutMeasuredDimension(
      FBYGNodeBoundAxis(
          node, FBYGFlexDirectionColumn, height, ownerHeight, ownerWidth),
      FBYGDimensionHeight);
}

static bool FBYGNodeFixedSizeSetMeasuredDimensions(
    const FBYGNodeRef node,
    const float availableWidth,
    const float availableHeight,
    const FBYGMeasureMode widthMeasureMode,
    const FBYGMeasureMode heightMeasureMode,
    const float ownerWidth,
    const float ownerHeight) {
  if ((!FBYGFloatIsUndefined(availableWidth) &&
       widthMeasureMode == FBYGMeasureModeAtMost && availableWidth <= 0.0f) ||
      (!FBYGFloatIsUndefined(availableHeight) &&
       heightMeasureMode == FBYGMeasureModeAtMost && availableHeight <= 0.0f) ||
      (widthMeasureMode == FBYGMeasureModeExactly &&
       heightMeasureMode == FBYGMeasureModeExactly)) {
    node->setLayoutMeasuredDimension(
        FBYGNodeBoundAxis(
            node,
            FBYGFlexDirectionRow,
            FBYGFloatIsUndefined(availableWidth) ||
                    (widthMeasureMode == FBYGMeasureModeAtMost &&
                     availableWidth < 0.0f)
                ? 0.0f
                : availableWidth,
            ownerWidth,
            ownerWidth),
        FBYGDimensionWidth);

    node->setLayoutMeasuredDimension(
        FBYGNodeBoundAxis(
            node,
            FBYGFlexDirectionColumn,
            FBYGFloatIsUndefined(availableHeight) ||
                    (heightMeasureMode == FBYGMeasureModeAtMost &&
                     availableHeight < 0.0f)
                ? 0.0f
                : availableHeight,
            ownerHeight,
            ownerWidth),
        FBYGDimensionHeight);
    return true;
  }

  return false;
}

static void FBYGZeroOutLayoutRecursively(
    const FBYGNodeRef node,
    void* layoutContext) {
  node->getLayout() = {};
  node->setLayoutDimension(0, 0);
  node->setLayoutDimension(0, 1);
  node->setHasNewLayout(true);

  node->iterChildrenAfterCloningIfNeeded(
      FBYGZeroOutLayoutRecursively, layoutContext);
}

static float FBYGNodeCalculateAvailableInnerDim(
    const FBYGNodeConstRef node,
    const FBYGDimension dimension,
    const float availableDim,
    const float paddingAndBorder,
    const float ownerDim) {
  float availableInnerDim = availableDim - paddingAndBorder;
  // Max dimension overrides predefined dimension value; Min dimension in turn
  // overrides both of the above
  if (!FBYGFloatIsUndefined(availableInnerDim)) {
    // We want to make sure our available height does not violate min and max
    // constraints
    const FBYGFloatOptional minDimensionOptional =
        FBYGResolveValue(node->getStyle().minDimensions()[dimension], ownerDim);
    const float minInnerDim = minDimensionOptional.isUndefined()
        ? 0.0f
        : minDimensionOptional.unwrap() - paddingAndBorder;

    const FBYGFloatOptional maxDimensionOptional =
        FBYGResolveValue(node->getStyle().maxDimensions()[dimension], ownerDim);

    const float maxInnerDim = maxDimensionOptional.isUndefined()
        ? FLT_MAX
        : maxDimensionOptional.unwrap() - paddingAndBorder;
    availableInnerDim =
        FBYGFloatMax(FBYGFloatMin(availableInnerDim, maxInnerDim), minInnerDim);
  }

  return availableInnerDim;
}

static float FBYGNodeComputeFlexBasisForChildren(
    const FBYGNodeRef node,
    const float availableInnerWidth,
    const float availableInnerHeight,
    FBYGMeasureMode widthMeasureMode,
    FBYGMeasureMode heightMeasureMode,
    FBYGDirection direction,
    FBYGFlexDirection mainAxis,
    const FBYGConfigRef config,
    bool performLayout,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    const uint32_t depth,
    const uint32_t generationCount) {
  float totalOuterFlexBasis = 0.0f;
  FBYGNodeRef singleFlexChild = nullptr;
  const FBYGVector& children = node->getChildren();
  FBYGMeasureMode measureModeMainDim =
      FBYGFlexDirectionIsRow(mainAxis) ? widthMeasureMode : heightMeasureMode;
  // If there is only one child with flexGrow + flexShrink it means we can set
  // the computedFlexBasis to 0 instead of measuring and shrinking / flexing the
  // child to exactly match the remaining space
  if (measureModeMainDim == FBYGMeasureModeExactly) {
    for (auto child : children) {
      if (child->isNodeFlexible()) {
        if (singleFlexChild != nullptr ||
            FBYGFloatsEqual(child->resolveFlexGrow(), 0.0f) ||
            FBYGFloatsEqual(child->resolveFlexShrink(), 0.0f)) {
          // There is already a flexible child, or this flexible child doesn't
          // have flexGrow and flexShrink, abort
          singleFlexChild = nullptr;
          break;
        } else {
          singleFlexChild = child;
        }
      }
    }
  }

  for (auto child : children) {
    child->resolveDimension();
    if (child->getStyle().display() == FBYGDisplayNone) {
      FBYGZeroOutLayoutRecursively(child, layoutContext);
      child->setHasNewLayout(true);
      child->setDirty(false);
      continue;
    }
    if (performLayout) {
      // Set the initial position (relative to the owner).
      const FBYGDirection childDirection = child->resolveDirection(direction);
      const float mainDim = FBYGFlexDirectionIsRow(mainAxis)
          ? availableInnerWidth
          : availableInnerHeight;
      const float crossDim = FBYGFlexDirectionIsRow(mainAxis)
          ? availableInnerHeight
          : availableInnerWidth;
      child->setPosition(
          childDirection, mainDim, crossDim, availableInnerWidth);
    }

    if (child->getStyle().positionType() == FBYGPositionTypeAbsolute) {
      continue;
    }
    if (child == singleFlexChild) {
      child->setLayoutComputedFlexBasisGeneration(generationCount);
      child->setLayoutComputedFlexBasis(FBYGFloatOptional(0));
    } else {
      FBYGNodeComputeFlexBasisForChild(
          node,
          child,
          availableInnerWidth,
          widthMeasureMode,
          availableInnerHeight,
          availableInnerWidth,
          availableInnerHeight,
          heightMeasureMode,
          direction,
          config,
          layoutMarkerData,
          layoutContext,
          depth,
          generationCount);
    }

    totalOuterFlexBasis +=
        (child->getLayout().computedFlexBasis +
         child->getMarginForAxis(mainAxis, availableInnerWidth))
            .unwrap();
  }

  return totalOuterFlexBasis;
}

// This function assumes that all the children of node have their
// computedFlexBasis properly computed(To do this use
// FBYGNodeComputeFlexBasisForChildren function). This function calculates
// FBYGCollectFlexItemsRowMeasurement
static FBYGCollectFlexItemsRowValues FBYGCalculateCollectFlexItemsRowValues(
    const FBYGNodeRef& node,
    const FBYGDirection ownerDirection,
    const float mainAxisownerSize,
    const float availableInnerWidth,
    const float availableInnerMainDim,
    const uint32_t startOfLineIndex,
    const uint32_t lineCount) {
  FBYGCollectFlexItemsRowValues flexAlgoRowMeasurement = {};
  flexAlgoRowMeasurement.relativeChildren.reserve(node->getChildren().size());

  float sizeConsumedOnCurrentLineIncludingMinConstraint = 0;
  const FBYGFlexDirection mainAxis = FBYGResolveFlexDirection(
      node->getStyle().flexDirection(), node->resolveDirection(ownerDirection));
  const bool isNodeFlexWrap = node->getStyle().flexWrap() != FBYGWrapNoWrap;
  const float gap = node->getGapForAxis(mainAxis, availableInnerWidth).unwrap();

  // Add items to the current line until it's full or we run out of items.
  uint32_t endOfLineIndex = startOfLineIndex;
  for (; endOfLineIndex < node->getChildren().size(); endOfLineIndex++) {
    const FBYGNodeRef child = node->getChild(endOfLineIndex);
    if (child->getStyle().display() == FBYGDisplayNone ||
        child->getStyle().positionType() == FBYGPositionTypeAbsolute) {
      continue;
    }

    const bool isFirstElementInLine = (endOfLineIndex - startOfLineIndex) == 0;

    child->setLineIndex(lineCount);
    const float childMarginMainAxis =
        child->getMarginForAxis(mainAxis, availableInnerWidth).unwrap();
    const float childLeadingGapMainAxis = isFirstElementInLine ? 0.0f : gap;
    const float flexBasisWithMinAndMaxConstraints =
        FBYGNodeBoundAxisWithinMinAndMax(
            child,
            mainAxis,
            child->getLayout().computedFlexBasis,
            mainAxisownerSize)
            .unwrap();

    // If this is a multi-line flow and this item pushes us over the available
    // size, we've hit the end of the current line. Break out of the loop and
    // lay out the current line.
    if (sizeConsumedOnCurrentLineIncludingMinConstraint +
                flexBasisWithMinAndMaxConstraints + childMarginMainAxis +
                childLeadingGapMainAxis >
            availableInnerMainDim &&
        isNodeFlexWrap && flexAlgoRowMeasurement.itemsOnLine > 0) {
      break;
    }

    sizeConsumedOnCurrentLineIncludingMinConstraint +=
        flexBasisWithMinAndMaxConstraints + childMarginMainAxis +
        childLeadingGapMainAxis;
    flexAlgoRowMeasurement.sizeConsumedOnCurrentLine +=
        flexBasisWithMinAndMaxConstraints + childMarginMainAxis +
        childLeadingGapMainAxis;
    flexAlgoRowMeasurement.itemsOnLine++;

    if (child->isNodeFlexible()) {
      flexAlgoRowMeasurement.totalFlexGrowFactors += child->resolveFlexGrow();

      // Unlike the grow factor, the shrink factor is scaled relative to the
      // child dimension.
      flexAlgoRowMeasurement.totalFlexShrinkScaledFactors +=
          -child->resolveFlexShrink() *
          child->getLayout().computedFlexBasis.unwrap();
    }

    flexAlgoRowMeasurement.relativeChildren.push_back(child);
  }

  // The total flex factor needs to be floored to 1.
  if (flexAlgoRowMeasurement.totalFlexGrowFactors > 0 &&
      flexAlgoRowMeasurement.totalFlexGrowFactors < 1) {
    flexAlgoRowMeasurement.totalFlexGrowFactors = 1;
  }

  // The total flex shrink factor needs to be floored to 1.
  if (flexAlgoRowMeasurement.totalFlexShrinkScaledFactors > 0 &&
      flexAlgoRowMeasurement.totalFlexShrinkScaledFactors < 1) {
    flexAlgoRowMeasurement.totalFlexShrinkScaledFactors = 1;
  }
  flexAlgoRowMeasurement.endOfLineIndex = endOfLineIndex;
  return flexAlgoRowMeasurement;
}

// It distributes the free space to the flexible items and ensures that the size
// of the flex items abide the min and max constraints. At the end of this
// function the child nodes would have proper size. Prior using this function
// please ensure that FBYGDistributeFreeSpaceFirstPass is called.
static float FBYGDistributeFreeSpaceSecondPass(
    FBYGCollectFlexItemsRowValues& collectedFlexItemsValues,
    const FBYGNodeRef node,
    const FBYGFlexDirection mainAxis,
    const FBYGFlexDirection crossAxis,
    const float mainAxisownerSize,
    const float availableInnerMainDim,
    const float availableInnerCrossDim,
    const float availableInnerWidth,
    const float availableInnerHeight,
    const bool mainAxisOverflows,
    const FBYGMeasureMode measureModeCrossDim,
    const bool performLayout,
    const FBYGConfigRef config,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    const uint32_t depth,
    const uint32_t generationCount) {
  float childFlexBasis = 0;
  float flexShrinkScaledFactor = 0;
  float flexGrowFactor = 0;
  float deltaFreeSpace = 0;
  const bool isMainAxisRow = FBYGFlexDirectionIsRow(mainAxis);
  const bool isNodeFlexWrap = node->getStyle().flexWrap() != FBYGWrapNoWrap;

  for (auto currentRelativeChild : collectedFlexItemsValues.relativeChildren) {
    childFlexBasis = FBYGNodeBoundAxisWithinMinAndMax(
                         currentRelativeChild,
                         mainAxis,
                         currentRelativeChild->getLayout().computedFlexBasis,
                         mainAxisownerSize)
                         .unwrap();
    float updatedMainSize = childFlexBasis;

    if (!FBYGFloatIsUndefined(collectedFlexItemsValues.remainingFreeSpace) &&
        collectedFlexItemsValues.remainingFreeSpace < 0) {
      flexShrinkScaledFactor =
          -currentRelativeChild->resolveFlexShrink() * childFlexBasis;
      // Is this child able to shrink?
      if (flexShrinkScaledFactor != 0) {
        float childSize;

        if (!FBYGFloatIsUndefined(
                collectedFlexItemsValues.totalFlexShrinkScaledFactors) &&
            collectedFlexItemsValues.totalFlexShrinkScaledFactors == 0) {
          childSize = childFlexBasis + flexShrinkScaledFactor;
        } else {
          childSize = childFlexBasis +
              (collectedFlexItemsValues.remainingFreeSpace /
               collectedFlexItemsValues.totalFlexShrinkScaledFactors) *
                  flexShrinkScaledFactor;
        }

        updatedMainSize = FBYGNodeBoundAxis(
            currentRelativeChild,
            mainAxis,
            childSize,
            availableInnerMainDim,
            availableInnerWidth);
      }
    } else if (
        !FBYGFloatIsUndefined(collectedFlexItemsValues.remainingFreeSpace) &&
        collectedFlexItemsValues.remainingFreeSpace > 0) {
      flexGrowFactor = currentRelativeChild->resolveFlexGrow();

      // Is this child able to grow?
      if (!FBYGFloatIsUndefined(flexGrowFactor) && flexGrowFactor != 0) {
        updatedMainSize = FBYGNodeBoundAxis(
            currentRelativeChild,
            mainAxis,
            childFlexBasis +
                collectedFlexItemsValues.remainingFreeSpace /
                    collectedFlexItemsValues.totalFlexGrowFactors *
                    flexGrowFactor,
            availableInnerMainDim,
            availableInnerWidth);
      }
    }

    deltaFreeSpace += updatedMainSize - childFlexBasis;

    const float marginMain =
        currentRelativeChild->getMarginForAxis(mainAxis, availableInnerWidth)
            .unwrap();
    const float marginCross =
        currentRelativeChild->getMarginForAxis(crossAxis, availableInnerWidth)
            .unwrap();

    float childCrossSize;
    float childMainSize = updatedMainSize + marginMain;
    FBYGMeasureMode childCrossMeasureMode;
    FBYGMeasureMode childMainMeasureMode = FBYGMeasureModeExactly;

    const auto& childStyle = currentRelativeChild->getStyle();
    if (!childStyle.aspectRatio().isUndefined()) {
      childCrossSize = isMainAxisRow
          ? (childMainSize - marginMain) / childStyle.aspectRatio().unwrap()
          : (childMainSize - marginMain) * childStyle.aspectRatio().unwrap();
      childCrossMeasureMode = FBYGMeasureModeExactly;

      childCrossSize += marginCross;
    } else if (
        !FBYGFloatIsUndefined(availableInnerCrossDim) &&
        !FBYGNodeIsStyleDimDefined(
            currentRelativeChild, crossAxis, availableInnerCrossDim) &&
        measureModeCrossDim == FBYGMeasureModeExactly &&
        !(isNodeFlexWrap && mainAxisOverflows) &&
        FBYGNodeAlignItem(node, currentRelativeChild) == FBYGAlignStretch &&
        currentRelativeChild->marginLeadingValue(crossAxis).unit !=
            FBYGUnitAuto &&
        currentRelativeChild->marginTrailingValue(crossAxis).unit !=
            FBYGUnitAuto) {
      childCrossSize = availableInnerCrossDim;
      childCrossMeasureMode = FBYGMeasureModeExactly;
    } else if (!FBYGNodeIsStyleDimDefined(
                   currentRelativeChild, crossAxis, availableInnerCrossDim)) {
      childCrossSize = availableInnerCrossDim;
      childCrossMeasureMode = FBYGFloatIsUndefined(childCrossSize)
          ? FBYGMeasureModeUndefined
          : FBYGMeasureModeAtMost;
    } else {
      childCrossSize =
          FBYGResolveValue(
              currentRelativeChild->getResolvedDimension(dim[crossAxis]),
              availableInnerCrossDim)
              .unwrap() +
          marginCross;
      const bool isLoosePercentageMeasurement =
          currentRelativeChild->getResolvedDimension(dim[crossAxis]).unit ==
              FBYGUnitPercent &&
          measureModeCrossDim != FBYGMeasureModeExactly;
      childCrossMeasureMode =
          FBYGFloatIsUndefined(childCrossSize) || isLoosePercentageMeasurement
          ? FBYGMeasureModeUndefined
          : FBYGMeasureModeExactly;
    }

    FBYGConstrainMaxSizeForMode(
        currentRelativeChild,
        mainAxis,
        availableInnerMainDim,
        availableInnerWidth,
        &childMainMeasureMode,
        &childMainSize);
    FBYGConstrainMaxSizeForMode(
        currentRelativeChild,
        crossAxis,
        availableInnerCrossDim,
        availableInnerWidth,
        &childCrossMeasureMode,
        &childCrossSize);

    const bool requiresStretchLayout =
        !FBYGNodeIsStyleDimDefined(
            currentRelativeChild, crossAxis, availableInnerCrossDim) &&
        FBYGNodeAlignItem(node, currentRelativeChild) == FBYGAlignStretch &&
        currentRelativeChild->marginLeadingValue(crossAxis).unit !=
            FBYGUnitAuto &&
        currentRelativeChild->marginTrailingValue(crossAxis).unit != FBYGUnitAuto;

    const float childWidth = isMainAxisRow ? childMainSize : childCrossSize;
    const float childHeight = !isMainAxisRow ? childMainSize : childCrossSize;

    const FBYGMeasureMode childWidthMeasureMode =
        isMainAxisRow ? childMainMeasureMode : childCrossMeasureMode;
    const FBYGMeasureMode childHeightMeasureMode =
        !isMainAxisRow ? childMainMeasureMode : childCrossMeasureMode;

    const bool isLayoutPass = performLayout && !requiresStretchLayout;
    // Recursively call the layout algorithm for this child with the updated
    // main size.
    FBYGLayoutNodeInternal(
        currentRelativeChild,
        childWidth,
        childHeight,
        node->getLayout().direction(),
        childWidthMeasureMode,
        childHeightMeasureMode,
        availableInnerWidth,
        availableInnerHeight,
        isLayoutPass,
        isLayoutPass ? LayoutPassReason::kFlexLayout
                     : LayoutPassReason::kFlexMeasure,
        config,
        layoutMarkerData,
        layoutContext,
        depth,
        generationCount);
    node->setLayoutHadOverflow(
        node->getLayout().hadOverflow() ||
        currentRelativeChild->getLayout().hadOverflow());
  }
  return deltaFreeSpace;
}

// It distributes the free space to the flexible items.For those flexible items
// whose min and max constraints are triggered, those flex item's clamped size
// is removed from the remaingfreespace.
static void FBYGDistributeFreeSpaceFirstPass(
    FBYGCollectFlexItemsRowValues& collectedFlexItemsValues,
    const FBYGFlexDirection mainAxis,
    const float mainAxisownerSize,
    const float availableInnerMainDim,
    const float availableInnerWidth) {
  float flexShrinkScaledFactor = 0;
  float flexGrowFactor = 0;
  float baseMainSize = 0;
  float boundMainSize = 0;
  float deltaFreeSpace = 0;

  for (auto currentRelativeChild : collectedFlexItemsValues.relativeChildren) {
    float childFlexBasis =
        FBYGNodeBoundAxisWithinMinAndMax(
            currentRelativeChild,
            mainAxis,
            currentRelativeChild->getLayout().computedFlexBasis,
            mainAxisownerSize)
            .unwrap();

    if (collectedFlexItemsValues.remainingFreeSpace < 0) {
      flexShrinkScaledFactor =
          -currentRelativeChild->resolveFlexShrink() * childFlexBasis;

      // Is this child able to shrink?
      if (!FBYGFloatIsUndefined(flexShrinkScaledFactor) &&
          flexShrinkScaledFactor != 0) {
        baseMainSize = childFlexBasis +
            collectedFlexItemsValues.remainingFreeSpace /
                collectedFlexItemsValues.totalFlexShrinkScaledFactors *
                flexShrinkScaledFactor;
        boundMainSize = FBYGNodeBoundAxis(
            currentRelativeChild,
            mainAxis,
            baseMainSize,
            availableInnerMainDim,
            availableInnerWidth);
        if (!FBYGFloatIsUndefined(baseMainSize) &&
            !FBYGFloatIsUndefined(boundMainSize) &&
            baseMainSize != boundMainSize) {
          // By excluding this item's size and flex factor from remaining, this
          // item's min/max constraints should also trigger in the second pass
          // resulting in the item's size calculation being identical in the
          // first and second passes.
          deltaFreeSpace += boundMainSize - childFlexBasis;
          collectedFlexItemsValues.totalFlexShrinkScaledFactors -=
              (-currentRelativeChild->resolveFlexShrink() *
               currentRelativeChild->getLayout().computedFlexBasis.unwrap());
        }
      }
    } else if (
        !FBYGFloatIsUndefined(collectedFlexItemsValues.remainingFreeSpace) &&
        collectedFlexItemsValues.remainingFreeSpace > 0) {
      flexGrowFactor = currentRelativeChild->resolveFlexGrow();

      // Is this child able to grow?
      if (!FBYGFloatIsUndefined(flexGrowFactor) && flexGrowFactor != 0) {
        baseMainSize = childFlexBasis +
            collectedFlexItemsValues.remainingFreeSpace /
                collectedFlexItemsValues.totalFlexGrowFactors * flexGrowFactor;
        boundMainSize = FBYGNodeBoundAxis(
            currentRelativeChild,
            mainAxis,
            baseMainSize,
            availableInnerMainDim,
            availableInnerWidth);

        if (!FBYGFloatIsUndefined(baseMainSize) &&
            !FBYGFloatIsUndefined(boundMainSize) &&
            baseMainSize != boundMainSize) {
          // By excluding this item's size and flex factor from remaining, this
          // item's min/max constraints should also trigger in the second pass
          // resulting in the item's size calculation being identical in the
          // first and second passes.
          deltaFreeSpace += boundMainSize - childFlexBasis;
          collectedFlexItemsValues.totalFlexGrowFactors -= flexGrowFactor;
        }
      }
    }
  }
  collectedFlexItemsValues.remainingFreeSpace -= deltaFreeSpace;
}

// Do two passes over the flex items to figure out how to distribute the
// remaining space.
//
// The first pass finds the items whose min/max constraints trigger, freezes
// them at those sizes, and excludes those sizes from the remaining space.
//
// The second pass sets the size of each flexible item. It distributes the
// remaining space amongst the items whose min/max constraints didn't trigger in
// the first pass. For the other items, it sets their sizes by forcing their
// min/max constraints to trigger again.
//
// This two pass approach for resolving min/max constraints deviates from the
// spec. The spec
// (https://www.w3.org/TR/CSS-flexbox-1/#resolve-flexible-lengths) describes a
// process that needs to be repeated a variable number of times. The algorithm
// implemented here won't handle all cases but it was simpler to implement and
// it mitigates performance concerns because we know exactly how many passes
// it'll do.
//
// At the end of this function the child nodes would have the proper size
// assigned to them.
//
static void FBYGResolveFlexibleLength(
    const FBYGNodeRef node,
    FBYGCollectFlexItemsRowValues& collectedFlexItemsValues,
    const FBYGFlexDirection mainAxis,
    const FBYGFlexDirection crossAxis,
    const float mainAxisownerSize,
    const float availableInnerMainDim,
    const float availableInnerCrossDim,
    const float availableInnerWidth,
    const float availableInnerHeight,
    const bool mainAxisOverflows,
    const FBYGMeasureMode measureModeCrossDim,
    const bool performLayout,
    const FBYGConfigRef config,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    const uint32_t depth,
    const uint32_t generationCount) {
  const float originalFreeSpace = collectedFlexItemsValues.remainingFreeSpace;
  // First pass: detect the flex items whose min/max constraints trigger
  FBYGDistributeFreeSpaceFirstPass(
      collectedFlexItemsValues,
      mainAxis,
      mainAxisownerSize,
      availableInnerMainDim,
      availableInnerWidth);

  // Second pass: resolve the sizes of the flexible items
  const float distributedFreeSpace = FBYGDistributeFreeSpaceSecondPass(
      collectedFlexItemsValues,
      node,
      mainAxis,
      crossAxis,
      mainAxisownerSize,
      availableInnerMainDim,
      availableInnerCrossDim,
      availableInnerWidth,
      availableInnerHeight,
      mainAxisOverflows,
      measureModeCrossDim,
      performLayout,
      config,
      layoutMarkerData,
      layoutContext,
      depth,
      generationCount);

  collectedFlexItemsValues.remainingFreeSpace =
      originalFreeSpace - distributedFreeSpace;
}

static void FBYGJustifyMainAxis(
    const FBYGNodeRef node,
    FBYGCollectFlexItemsRowValues& collectedFlexItemsValues,
    const uint32_t startOfLineIndex,
    const FBYGFlexDirection mainAxis,
    const FBYGFlexDirection crossAxis,
    const FBYGMeasureMode measureModeMainDim,
    const FBYGMeasureMode measureModeCrossDim,
    const float mainAxisownerSize,
    const float ownerWidth,
    const float availableInnerMainDim,
    const float availableInnerCrossDim,
    const float availableInnerWidth,
    const bool performLayout,
    void* const layoutContext) {
  const auto& style = node->getStyle();
  const float leadingPaddingAndBorderMain =
      node->getLeadingPaddingAndBorder(mainAxis, ownerWidth).unwrap();
  const float trailingPaddingAndBorderMain =
      node->getTrailingPaddingAndBorder(mainAxis, ownerWidth).unwrap();
  const float gap = node->getGapForAxis(mainAxis, ownerWidth).unwrap();
  // If we are using "at most" rules in the main axis, make sure that
  // remainingFreeSpace is 0 when min main dimension is not given
  if (measureModeMainDim == FBYGMeasureModeAtMost &&
      collectedFlexItemsValues.remainingFreeSpace > 0) {
    if (!style.minDimensions()[dim[mainAxis]].isUndefined() &&
        !FBYGResolveValue(style.minDimensions()[dim[mainAxis]], mainAxisownerSize)
             .isUndefined()) {
      // This condition makes sure that if the size of main dimension(after
      // considering child nodes main dim, leading and trailing padding etc)
      // falls below min dimension, then the remainingFreeSpace is reassigned
      // considering the min dimension

      // `minAvailableMainDim` denotes minimum available space in which child
      // can be laid out, it will exclude space consumed by padding and border.
      const float minAvailableMainDim =
          FBYGResolveValue(
              style.minDimensions()[dim[mainAxis]], mainAxisownerSize)
              .unwrap() -
          leadingPaddingAndBorderMain - trailingPaddingAndBorderMain;
      const float occupiedSpaceByChildNodes =
          availableInnerMainDim - collectedFlexItemsValues.remainingFreeSpace;
      collectedFlexItemsValues.remainingFreeSpace =
          FBYGFloatMax(0, minAvailableMainDim - occupiedSpaceByChildNodes);
    } else {
      collectedFlexItemsValues.remainingFreeSpace = 0;
    }
  }

  int numberOfAutoMarginsOnCurrentLine = 0;
  for (uint32_t i = startOfLineIndex;
       i < collectedFlexItemsValues.endOfLineIndex;
       i++) {
    const FBYGNodeRef child = node->getChild(i);
    if (child->getStyle().positionType() != FBYGPositionTypeAbsolute) {
      if (child->marginLeadingValue(mainAxis).unit == FBYGUnitAuto) {
        numberOfAutoMarginsOnCurrentLine++;
      }
      if (child->marginTrailingValue(mainAxis).unit == FBYGUnitAuto) {
        numberOfAutoMarginsOnCurrentLine++;
      }
    }
  }

  // In order to position the elements in the main axis, we have two controls.
  // The space between the beginning and the first element and the space between
  // each two elements.
  float leadingMainDim = 0;
  float betweenMainDim = gap;
  const FBYGJustify justifyContent = node->getStyle().justifyContent();

  if (numberOfAutoMarginsOnCurrentLine == 0) {
    switch (justifyContent) {
      case FBYGJustifyCenter:
        leadingMainDim = collectedFlexItemsValues.remainingFreeSpace / 2;
        break;
      case FBYGJustifyFlexEnd:
        leadingMainDim = collectedFlexItemsValues.remainingFreeSpace;
        break;
      case FBYGJustifySpaceBetween:
        if (collectedFlexItemsValues.itemsOnLine > 1) {
          betweenMainDim +=
              FBYGFloatMax(collectedFlexItemsValues.remainingFreeSpace, 0) /
              (collectedFlexItemsValues.itemsOnLine - 1);
        }
        break;
      case FBYGJustifySpaceEvenly:
        // Space is distributed evenly across all elements
        leadingMainDim = collectedFlexItemsValues.remainingFreeSpace /
            (collectedFlexItemsValues.itemsOnLine + 1);
        betweenMainDim += leadingMainDim;
        break;
      case FBYGJustifySpaceAround:
        // Space on the edges is half of the space between elements
        leadingMainDim = 0.5f * collectedFlexItemsValues.remainingFreeSpace /
            collectedFlexItemsValues.itemsOnLine;
        betweenMainDim += leadingMainDim * 2;
        break;
      case FBYGJustifyFlexStart:
        break;
    }
  }

  collectedFlexItemsValues.mainDim =
      leadingPaddingAndBorderMain + leadingMainDim;
  collectedFlexItemsValues.crossDim = 0;

  float maxAscentForCurrentLine = 0;
  float maxDescentForCurrentLine = 0;
  bool isNodeBaselineLayout = FBYGIsBaselineLayout(node);
  for (uint32_t i = startOfLineIndex;
       i < collectedFlexItemsValues.endOfLineIndex;
       i++) {
    const FBYGNodeRef child = node->getChild(i);
    const FBYGStyle& childStyle = child->getStyle();
    const FBYGLayout childLayout = child->getLayout();
    const bool isLastChild = i == collectedFlexItemsValues.endOfLineIndex - 1;
    // remove the gap if it is the last element of the line
    if (isLastChild) {
      betweenMainDim -= gap;
    }
    if (childStyle.display() == FBYGDisplayNone) {
      continue;
    }
    if (childStyle.positionType() == FBYGPositionTypeAbsolute &&
        child->isLeadingPositionDefined(mainAxis)) {
      if (performLayout) {
        // In case the child is position absolute and has left/top being
        // defined, we override the position to whatever the user said (and
        // margin/border).
        child->setLayoutPosition(
            child->getLeadingPosition(mainAxis, availableInnerMainDim)
                    .unwrap() +
                node->getLeadingBorder(mainAxis) +
                child->getLeadingMargin(mainAxis, availableInnerWidth).unwrap(),
            pos[mainAxis]);
      }
    } else {
      // Now that we placed the element, we need to update the variables.
      // We need to do that only for relative elements. Absolute elements do not
      // take part in that phase.
      if (childStyle.positionType() != FBYGPositionTypeAbsolute) {
        if (child->marginLeadingValue(mainAxis).unit == FBYGUnitAuto) {
          collectedFlexItemsValues.mainDim +=
              collectedFlexItemsValues.remainingFreeSpace /
              numberOfAutoMarginsOnCurrentLine;
        }

        if (performLayout) {
          child->setLayoutPosition(
              childLayout.position[pos[mainAxis]] +
                  collectedFlexItemsValues.mainDim,
              pos[mainAxis]);
        }

        if (child->marginTrailingValue(mainAxis).unit == FBYGUnitAuto) {
          collectedFlexItemsValues.mainDim +=
              collectedFlexItemsValues.remainingFreeSpace /
              numberOfAutoMarginsOnCurrentLine;
        }
        bool canSkipFlex =
            !performLayout && measureModeCrossDim == FBYGMeasureModeExactly;
        if (canSkipFlex) {
          // If we skipped the flex step, then we can't rely on the measuredDims
          // because they weren't computed. This means we can't call
          // FBYGNodeDimWithMargin.
          collectedFlexItemsValues.mainDim += betweenMainDim +
              child->getMarginForAxis(mainAxis, availableInnerWidth).unwrap() +
              childLayout.computedFlexBasis.unwrap();
          collectedFlexItemsValues.crossDim = availableInnerCrossDim;
        } else {
          // The main dimension is the sum of all the elements dimension plus
          // the spacing.
          collectedFlexItemsValues.mainDim += betweenMainDim +
              FBYGNodeDimWithMargin(child, mainAxis, availableInnerWidth);

          if (isNodeBaselineLayout) {
            // If the child is baseline aligned then the cross dimension is
            // calculated by adding maxAscent and maxDescent from the baseline.
            const float ascent = FBYGBaseline(child, layoutContext) +
                child
                    ->getLeadingMargin(
                        FBYGFlexDirectionColumn, availableInnerWidth)
                    .unwrap();
            const float descent =
                child->getLayout().measuredDimensions[FBYGDimensionHeight] +
                child
                    ->getMarginForAxis(
                        FBYGFlexDirectionColumn, availableInnerWidth)
                    .unwrap() -
                ascent;

            maxAscentForCurrentLine =
                FBYGFloatMax(maxAscentForCurrentLine, ascent);
            maxDescentForCurrentLine =
                FBYGFloatMax(maxDescentForCurrentLine, descent);
          } else {
            // The cross dimension is the max of the elements dimension since
            // there can only be one element in that cross dimension in the case
            // when the items are not baseline aligned
            collectedFlexItemsValues.crossDim = FBYGFloatMax(
                collectedFlexItemsValues.crossDim,
                FBYGNodeDimWithMargin(child, crossAxis, availableInnerWidth));
          }
        }
      } else if (performLayout) {
        child->setLayoutPosition(
            childLayout.position[pos[mainAxis]] +
                node->getLeadingBorder(mainAxis) + leadingMainDim,
            pos[mainAxis]);
      }
    }
  }
  collectedFlexItemsValues.mainDim += trailingPaddingAndBorderMain;

  if (isNodeBaselineLayout) {
    collectedFlexItemsValues.crossDim =
        maxAscentForCurrentLine + maxDescentForCurrentLine;
  }
}

//
// This is the main routine that implements a subset of the flexbox layout
// algorithm described in the W3C CSS documentation:
// https://www.w3.org/TR/CSS3-flexbox/.
//
// Limitations of this algorithm, compared to the full standard:
//  * Display property is always assumed to be 'flex' except for Text nodes,
//    which are assumed to be 'inline-flex'.
//  * The 'zIndex' property (or any form of z ordering) is not supported. Nodes
//    are stacked in document order.
//  * The 'order' property is not supported. The order of flex items is always
//    defined by document order.
//  * The 'visibility' property is always assumed to be 'visible'. Values of
//    'collapse' and 'hidden' are not supported.
//  * There is no support for forced breaks.
//  * It does not support vertical inline directions (top-to-bottom or
//    bottom-to-top text).
//
// Deviations from standard:
//  * Section 4.5 of the spec indicates that all flex items have a default
//    minimum main size. For text blocks, for example, this is the width of the
//    widest word. Calculating the minimum width is expensive, so we forego it
//    and assume a default minimum main size of 0.
//  * Min/Max sizes in the main axis are not honored when resolving flexible
//    lengths.
//  * The spec indicates that the default value for 'flexDirection' is 'row',
//    but the algorithm below assumes a default of 'column'.
//
// Input parameters:
//    - node: current node to be sized and laid out
//    - availableWidth & availableHeight: available size to be used for sizing
//      the node or FBYGUndefined if the size is not available; interpretation
//      depends on layout flags
//    - ownerDirection: the inline (text) direction within the owner
//      (left-to-right or right-to-left)
//    - widthMeasureMode: indicates the sizing rules for the width (see below
//      for explanation)
//    - heightMeasureMode: indicates the sizing rules for the height (see below
//      for explanation)
//    - performLayout: specifies whether the caller is interested in just the
//      dimensions of the node or it requires the entire node and its subtree to
//      be laid out (with final positions)
//
// Details:
//    This routine is called recursively to lay out subtrees of flexbox
//    elements. It uses the information in node.style, which is treated as a
//    read-only input. It is responsible for setting the layout.direction and
//    layout.measuredDimensions fields for the input node as well as the
//    layout.position and layout.lineIndex fields for its child nodes. The
//    layout.measuredDimensions field includes any border or padding for the
//    node but does not include margins.
//
//    The spec describes four different layout modes: "fill available", "max
//    content", "min content", and "fit content". Of these, we don't use "min
//    content" because we don't support default minimum main sizes (see above
//    for details). Each of our measure modes maps to a layout mode from the
//    spec (https://www.w3.org/TR/CSS3-sizing/#terms):
//      - FBYGMeasureModeUndefined: max content
//      - FBYGMeasureModeExactly: fill available
//      - FBYGMeasureModeAtMost: fit content
//
//    When calling FBYGNodelayoutImpl and FBYGLayoutNodeInternal, if the caller
//    passes an available size of undefined then it must also pass a measure
//    mode of FBYGMeasureModeUndefined in that dimension.
//
static void FBYGNodelayoutImpl(
    const FBYGNodeRef node,
    const float availableWidth,
    const float availableHeight,
    const FBYGDirection ownerDirection,
    const FBYGMeasureMode widthMeasureMode,
    const FBYGMeasureMode heightMeasureMode,
    const float ownerWidth,
    const float ownerHeight,
    const bool performLayout,
    const FBYGConfigRef config,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    const uint32_t depth,
    const uint32_t generationCount,
    const LayoutPassReason reason) {
  FBYGAssertWithNode(
      node,
      FBYGFloatIsUndefined(availableWidth)
          ? widthMeasureMode == FBYGMeasureModeUndefined
          : true,
      "availableWidth is indefinite so widthMeasureMode must be "
      "FBYGMeasureModeUndefined");
  FBYGAssertWithNode(
      node,
      FBYGFloatIsUndefined(availableHeight)
          ? heightMeasureMode == FBYGMeasureModeUndefined
          : true,
      "availableHeight is indefinite so heightMeasureMode must be "
      "FBYGMeasureModeUndefined");

  (performLayout ? layoutMarkerData.layouts : layoutMarkerData.measures) += 1;

  // Set the resolved resolution in the node's layout.
  const FBYGDirection direction = node->resolveDirection(ownerDirection);
  node->setLayoutDirection(direction);

  const FBYGFlexDirection flexRowDirection =
      FBYGResolveFlexDirection(FBYGFlexDirectionRow, direction);
  const FBYGFlexDirection flexColumnDirection =
      FBYGResolveFlexDirection(FBYGFlexDirectionColumn, direction);

  const FBYGEdge startEdge =
      direction == FBYGDirectionLTR ? FBYGEdgeLeft : FBYGEdgeRight;
  const FBYGEdge endEdge = direction == FBYGDirectionLTR ? FBYGEdgeRight : FBYGEdgeLeft;

  const float marginRowLeading =
      node->getLeadingMargin(flexRowDirection, ownerWidth).unwrap();
  node->setLayoutMargin(marginRowLeading, startEdge);
  const float marginRowTrailing =
      node->getTrailingMargin(flexRowDirection, ownerWidth).unwrap();
  node->setLayoutMargin(marginRowTrailing, endEdge);
  const float marginColumnLeading =
      node->getLeadingMargin(flexColumnDirection, ownerWidth).unwrap();
  node->setLayoutMargin(marginColumnLeading, FBYGEdgeTop);
  const float marginColumnTrailing =
      node->getTrailingMargin(flexColumnDirection, ownerWidth).unwrap();
  node->setLayoutMargin(marginColumnTrailing, FBYGEdgeBottom);

  const float marginAxisRow = marginRowLeading + marginRowTrailing;
  const float marginAxisColumn = marginColumnLeading + marginColumnTrailing;

  node->setLayoutBorder(node->getLeadingBorder(flexRowDirection), startEdge);
  node->setLayoutBorder(node->getTrailingBorder(flexRowDirection), endEdge);
  node->setLayoutBorder(node->getLeadingBorder(flexColumnDirection), FBYGEdgeTop);
  node->setLayoutBorder(
      node->getTrailingBorder(flexColumnDirection), FBYGEdgeBottom);

  node->setLayoutPadding(
      node->getLeadingPadding(flexRowDirection, ownerWidth).unwrap(),
      startEdge);
  node->setLayoutPadding(
      node->getTrailingPadding(flexRowDirection, ownerWidth).unwrap(), endEdge);
  node->setLayoutPadding(
      node->getLeadingPadding(flexColumnDirection, ownerWidth).unwrap(),
      FBYGEdgeTop);
  node->setLayoutPadding(
      node->getTrailingPadding(flexColumnDirection, ownerWidth).unwrap(),
      FBYGEdgeBottom);

  if (node->hasMeasureFunc()) {
    FBYGNodeWithMeasureFuncSetMeasuredDimensions(
        node,
        availableWidth - marginAxisRow,
        availableHeight - marginAxisColumn,
        widthMeasureMode,
        heightMeasureMode,
        ownerWidth,
        ownerHeight,
        layoutMarkerData,
        layoutContext,
        reason);
    return;
  }

  const uint32_t childCount = FBYGNodeGetChildCount(node);
  if (childCount == 0) {
    FBYGNodeEmptyContainerSetMeasuredDimensions(
        node,
        availableWidth - marginAxisRow,
        availableHeight - marginAxisColumn,
        widthMeasureMode,
        heightMeasureMode,
        ownerWidth,
        ownerHeight);
    return;
  }

  // If we're not being asked to perform a full layout we can skip the algorithm
  // if we already know the size
  if (!performLayout &&
      FBYGNodeFixedSizeSetMeasuredDimensions(
          node,
          availableWidth - marginAxisRow,
          availableHeight - marginAxisColumn,
          widthMeasureMode,
          heightMeasureMode,
          ownerWidth,
          ownerHeight)) {
    return;
  }

  // At this point we know we're going to perform work. Ensure that each child
  // has a mutable copy.
  node->cloneChildrenIfNeeded(layoutContext);
  // Reset layout flags, as they could have changed.
  node->setLayoutHadOverflow(false);

  // STEP 1: CALCULATE VALUES FOR REMAINDER OF ALGORITHM
  const FBYGFlexDirection mainAxis =
      FBYGResolveFlexDirection(node->getStyle().flexDirection(), direction);
  const FBYGFlexDirection crossAxis = FBYGFlexDirectionCross(mainAxis, direction);
  const bool isMainAxisRow = FBYGFlexDirectionIsRow(mainAxis);
  const bool isNodeFlexWrap = node->getStyle().flexWrap() != FBYGWrapNoWrap;

  const float mainAxisownerSize = isMainAxisRow ? ownerWidth : ownerHeight;
  const float crossAxisownerSize = isMainAxisRow ? ownerHeight : ownerWidth;

  const float paddingAndBorderAxisMain =
      FBYGNodePaddingAndBorderForAxis(node, mainAxis, ownerWidth);
  const float leadingPaddingAndBorderCross =
      node->getLeadingPaddingAndBorder(crossAxis, ownerWidth).unwrap();
  const float trailingPaddingAndBorderCross =
      node->getTrailingPaddingAndBorder(crossAxis, ownerWidth).unwrap();
  const float paddingAndBorderAxisCross =
      leadingPaddingAndBorderCross + trailingPaddingAndBorderCross;

  FBYGMeasureMode measureModeMainDim =
      isMainAxisRow ? widthMeasureMode : heightMeasureMode;
  FBYGMeasureMode measureModeCrossDim =
      isMainAxisRow ? heightMeasureMode : widthMeasureMode;

  const float paddingAndBorderAxisRow =
      isMainAxisRow ? paddingAndBorderAxisMain : paddingAndBorderAxisCross;
  const float paddingAndBorderAxisColumn =
      isMainAxisRow ? paddingAndBorderAxisCross : paddingAndBorderAxisMain;

  // STEP 2: DETERMINE AVAILABLE SIZE IN MAIN AND CROSS DIRECTIONS

  float availableInnerWidth = FBYGNodeCalculateAvailableInnerDim(
      node,
      FBYGDimensionWidth,
      availableWidth - marginAxisRow,
      paddingAndBorderAxisRow,
      ownerWidth);
  float availableInnerHeight = FBYGNodeCalculateAvailableInnerDim(
      node,
      FBYGDimensionHeight,
      availableHeight - marginAxisColumn,
      paddingAndBorderAxisColumn,
      ownerHeight);

  float availableInnerMainDim =
      isMainAxisRow ? availableInnerWidth : availableInnerHeight;
  const float availableInnerCrossDim =
      isMainAxisRow ? availableInnerHeight : availableInnerWidth;

  // STEP 3: DETERMINE FLEX BASIS FOR EACH ITEM

  // Computed basis + margins + gap
  float totalMainDim = 0;
  totalMainDim += FBYGNodeComputeFlexBasisForChildren(
      node,
      availableInnerWidth,
      availableInnerHeight,
      widthMeasureMode,
      heightMeasureMode,
      direction,
      mainAxis,
      config,
      performLayout,
      layoutMarkerData,
      layoutContext,
      depth,
      generationCount);

  if (childCount > 1) {
    totalMainDim +=
        node->getGapForAxis(mainAxis, availableInnerCrossDim).unwrap() *
        (childCount - 1);
  }

  const bool mainAxisOverflows =
      (measureModeMainDim != FBYGMeasureModeUndefined) &&
      totalMainDim > availableInnerMainDim;

  if (isNodeFlexWrap && mainAxisOverflows &&
      measureModeMainDim == FBYGMeasureModeAtMost) {
    measureModeMainDim = FBYGMeasureModeExactly;
  }
  // STEP 4: COLLECT FLEX ITEMS INTO FLEX LINES

  // Indexes of children that represent the first and last items in the line.
  uint32_t startOfLineIndex = 0;
  uint32_t endOfLineIndex = 0;

  // Number of lines.
  uint32_t lineCount = 0;

  // Accumulated cross dimensions of all lines so far.
  float totalLineCrossDim = 0;

  const float crossAxisGap =
      node->getGapForAxis(crossAxis, availableInnerCrossDim).unwrap();

  // Max main dimension of all the lines.
  float maxLineMainDim = 0;
  FBYGCollectFlexItemsRowValues collectedFlexItemsValues;
  for (; endOfLineIndex < childCount;
       lineCount++, startOfLineIndex = endOfLineIndex) {
    collectedFlexItemsValues = FBYGCalculateCollectFlexItemsRowValues(
        node,
        ownerDirection,
        mainAxisownerSize,
        availableInnerWidth,
        availableInnerMainDim,
        startOfLineIndex,
        lineCount);
    endOfLineIndex = collectedFlexItemsValues.endOfLineIndex;

    // If we don't need to measure the cross axis, we can skip the entire flex
    // step.
    const bool canSkipFlex =
        !performLayout && measureModeCrossDim == FBYGMeasureModeExactly;

    // STEP 5: RESOLVING FLEXIBLE LENGTHS ON MAIN AXIS
    // Calculate the remaining available space that needs to be allocated. If
    // the main dimension size isn't known, it is computed based on the line
    // length, so there's no more space left to distribute.

    bool sizeBasedOnContent = false;
    // If we don't measure with exact main dimension we want to ensure we don't
    // violate min and max
    if (measureModeMainDim != FBYGMeasureModeExactly) {
      const auto& minDimensions = node->getStyle().minDimensions();
      const auto& maxDimensions = node->getStyle().maxDimensions();
      const float minInnerWidth =
          FBYGResolveValue(minDimensions[FBYGDimensionWidth], ownerWidth).unwrap() -
          paddingAndBorderAxisRow;
      const float maxInnerWidth =
          FBYGResolveValue(maxDimensions[FBYGDimensionWidth], ownerWidth).unwrap() -
          paddingAndBorderAxisRow;
      const float minInnerHeight =
          FBYGResolveValue(minDimensions[FBYGDimensionHeight], ownerHeight)
              .unwrap() -
          paddingAndBorderAxisColumn;
      const float maxInnerHeight =
          FBYGResolveValue(maxDimensions[FBYGDimensionHeight], ownerHeight)
              .unwrap() -
          paddingAndBorderAxisColumn;

      const float minInnerMainDim =
          isMainAxisRow ? minInnerWidth : minInnerHeight;
      const float maxInnerMainDim =
          isMainAxisRow ? maxInnerWidth : maxInnerHeight;

      if (!FBYGFloatIsUndefined(minInnerMainDim) &&
          collectedFlexItemsValues.sizeConsumedOnCurrentLine <
              minInnerMainDim) {
        availableInnerMainDim = minInnerMainDim;
      } else if (
          !FBYGFloatIsUndefined(maxInnerMainDim) &&
          collectedFlexItemsValues.sizeConsumedOnCurrentLine >
              maxInnerMainDim) {
        availableInnerMainDim = maxInnerMainDim;
      } else {
        bool useLegacyStretchBehaviour =
            node->hasErrata(FBYGErrataStretchFlexBasis);

        if (!useLegacyStretchBehaviour &&
            ((!FBYGFloatIsUndefined(
                  collectedFlexItemsValues.totalFlexGrowFactors) &&
              collectedFlexItemsValues.totalFlexGrowFactors == 0) ||
             (!FBYGFloatIsUndefined(node->resolveFlexGrow()) &&
              node->resolveFlexGrow() == 0))) {
          // If we don't have any children to flex or we can't flex the node
          // itself, space we've used is all space we need. Root node also
          // should be shrunk to minimum
          availableInnerMainDim =
              collectedFlexItemsValues.sizeConsumedOnCurrentLine;
        }

        sizeBasedOnContent = !useLegacyStretchBehaviour;
      }
    }

    if (!sizeBasedOnContent && !FBYGFloatIsUndefined(availableInnerMainDim)) {
      collectedFlexItemsValues.remainingFreeSpace = availableInnerMainDim -
          collectedFlexItemsValues.sizeConsumedOnCurrentLine;
    } else if (collectedFlexItemsValues.sizeConsumedOnCurrentLine < 0) {
      // availableInnerMainDim is indefinite which means the node is being sized
      // based on its content. sizeConsumedOnCurrentLine is negative which means
      // the node will allocate 0 points for its content. Consequently,
      // remainingFreeSpace is 0 - sizeConsumedOnCurrentLine.
      collectedFlexItemsValues.remainingFreeSpace =
          -collectedFlexItemsValues.sizeConsumedOnCurrentLine;
    }

    if (!canSkipFlex) {
      FBYGResolveFlexibleLength(
          node,
          collectedFlexItemsValues,
          mainAxis,
          crossAxis,
          mainAxisownerSize,
          availableInnerMainDim,
          availableInnerCrossDim,
          availableInnerWidth,
          availableInnerHeight,
          mainAxisOverflows,
          measureModeCrossDim,
          performLayout,
          config,
          layoutMarkerData,
          layoutContext,
          depth,
          generationCount);
    }

    node->setLayoutHadOverflow(
        node->getLayout().hadOverflow() |
        (collectedFlexItemsValues.remainingFreeSpace < 0));

    // STEP 6: MAIN-AXIS JUSTIFICATION & CROSS-AXIS SIZE DETERMINATION

    // At this point, all the children have their dimensions set in the main
    // axis. Their dimensions are also set in the cross axis with the exception
    // of items that are aligned "stretch". We need to compute these stretch
    // values and set the final positions.

    FBYGJustifyMainAxis(
        node,
        collectedFlexItemsValues,
        startOfLineIndex,
        mainAxis,
        crossAxis,
        measureModeMainDim,
        measureModeCrossDim,
        mainAxisownerSize,
        ownerWidth,
        availableInnerMainDim,
        availableInnerCrossDim,
        availableInnerWidth,
        performLayout,
        layoutContext);

    float containerCrossAxis = availableInnerCrossDim;
    if (measureModeCrossDim == FBYGMeasureModeUndefined ||
        measureModeCrossDim == FBYGMeasureModeAtMost) {
      // Compute the cross axis from the max cross dimension of the children.
      containerCrossAxis =
          FBYGNodeBoundAxis(
              node,
              crossAxis,
              collectedFlexItemsValues.crossDim + paddingAndBorderAxisCross,
              crossAxisownerSize,
              ownerWidth) -
          paddingAndBorderAxisCross;
    }

    // If there's no flex wrap, the cross dimension is defined by the container.
    if (!isNodeFlexWrap && measureModeCrossDim == FBYGMeasureModeExactly) {
      collectedFlexItemsValues.crossDim = availableInnerCrossDim;
    }

    // Clamp to the min/max size specified on the container.
    collectedFlexItemsValues.crossDim =
        FBYGNodeBoundAxis(
            node,
            crossAxis,
            collectedFlexItemsValues.crossDim + paddingAndBorderAxisCross,
            crossAxisownerSize,
            ownerWidth) -
        paddingAndBorderAxisCross;

    // STEP 7: CROSS-AXIS ALIGNMENT
    // We can skip child alignment if we're just measuring the container.
    if (performLayout) {
      for (uint32_t i = startOfLineIndex; i < endOfLineIndex; i++) {
        const FBYGNodeRef child = node->getChild(i);
        if (child->getStyle().display() == FBYGDisplayNone) {
          continue;
        }
        if (child->getStyle().positionType() == FBYGPositionTypeAbsolute) {
          // If the child is absolutely positioned and has a
          // top/left/bottom/right set, override all the previously computed
          // positions to set it correctly.
          const bool isChildLeadingPosDefined =
              child->isLeadingPositionDefined(crossAxis);
          if (isChildLeadingPosDefined) {
            child->setLayoutPosition(
                child->getLeadingPosition(crossAxis, availableInnerCrossDim)
                        .unwrap() +
                    node->getLeadingBorder(crossAxis) +
                    child->getLeadingMargin(crossAxis, availableInnerWidth)
                        .unwrap(),
                pos[crossAxis]);
          }
          // If leading position is not defined or calculations result in Nan,
          // default to border + margin
          if (!isChildLeadingPosDefined ||
              FBYGFloatIsUndefined(child->getLayout().position[pos[crossAxis]])) {
            child->setLayoutPosition(
                node->getLeadingBorder(crossAxis) +
                    child->getLeadingMargin(crossAxis, availableInnerWidth)
                        .unwrap(),
                pos[crossAxis]);
          }
        } else {
          float leadingCrossDim = leadingPaddingAndBorderCross;

          // For a relative children, we're either using alignItems (owner) or
          // alignSelf (child) in order to determine the position in the cross
          // axis
          const FBYGAlign alignItem = FBYGNodeAlignItem(node, child);

          // If the child uses align stretch, we need to lay it out one more
          // time, this time forcing the cross-axis size to be the computed
          // cross size for the current line.
          if (alignItem == FBYGAlignStretch &&
              child->marginLeadingValue(crossAxis).unit != FBYGUnitAuto &&
              child->marginTrailingValue(crossAxis).unit != FBYGUnitAuto) {
            // If the child defines a definite size for its cross axis, there's
            // no need to stretch.
            if (!FBYGNodeIsStyleDimDefined(
                    child, crossAxis, availableInnerCrossDim)) {
              float childMainSize =
                  child->getLayout().measuredDimensions[dim[mainAxis]];
              const auto& childStyle = child->getStyle();
              float childCrossSize = !childStyle.aspectRatio().isUndefined()
                  ? child->getMarginForAxis(crossAxis, availableInnerWidth)
                          .unwrap() +
                      (isMainAxisRow
                           ? childMainSize / childStyle.aspectRatio().unwrap()
                           : childMainSize * childStyle.aspectRatio().unwrap())
                  : collectedFlexItemsValues.crossDim;

              childMainSize +=
                  child->getMarginForAxis(mainAxis, availableInnerWidth)
                      .unwrap();

              FBYGMeasureMode childMainMeasureMode = FBYGMeasureModeExactly;
              FBYGMeasureMode childCrossMeasureMode = FBYGMeasureModeExactly;
              FBYGConstrainMaxSizeForMode(
                  child,
                  mainAxis,
                  availableInnerMainDim,
                  availableInnerWidth,
                  &childMainMeasureMode,
                  &childMainSize);
              FBYGConstrainMaxSizeForMode(
                  child,
                  crossAxis,
                  availableInnerCrossDim,
                  availableInnerWidth,
                  &childCrossMeasureMode,
                  &childCrossSize);

              const float childWidth =
                  isMainAxisRow ? childMainSize : childCrossSize;
              const float childHeight =
                  !isMainAxisRow ? childMainSize : childCrossSize;

              auto alignContent = node->getStyle().alignContent();
              auto crossAxisDoesNotGrow =
                  alignContent != FBYGAlignStretch && isNodeFlexWrap;
              const FBYGMeasureMode childWidthMeasureMode =
                  FBYGFloatIsUndefined(childWidth) ||
                      (!isMainAxisRow && crossAxisDoesNotGrow)
                  ? FBYGMeasureModeUndefined
                  : FBYGMeasureModeExactly;
              const FBYGMeasureMode childHeightMeasureMode =
                  FBYGFloatIsUndefined(childHeight) ||
                      (isMainAxisRow && crossAxisDoesNotGrow)
                  ? FBYGMeasureModeUndefined
                  : FBYGMeasureModeExactly;

              FBYGLayoutNodeInternal(
                  child,
                  childWidth,
                  childHeight,
                  direction,
                  childWidthMeasureMode,
                  childHeightMeasureMode,
                  availableInnerWidth,
                  availableInnerHeight,
                  true,
                  LayoutPassReason::kStretch,
                  config,
                  layoutMarkerData,
                  layoutContext,
                  depth,
                  generationCount);
            }
          } else {
            const float remainingCrossDim = containerCrossAxis -
                FBYGNodeDimWithMargin(child, crossAxis, availableInnerWidth);

            if (child->marginLeadingValue(crossAxis).unit == FBYGUnitAuto &&
                child->marginTrailingValue(crossAxis).unit == FBYGUnitAuto) {
              leadingCrossDim += FBYGFloatMax(0.0f, remainingCrossDim / 2);
            } else if (
                child->marginTrailingValue(crossAxis).unit == FBYGUnitAuto) {
              // No-Op
            } else if (
                child->marginLeadingValue(crossAxis).unit == FBYGUnitAuto) {
              leadingCrossDim += FBYGFloatMax(0.0f, remainingCrossDim);
            } else if (alignItem == FBYGAlignFlexStart) {
              // No-Op
            } else if (alignItem == FBYGAlignCenter) {
              leadingCrossDim += remainingCrossDim / 2;
            } else {
              leadingCrossDim += remainingCrossDim;
            }
          }
          // And we apply the position
          child->setLayoutPosition(
              child->getLayout().position[pos[crossAxis]] + totalLineCrossDim +
                  leadingCrossDim,
              pos[crossAxis]);
        }
      }
    }

    const float appliedCrossGap = lineCount != 0 ? crossAxisGap : 0.0f;
    totalLineCrossDim += collectedFlexItemsValues.crossDim + appliedCrossGap;
    maxLineMainDim =
        FBYGFloatMax(maxLineMainDim, collectedFlexItemsValues.mainDim);
  }

  // STEP 8: MULTI-LINE CONTENT ALIGNMENT
  // currentLead stores the size of the cross dim
  if (performLayout && (isNodeFlexWrap || FBYGIsBaselineLayout(node))) {
    float crossDimLead = 0;
    float currentLead = leadingPaddingAndBorderCross;
    if (!FBYGFloatIsUndefined(availableInnerCrossDim)) {
      const float remainingAlignContentDim =
          availableInnerCrossDim - totalLineCrossDim;
      switch (node->getStyle().alignContent()) {
        case FBYGAlignFlexEnd:
          currentLead += remainingAlignContentDim;
          break;
        case FBYGAlignCenter:
          currentLead += remainingAlignContentDim / 2;
          break;
        case FBYGAlignStretch:
          if (availableInnerCrossDim > totalLineCrossDim) {
            crossDimLead = remainingAlignContentDim / lineCount;
          }
          break;
        case FBYGAlignSpaceAround:
          if (availableInnerCrossDim > totalLineCrossDim) {
            currentLead += remainingAlignContentDim / (2 * lineCount);
            if (lineCount > 1) {
              crossDimLead = remainingAlignContentDim / lineCount;
            }
          } else {
            currentLead += remainingAlignContentDim / 2;
          }
          break;
        case FBYGAlignSpaceBetween:
          if (availableInnerCrossDim > totalLineCrossDim && lineCount > 1) {
            crossDimLead = remainingAlignContentDim / (lineCount - 1);
          }
          break;
        case FBYGAlignAuto:
        case FBYGAlignFlexStart:
        case FBYGAlignBaseline:
          break;
      }
    }
    uint32_t endIndex = 0;
    for (uint32_t i = 0; i < lineCount; i++) {
      const uint32_t startIndex = endIndex;
      uint32_t ii;

      // compute the line's height and find the endIndex
      float lineHeight = 0;
      float maxAscentForCurrentLine = 0;
      float maxDescentForCurrentLine = 0;
      for (ii = startIndex; ii < childCount; ii++) {
        const FBYGNodeRef child = node->getChild(ii);
        if (child->getStyle().display() == FBYGDisplayNone) {
          continue;
        }
        if (child->getStyle().positionType() != FBYGPositionTypeAbsolute) {
          if (child->getLineIndex() != i) {
            break;
          }
          if (FBYGNodeIsLayoutDimDefined(child, crossAxis)) {
            lineHeight = FBYGFloatMax(
                lineHeight,
                child->getLayout().measuredDimensions[dim[crossAxis]] +
                    child->getMarginForAxis(crossAxis, availableInnerWidth)
                        .unwrap());
          }
          if (FBYGNodeAlignItem(node, child) == FBYGAlignBaseline) {
            const float ascent = FBYGBaseline(child, layoutContext) +
                child
                    ->getLeadingMargin(
                        FBYGFlexDirectionColumn, availableInnerWidth)
                    .unwrap();
            const float descent =
                child->getLayout().measuredDimensions[FBYGDimensionHeight] +
                child
                    ->getMarginForAxis(
                        FBYGFlexDirectionColumn, availableInnerWidth)
                    .unwrap() -
                ascent;
            maxAscentForCurrentLine =
                FBYGFloatMax(maxAscentForCurrentLine, ascent);
            maxDescentForCurrentLine =
                FBYGFloatMax(maxDescentForCurrentLine, descent);
            lineHeight = FBYGFloatMax(
                lineHeight, maxAscentForCurrentLine + maxDescentForCurrentLine);
          }
        }
      }
      endIndex = ii;
      lineHeight += crossDimLead;
      currentLead += i != 0 ? crossAxisGap : 0;

      if (performLayout) {
        for (ii = startIndex; ii < endIndex; ii++) {
          const FBYGNodeRef child = node->getChild(ii);
          if (child->getStyle().display() == FBYGDisplayNone) {
            continue;
          }
          if (child->getStyle().positionType() != FBYGPositionTypeAbsolute) {
            switch (FBYGNodeAlignItem(node, child)) {
              case FBYGAlignFlexStart: {
                child->setLayoutPosition(
                    currentLead +
                        child->getLeadingMargin(crossAxis, availableInnerWidth)
                            .unwrap(),
                    pos[crossAxis]);
                break;
              }
              case FBYGAlignFlexEnd: {
                child->setLayoutPosition(
                    currentLead + lineHeight -
                        child->getTrailingMargin(crossAxis, availableInnerWidth)
                            .unwrap() -
                        child->getLayout().measuredDimensions[dim[crossAxis]],
                    pos[crossAxis]);
                break;
              }
              case FBYGAlignCenter: {
                float childHeight =
                    child->getLayout().measuredDimensions[dim[crossAxis]];

                child->setLayoutPosition(
                    currentLead + (lineHeight - childHeight) / 2,
                    pos[crossAxis]);
                break;
              }
              case FBYGAlignStretch: {
                child->setLayoutPosition(
                    currentLead +
                        child->getLeadingMargin(crossAxis, availableInnerWidth)
                            .unwrap(),
                    pos[crossAxis]);

                // Remeasure child with the line height as it as been only
                // measured with the owners height yet.
                if (!FBYGNodeIsStyleDimDefined(
                        child, crossAxis, availableInnerCrossDim)) {
                  const float childWidth = isMainAxisRow
                      ? (child->getLayout()
                             .measuredDimensions[FBYGDimensionWidth] +
                         child->getMarginForAxis(mainAxis, availableInnerWidth)
                             .unwrap())
                      : lineHeight;

                  const float childHeight = !isMainAxisRow
                      ? (child->getLayout()
                             .measuredDimensions[FBYGDimensionHeight] +
                         child->getMarginForAxis(crossAxis, availableInnerWidth)
                             .unwrap())
                      : lineHeight;

                  if (!(FBYGFloatsEqual(
                            childWidth,
                            child->getLayout()
                                .measuredDimensions[FBYGDimensionWidth]) &&
                        FBYGFloatsEqual(
                            childHeight,
                            child->getLayout()
                                .measuredDimensions[FBYGDimensionHeight]))) {
                    FBYGLayoutNodeInternal(
                        child,
                        childWidth,
                        childHeight,
                        direction,
                        FBYGMeasureModeExactly,
                        FBYGMeasureModeExactly,
                        availableInnerWidth,
                        availableInnerHeight,
                        true,
                        LayoutPassReason::kMultilineStretch,
                        config,
                        layoutMarkerData,
                        layoutContext,
                        depth,
                        generationCount);
                  }
                }
                break;
              }
              case FBYGAlignBaseline: {
                child->setLayoutPosition(
                    currentLead + maxAscentForCurrentLine -
                        FBYGBaseline(child, layoutContext) +
                        child
                            ->getLeadingPosition(
                                FBYGFlexDirectionColumn, availableInnerCrossDim)
                            .unwrap(),
                    FBYGEdgeTop);

                break;
              }
              case FBYGAlignAuto:
              case FBYGAlignSpaceBetween:
              case FBYGAlignSpaceAround:
                break;
            }
          }
        }
      }
      currentLead += lineHeight;
    }
  }

  // STEP 9: COMPUTING FINAL DIMENSIONS

  node->setLayoutMeasuredDimension(
      FBYGNodeBoundAxis(
          node,
          FBYGFlexDirectionRow,
          availableWidth - marginAxisRow,
          ownerWidth,
          ownerWidth),
      FBYGDimensionWidth);

  node->setLayoutMeasuredDimension(
      FBYGNodeBoundAxis(
          node,
          FBYGFlexDirectionColumn,
          availableHeight - marginAxisColumn,
          ownerHeight,
          ownerWidth),
      FBYGDimensionHeight);

  // If the user didn't specify a width or height for the node, set the
  // dimensions based on the children.
  if (measureModeMainDim == FBYGMeasureModeUndefined ||
      (node->getStyle().overflow() != FBYGOverflowScroll &&
       measureModeMainDim == FBYGMeasureModeAtMost)) {
    // Clamp the size to the min/max size, if specified, and make sure it
    // doesn't go below the padding and border amount.
    node->setLayoutMeasuredDimension(
        FBYGNodeBoundAxis(
            node, mainAxis, maxLineMainDim, mainAxisownerSize, ownerWidth),
        dim[mainAxis]);

  } else if (
      measureModeMainDim == FBYGMeasureModeAtMost &&
      node->getStyle().overflow() == FBYGOverflowScroll) {
    node->setLayoutMeasuredDimension(
        FBYGFloatMax(
            FBYGFloatMin(
                availableInnerMainDim + paddingAndBorderAxisMain,
                FBYGNodeBoundAxisWithinMinAndMax(
                    node,
                    mainAxis,
                    FBYGFloatOptional{maxLineMainDim},
                    mainAxisownerSize)
                    .unwrap()),
            paddingAndBorderAxisMain),
        dim[mainAxis]);
  }

  if (measureModeCrossDim == FBYGMeasureModeUndefined ||
      (node->getStyle().overflow() != FBYGOverflowScroll &&
       measureModeCrossDim == FBYGMeasureModeAtMost)) {
    // Clamp the size to the min/max size, if specified, and make sure it
    // doesn't go below the padding and border amount.
    node->setLayoutMeasuredDimension(
        FBYGNodeBoundAxis(
            node,
            crossAxis,
            totalLineCrossDim + paddingAndBorderAxisCross,
            crossAxisownerSize,
            ownerWidth),
        dim[crossAxis]);

  } else if (
      measureModeCrossDim == FBYGMeasureModeAtMost &&
      node->getStyle().overflow() == FBYGOverflowScroll) {
    node->setLayoutMeasuredDimension(
        FBYGFloatMax(
            FBYGFloatMin(
                availableInnerCrossDim + paddingAndBorderAxisCross,
                FBYGNodeBoundAxisWithinMinAndMax(
                    node,
                    crossAxis,
                    FBYGFloatOptional{
                        totalLineCrossDim + paddingAndBorderAxisCross},
                    crossAxisownerSize)
                    .unwrap()),
            paddingAndBorderAxisCross),
        dim[crossAxis]);
  }

  // As we only wrapped in normal direction yet, we need to reverse the
  // positions on wrap-reverse.
  if (performLayout && node->getStyle().flexWrap() == FBYGWrapWrapReverse) {
    for (uint32_t i = 0; i < childCount; i++) {
      const FBYGNodeRef child = FBYGNodeGetChild(node, i);
      if (child->getStyle().positionType() != FBYGPositionTypeAbsolute) {
        child->setLayoutPosition(
            node->getLayout().measuredDimensions[dim[crossAxis]] -
                child->getLayout().position[pos[crossAxis]] -
                child->getLayout().measuredDimensions[dim[crossAxis]],
            pos[crossAxis]);
      }
    }
  }

  if (performLayout) {
    // STEP 10: SIZING AND POSITIONING ABSOLUTE CHILDREN
    for (auto child : node->getChildren()) {
      if (child->getStyle().display() == FBYGDisplayNone ||
          child->getStyle().positionType() != FBYGPositionTypeAbsolute) {
        continue;
      }
      const bool absolutePercentageAgainstPaddingEdge =
          node->getConfig()->isExperimentalFeatureEnabled(
              FBYGExperimentalFeatureAbsolutePercentageAgainstPaddingEdge);

      FBYGNodeAbsoluteLayoutChild(
          node,
          child,
          absolutePercentageAgainstPaddingEdge
              ? node->getLayout().measuredDimensions[FBYGDimensionWidth]
              : availableInnerWidth,
          isMainAxisRow ? measureModeMainDim : measureModeCrossDim,
          absolutePercentageAgainstPaddingEdge
              ? node->getLayout().measuredDimensions[FBYGDimensionHeight]
              : availableInnerHeight,
          direction,
          config,
          layoutMarkerData,
          layoutContext,
          depth,
          generationCount);
    }

    // STEP 11: SETTING TRAILING POSITIONS FOR CHILDREN
    const bool needsMainTrailingPos = mainAxis == FBYGFlexDirectionRowReverse ||
        mainAxis == FBYGFlexDirectionColumnReverse;
    const bool needsCrossTrailingPos = crossAxis == FBYGFlexDirectionRowReverse ||
        crossAxis == FBYGFlexDirectionColumnReverse;

    // Set trailing position if necessary.
    if (needsMainTrailingPos || needsCrossTrailingPos) {
      for (uint32_t i = 0; i < childCount; i++) {
        const FBYGNodeRef child = node->getChild(i);
        if (child->getStyle().display() == FBYGDisplayNone) {
          continue;
        }
        if (needsMainTrailingPos) {
          FBYGNodeSetChildTrailingPosition(node, child, mainAxis);
        }

        if (needsCrossTrailingPos) {
          FBYGNodeSetChildTrailingPosition(node, child, crossAxis);
        }
      }
    }
  }
}

bool gPrintChanges = false;
bool gPrintSkips = false;

static const char* spacer =
    "                                                            ";

static const char* FBYGSpacer(const unsigned long level) {
  const size_t spacerLen = strlen(spacer);
  if (level > spacerLen) {
    return &spacer[0];
  } else {
    return &spacer[spacerLen - level];
  }
}

static const char* FBYGMeasureModeName(
    const FBYGMeasureMode mode,
    const bool performLayout) {
  constexpr auto N = enums::count<FBYGMeasureMode>();
  const char* kMeasureModeNames[N] = {"UNDEFINED", "EXACTLY", "AT_MOST"};
  const char* kLayoutModeNames[N] = {
      "LAY_UNDEFINED", "LAY_EXACTLY", "LAY_AT_MOST"};

  if (mode >= N) {
    return "";
  }

  return performLayout ? kLayoutModeNames[mode] : kMeasureModeNames[mode];
}

static inline bool FBYGMeasureModeSizeIsExactAndMatchesOldMeasuredSize(
    FBYGMeasureMode sizeMode,
    float size,
    float lastComputedSize) {
  return sizeMode == FBYGMeasureModeExactly &&
      FBYGFloatsEqual(size, lastComputedSize);
}

static inline bool FBYGMeasureModeOldSizeIsUnspecifiedAndStillFits(
    FBYGMeasureMode sizeMode,
    float size,
    FBYGMeasureMode lastSizeMode,
    float lastComputedSize) {
  return sizeMode == FBYGMeasureModeAtMost &&
      lastSizeMode == FBYGMeasureModeUndefined &&
      (size >= lastComputedSize || FBYGFloatsEqual(size, lastComputedSize));
}

static inline bool FBYGMeasureModeNewMeasureSizeIsStricterAndStillValid(
    FBYGMeasureMode sizeMode,
    float size,
    FBYGMeasureMode lastSizeMode,
    float lastSize,
    float lastComputedSize) {
  return lastSizeMode == FBYGMeasureModeAtMost &&
      sizeMode == FBYGMeasureModeAtMost && !FBYGFloatIsUndefined(lastSize) &&
      !FBYGFloatIsUndefined(size) && !FBYGFloatIsUndefined(lastComputedSize) &&
      lastSize > size &&
      (lastComputedSize <= size || FBYGFloatsEqual(size, lastComputedSize));
}

YOGA_EXPORT float FBYGRoundValueToPixelGrid(
    const double value,
    const double pointScaleFactor,
    const bool forceCeil,
    const bool forceFloor) {
  double scaledValue = value * pointScaleFactor;
  // We want to calculate `fractial` such that `floor(scaledValue) = scaledValue
  // - fractial`.
  double fractial = fmod(scaledValue, 1.0);
  if (fractial < 0) {
    // This branch is for handling negative numbers for `value`.
    //
    // Regarding `floor` and `ceil`. Note that for a number x, `floor(x) <= x <=
    // ceil(x)` even for negative numbers. Here are a couple of examples:
    //   - x =  2.2: floor( 2.2) =  2, ceil( 2.2) =  3
    //   - x = -2.2: floor(-2.2) = -3, ceil(-2.2) = -2
    //
    // Regarding `fmodf`. For fractional negative numbers, `fmodf` returns a
    // negative number. For example, `fmodf(-2.2) = -0.2`. However, we want
    // `fractial` to be the number such that subtracting it from `value` will
    // give us `floor(value)`. In the case of negative numbers, adding 1 to
    // `fmodf(value)` gives us this. Let's continue the example from above:
    //   - fractial = fmodf(-2.2) = -0.2
    //   - Add 1 to the fraction: fractial2 = fractial + 1 = -0.2 + 1 = 0.8
    //   - Finding the `floor`: -2.2 - fractial2 = -2.2 - 0.8 = -3
    ++fractial;
  }
  if (FBYGDoubleEqual(fractial, 0)) {
    // First we check if the value is already rounded
    scaledValue = scaledValue - fractial;
  } else if (FBYGDoubleEqual(fractial, 1.0)) {
    scaledValue = scaledValue - fractial + 1.0;
  } else if (forceCeil) {
    // Next we check if we need to use forced rounding
    scaledValue = scaledValue - fractial + 1.0;
  } else if (forceFloor) {
    scaledValue = scaledValue - fractial;
  } else {
    // Finally we just round the value
    scaledValue = scaledValue - fractial +
        (!FBYGDoubleIsUndefined(fractial) &&
                 (fractial > 0.5 || FBYGDoubleEqual(fractial, 0.5))
             ? 1.0
             : 0.0);
  }
  return (FBYGDoubleIsUndefined(scaledValue) ||
          FBYGDoubleIsUndefined(pointScaleFactor))
      ? FBYGUndefined
      : (float) (scaledValue / pointScaleFactor);
}

YOGA_EXPORT bool FBYGNodeCanUseCachedMeasurement(
    const FBYGMeasureMode widthMode,
    const float width,
    const FBYGMeasureMode heightMode,
    const float height,
    const FBYGMeasureMode lastWidthMode,
    const float lastWidth,
    const FBYGMeasureMode lastHeightMode,
    const float lastHeight,
    const float lastComputedWidth,
    const float lastComputedHeight,
    const float marginRow,
    const float marginColumn,
    const FBYGConfigRef config) {
  if ((!FBYGFloatIsUndefined(lastComputedHeight) && lastComputedHeight < 0) ||
      (!FBYGFloatIsUndefined(lastComputedWidth) && lastComputedWidth < 0)) {
    return false;
  }
  bool useRoundedComparison =
      config != nullptr && config->getPointScaleFactor() != 0;
  const float effectiveWidth = useRoundedComparison
      ? FBYGRoundValueToPixelGrid(
            width, config->getPointScaleFactor(), false, false)
      : width;
  const float effectiveHeight = useRoundedComparison
      ? FBYGRoundValueToPixelGrid(
            height, config->getPointScaleFactor(), false, false)
      : height;
  const float effectiveLastWidth = useRoundedComparison
      ? FBYGRoundValueToPixelGrid(
            lastWidth, config->getPointScaleFactor(), false, false)
      : lastWidth;
  const float effectiveLastHeight = useRoundedComparison
      ? FBYGRoundValueToPixelGrid(
            lastHeight, config->getPointScaleFactor(), false, false)
      : lastHeight;

  const bool hasSameWidthSpec = lastWidthMode == widthMode &&
      FBYGFloatsEqual(effectiveLastWidth, effectiveWidth);
  const bool hasSameHeightSpec = lastHeightMode == heightMode &&
      FBYGFloatsEqual(effectiveLastHeight, effectiveHeight);

  const bool widthIsCompatible =
      hasSameWidthSpec ||
      FBYGMeasureModeSizeIsExactAndMatchesOldMeasuredSize(
          widthMode, width - marginRow, lastComputedWidth) ||
      FBYGMeasureModeOldSizeIsUnspecifiedAndStillFits(
          widthMode, width - marginRow, lastWidthMode, lastComputedWidth) ||
      FBYGMeasureModeNewMeasureSizeIsStricterAndStillValid(
          widthMode,
          width - marginRow,
          lastWidthMode,
          lastWidth,
          lastComputedWidth);

  const bool heightIsCompatible =
      hasSameHeightSpec ||
      FBYGMeasureModeSizeIsExactAndMatchesOldMeasuredSize(
          heightMode, height - marginColumn, lastComputedHeight) ||
      FBYGMeasureModeOldSizeIsUnspecifiedAndStillFits(
          heightMode,
          height - marginColumn,
          lastHeightMode,
          lastComputedHeight) ||
      FBYGMeasureModeNewMeasureSizeIsStricterAndStillValid(
          heightMode,
          height - marginColumn,
          lastHeightMode,
          lastHeight,
          lastComputedHeight);

  return widthIsCompatible && heightIsCompatible;
}

//
// This is a wrapper around the FBYGNodelayoutImpl function. It determines whether
// the layout request is redundant and can be skipped.
//
// Parameters:
//  Input parameters are the same as FBYGNodelayoutImpl (see above)
//  Return parameter is true if layout was performed, false if skipped
//
bool FBYGLayoutNodeInternal(
    const FBYGNodeRef node,
    const float availableWidth,
    const float availableHeight,
    const FBYGDirection ownerDirection,
    const FBYGMeasureMode widthMeasureMode,
    const FBYGMeasureMode heightMeasureMode,
    const float ownerWidth,
    const float ownerHeight,
    const bool performLayout,
    const LayoutPassReason reason,
    const FBYGConfigRef config,
    LayoutData& layoutMarkerData,
    void* const layoutContext,
    uint32_t depth,
    const uint32_t generationCount) {
  FBYGLayout* layout = &node->getLayout();

  depth++;

  const bool needToVisitNode =
      (node->isDirty() && layout->generationCount != generationCount) ||
      layout->lastOwnerDirection != ownerDirection;

  if (needToVisitNode) {
    // Invalidate the cached results.
    layout->nextCachedMeasurementsIndex = 0;
    layout->cachedLayout.availableWidth = -1;
    layout->cachedLayout.availableHeight = -1;
    layout->cachedLayout.widthMeasureMode = FBYGMeasureModeUndefined;
    layout->cachedLayout.heightMeasureMode = FBYGMeasureModeUndefined;
    layout->cachedLayout.computedWidth = -1;
    layout->cachedLayout.computedHeight = -1;
  }

  FBYGCachedMeasurement* cachedResults = nullptr;

  // Determine whether the results are already cached. We maintain a separate
  // cache for layouts and measurements. A layout operation modifies the
  // positions and dimensions for nodes in the subtree. The algorithm assumes
  // that each node gets laid out a maximum of one time per tree layout, but
  // multiple measurements may be required to resolve all of the flex
  // dimensions. We handle nodes with measure functions specially here because
  // they are the most expensive to measure, so it's worth avoiding redundant
  // measurements if at all possible.
  if (node->hasMeasureFunc()) {
    const float marginAxisRow =
        node->getMarginForAxis(FBYGFlexDirectionRow, ownerWidth).unwrap();
    const float marginAxisColumn =
        node->getMarginForAxis(FBYGFlexDirectionColumn, ownerWidth).unwrap();

    // First, try to use the layout cache.
    if (FBYGNodeCanUseCachedMeasurement(
            widthMeasureMode,
            availableWidth,
            heightMeasureMode,
            availableHeight,
            layout->cachedLayout.widthMeasureMode,
            layout->cachedLayout.availableWidth,
            layout->cachedLayout.heightMeasureMode,
            layout->cachedLayout.availableHeight,
            layout->cachedLayout.computedWidth,
            layout->cachedLayout.computedHeight,
            marginAxisRow,
            marginAxisColumn,
            config)) {
      cachedResults = &layout->cachedLayout;
    } else {
      // Try to use the measurement cache.
      for (uint32_t i = 0; i < layout->nextCachedMeasurementsIndex; i++) {
        if (FBYGNodeCanUseCachedMeasurement(
                widthMeasureMode,
                availableWidth,
                heightMeasureMode,
                availableHeight,
                layout->cachedMeasurements[i].widthMeasureMode,
                layout->cachedMeasurements[i].availableWidth,
                layout->cachedMeasurements[i].heightMeasureMode,
                layout->cachedMeasurements[i].availableHeight,
                layout->cachedMeasurements[i].computedWidth,
                layout->cachedMeasurements[i].computedHeight,
                marginAxisRow,
                marginAxisColumn,
                config)) {
          cachedResults = &layout->cachedMeasurements[i];
          break;
        }
      }
    }
  } else if (performLayout) {
    if (FBYGFloatsEqual(layout->cachedLayout.availableWidth, availableWidth) &&
        FBYGFloatsEqual(layout->cachedLayout.availableHeight, availableHeight) &&
        layout->cachedLayout.widthMeasureMode == widthMeasureMode &&
        layout->cachedLayout.heightMeasureMode == heightMeasureMode) {
      cachedResults = &layout->cachedLayout;
    }
  } else {
    for (uint32_t i = 0; i < layout->nextCachedMeasurementsIndex; i++) {
      if (FBYGFloatsEqual(
              layout->cachedMeasurements[i].availableWidth, availableWidth) &&
          FBYGFloatsEqual(
              layout->cachedMeasurements[i].availableHeight, availableHeight) &&
          layout->cachedMeasurements[i].widthMeasureMode == widthMeasureMode &&
          layout->cachedMeasurements[i].heightMeasureMode ==
              heightMeasureMode) {
        cachedResults = &layout->cachedMeasurements[i];
        break;
      }
    }
  }

  if (!needToVisitNode && cachedResults != nullptr) {
    layout->measuredDimensions[FBYGDimensionWidth] = cachedResults->computedWidth;
    layout->measuredDimensions[FBYGDimensionHeight] =
        cachedResults->computedHeight;

    (performLayout ? layoutMarkerData.cachedLayouts
                   : layoutMarkerData.cachedMeasures) += 1;

    if (gPrintChanges && gPrintSkips) {
      Log::log(
          node,
          FBYGLogLevelVerbose,
          nullptr,
          "%s%d.{[skipped] ",
          FBYGSpacer(depth),
          depth);
      node->print(layoutContext);
      Log::log(
          node,
          FBYGLogLevelVerbose,
          nullptr,
          "wm: %s, hm: %s, aw: %f ah: %f => d: (%f, %f) %s\n",
          FBYGMeasureModeName(widthMeasureMode, performLayout),
          FBYGMeasureModeName(heightMeasureMode, performLayout),
          availableWidth,
          availableHeight,
          cachedResults->computedWidth,
          cachedResults->computedHeight,
          LayoutPassReasonToString(reason));
    }
  } else {
    if (gPrintChanges) {
      Log::log(
          node,
          FBYGLogLevelVerbose,
          nullptr,
          "%s%d.{%s",
          FBYGSpacer(depth),
          depth,
          needToVisitNode ? "*" : "");
      node->print(layoutContext);
      Log::log(
          node,
          FBYGLogLevelVerbose,
          nullptr,
          "wm: %s, hm: %s, aw: %f ah: %f %s\n",
          FBYGMeasureModeName(widthMeasureMode, performLayout),
          FBYGMeasureModeName(heightMeasureMode, performLayout),
          availableWidth,
          availableHeight,
          LayoutPassReasonToString(reason));
    }

    FBYGNodelayoutImpl(
        node,
        availableWidth,
        availableHeight,
        ownerDirection,
        widthMeasureMode,
        heightMeasureMode,
        ownerWidth,
        ownerHeight,
        performLayout,
        config,
        layoutMarkerData,
        layoutContext,
        depth,
        generationCount,
        reason);

    if (gPrintChanges) {
      Log::log(
          node,
          FBYGLogLevelVerbose,
          nullptr,
          "%s%d.}%s",
          FBYGSpacer(depth),
          depth,
          needToVisitNode ? "*" : "");
      node->print(layoutContext);
      Log::log(
          node,
          FBYGLogLevelVerbose,
          nullptr,
          "wm: %s, hm: %s, d: (%f, %f) %s\n",
          FBYGMeasureModeName(widthMeasureMode, performLayout),
          FBYGMeasureModeName(heightMeasureMode, performLayout),
          layout->measuredDimensions[FBYGDimensionWidth],
          layout->measuredDimensions[FBYGDimensionHeight],
          LayoutPassReasonToString(reason));
    }

    layout->lastOwnerDirection = ownerDirection;

    if (cachedResults == nullptr) {
      if (layout->nextCachedMeasurementsIndex + 1 >
          (uint32_t) layoutMarkerData.maxMeasureCache) {
        layoutMarkerData.maxMeasureCache =
            layout->nextCachedMeasurementsIndex + 1;
      }
      if (layout->nextCachedMeasurementsIndex == FBYG_MAX_CACHED_RESULT_COUNT) {
        if (gPrintChanges) {
          Log::log(node, FBYGLogLevelVerbose, nullptr, "Out of cache entries!\n");
        }
        layout->nextCachedMeasurementsIndex = 0;
      }

      FBYGCachedMeasurement* newCacheEntry;
      if (performLayout) {
        // Use the single layout cache entry.
        newCacheEntry = &layout->cachedLayout;
      } else {
        // Allocate a new measurement cache entry.
        newCacheEntry =
            &layout->cachedMeasurements[layout->nextCachedMeasurementsIndex];
        layout->nextCachedMeasurementsIndex++;
      }

      newCacheEntry->availableWidth = availableWidth;
      newCacheEntry->availableHeight = availableHeight;
      newCacheEntry->widthMeasureMode = widthMeasureMode;
      newCacheEntry->heightMeasureMode = heightMeasureMode;
      newCacheEntry->computedWidth =
          layout->measuredDimensions[FBYGDimensionWidth];
      newCacheEntry->computedHeight =
          layout->measuredDimensions[FBYGDimensionHeight];
    }
  }

  if (performLayout) {
    node->setLayoutDimension(
        node->getLayout().measuredDimensions[FBYGDimensionWidth],
        FBYGDimensionWidth);
    node->setLayoutDimension(
        node->getLayout().measuredDimensions[FBYGDimensionHeight],
        FBYGDimensionHeight);

    node->setHasNewLayout(true);
    node->setDirty(false);
  }

  layout->generationCount = generationCount;

  LayoutType layoutType;
  if (performLayout) {
    layoutType = !needToVisitNode && cachedResults == &layout->cachedLayout
        ? LayoutType::kCachedLayout
        : LayoutType::kLayout;
  } else {
    layoutType = cachedResults != nullptr ? LayoutType::kCachedMeasure
                                          : LayoutType::kMeasure;
  }
  Event::publish<Event::NodeLayout>(node, {layoutType, layoutContext});

  return (needToVisitNode || cachedResults == nullptr);
}

YOGA_EXPORT void FBYGConfigSetPointScaleFactor(
    const FBYGConfigRef config,
    const float pixelsInPoint) {
  FBYGAssertWithConfig(
      config,
      pixelsInPoint >= 0.0f,
      "Scale factor should not be less than zero");

  // We store points for Pixel as we will use it for rounding
  if (pixelsInPoint == 0.0f) {
    // Zero is used to skip rounding
    config->setPointScaleFactor(0.0f);
  } else {
    config->setPointScaleFactor(pixelsInPoint);
  }
}

YOGA_EXPORT float FBYGConfigGetPointScaleFactor(const FBYGConfigRef config) {
  return config->getPointScaleFactor();
}

static void FBYGRoundToPixelGrid(
    const FBYGNodeRef node,
    const double pointScaleFactor,
    const double absoluteLeft,
    const double absoluteTop) {
  if (pointScaleFactor == 0.0f) {
    return;
  }

  const double nodeLeft = node->getLayout().position[FBYGEdgeLeft];
  const double nodeTop = node->getLayout().position[FBYGEdgeTop];

  const double nodeWidth = node->getLayout().dimensions[FBYGDimensionWidth];
  const double nodeHeight = node->getLayout().dimensions[FBYGDimensionHeight];

  const double absoluteNodeLeft = absoluteLeft + nodeLeft;
  const double absoluteNodeTop = absoluteTop + nodeTop;

  const double absoluteNodeRight = absoluteNodeLeft + nodeWidth;
  const double absoluteNodeBottom = absoluteNodeTop + nodeHeight;

  // If a node has a custom measure function we never want to round down its
  // size as this could lead to unwanted text truncation.
  const bool textRounding = node->getNodeType() == FBYGNodeTypeText;

  node->setLayoutPosition(
      FBYGRoundValueToPixelGrid(nodeLeft, pointScaleFactor, false, textRounding),
      FBYGEdgeLeft);

  node->setLayoutPosition(
      FBYGRoundValueToPixelGrid(nodeTop, pointScaleFactor, false, textRounding),
      FBYGEdgeTop);

  // We multiply dimension by scale factor and if the result is close to the
  // whole number, we don't have any fraction To verify if the result is close
  // to whole number we want to check both floor and ceil numbers
  const bool hasFractionalWidth =
      !FBYGDoubleEqual(fmod(nodeWidth * pointScaleFactor, 1.0), 0) &&
      !FBYGDoubleEqual(fmod(nodeWidth * pointScaleFactor, 1.0), 1.0);
  const bool hasFractionalHeight =
      !FBYGDoubleEqual(fmod(nodeHeight * pointScaleFactor, 1.0), 0) &&
      !FBYGDoubleEqual(fmod(nodeHeight * pointScaleFactor, 1.0), 1.0);

  node->setLayoutDimension(
      FBYGRoundValueToPixelGrid(
          absoluteNodeRight,
          pointScaleFactor,
          (textRounding && hasFractionalWidth),
          (textRounding && !hasFractionalWidth)) -
          FBYGRoundValueToPixelGrid(
              absoluteNodeLeft, pointScaleFactor, false, textRounding),
      FBYGDimensionWidth);

  node->setLayoutDimension(
      FBYGRoundValueToPixelGrid(
          absoluteNodeBottom,
          pointScaleFactor,
          (textRounding && hasFractionalHeight),
          (textRounding && !hasFractionalHeight)) -
          FBYGRoundValueToPixelGrid(
              absoluteNodeTop, pointScaleFactor, false, textRounding),
      FBYGDimensionHeight);

  const uint32_t childCount = FBYGNodeGetChildCount(node);
  for (uint32_t i = 0; i < childCount; i++) {
    FBYGRoundToPixelGrid(
        FBYGNodeGetChild(node, i),
        pointScaleFactor,
        absoluteNodeLeft,
        absoluteNodeTop);
  }
}

YOGA_EXPORT void FBYGNodeCalculateLayoutWithContext(
    const FBYGNodeRef node,
    const float ownerWidth,
    const float ownerHeight,
    const FBYGDirection ownerDirection,
    void* layoutContext) {

  Event::publish<Event::LayoutPassStart>(node, {layoutContext});
  LayoutData markerData = {};

  // Increment the generation count. This will force the recursive routine to
  // visit all dirty nodes at least once. Subsequent visits will be skipped if
  // the input parameters don't change.
  gCurrentGenerationCount.fetch_add(1, std::memory_order_relaxed);
  node->resolveDimension();
  float width = FBYGUndefined;
  FBYGMeasureMode widthMeasureMode = FBYGMeasureModeUndefined;
  const auto& maxDimensions = node->getStyle().maxDimensions();
  if (FBYGNodeIsStyleDimDefined(node, FBYGFlexDirectionRow, ownerWidth)) {
    width =
        (FBYGResolveValue(
             node->getResolvedDimension(dim[FBYGFlexDirectionRow]), ownerWidth) +
         node->getMarginForAxis(FBYGFlexDirectionRow, ownerWidth))
            .unwrap();
    widthMeasureMode = FBYGMeasureModeExactly;
  } else if (!FBYGResolveValue(maxDimensions[FBYGDimensionWidth], ownerWidth)
                  .isUndefined()) {
    width =
        FBYGResolveValue(maxDimensions[FBYGDimensionWidth], ownerWidth).unwrap();
    widthMeasureMode = FBYGMeasureModeAtMost;
  } else {
    width = ownerWidth;
    widthMeasureMode = FBYGFloatIsUndefined(width) ? FBYGMeasureModeUndefined
                                                 : FBYGMeasureModeExactly;
  }

  float height = FBYGUndefined;
  FBYGMeasureMode heightMeasureMode = FBYGMeasureModeUndefined;
  if (FBYGNodeIsStyleDimDefined(node, FBYGFlexDirectionColumn, ownerHeight)) {
    height = (FBYGResolveValue(
                  node->getResolvedDimension(dim[FBYGFlexDirectionColumn]),
                  ownerHeight) +
              node->getMarginForAxis(FBYGFlexDirectionColumn, ownerWidth))
                 .unwrap();
    heightMeasureMode = FBYGMeasureModeExactly;
  } else if (!FBYGResolveValue(maxDimensions[FBYGDimensionHeight], ownerHeight)
                  .isUndefined()) {
    height =
        FBYGResolveValue(maxDimensions[FBYGDimensionHeight], ownerHeight).unwrap();
    heightMeasureMode = FBYGMeasureModeAtMost;
  } else {
    height = ownerHeight;
    heightMeasureMode = FBYGFloatIsUndefined(height) ? FBYGMeasureModeUndefined
                                                   : FBYGMeasureModeExactly;
  }
  if (FBYGLayoutNodeInternal(
          node,
          width,
          height,
          ownerDirection,
          widthMeasureMode,
          heightMeasureMode,
          ownerWidth,
          ownerHeight,
          true,
          LayoutPassReason::kInitial,
          node->getConfig(),
          markerData,
          layoutContext,
          0, // tree root
          gCurrentGenerationCount.load(std::memory_order_relaxed))) {
    node->setPosition(
        node->getLayout().direction(), ownerWidth, ownerHeight, ownerWidth);
    FBYGRoundToPixelGrid(
        node, node->getConfig()->getPointScaleFactor(), 0.0f, 0.0f);

#ifdef DEBUG
    if (node->getConfig()->shouldPrintTree()) {
      FBYGNodePrint(
          node,
          (FBYGPrintOptions) (FBYGPrintOptionsLayout | FBYGPrintOptionsChildren | FBYGPrintOptionsStyle));
    }
#endif
  }

  Event::publish<Event::LayoutPassEnd>(node, {layoutContext, &markerData});
}

YOGA_EXPORT void FBYGNodeCalculateLayout(
    const FBYGNodeRef node,
    const float ownerWidth,
    const float ownerHeight,
    const FBYGDirection ownerDirection) {
  FBYGNodeCalculateLayoutWithContext(
      node, ownerWidth, ownerHeight, ownerDirection, nullptr);
}

YOGA_EXPORT void FBYGConfigSetLogger(const FBYGConfigRef config, FBYGLogger logger) {
  if (logger != nullptr) {
    config->setLogger(logger);
  } else {
#ifdef ANDROID
    config->setLogger(&FBYGAndroidLog);
#else
    config->setLogger(&FBYGDefaultLog);
#endif
  }
}

void FBYGAssert(const bool condition, const char* message) {
  if (!condition) {
    Log::log(FBYGNodeRef{nullptr}, FBYGLogLevelFatal, nullptr, "%s\n", message);
    throwLogicalErrorWithMessage(message);
  }
}

void FBYGAssertWithNode(
    const FBYGNodeRef node,
    const bool condition,
    const char* message) {
  if (!condition) {
    Log::log(node, FBYGLogLevelFatal, nullptr, "%s\n", message);
    throwLogicalErrorWithMessage(message);
  }
}

void FBYGAssertWithConfig(
    const FBYGConfigRef config,
    const bool condition,
    const char* message) {
  if (!condition) {
    Log::log(config, FBYGLogLevelFatal, nullptr, "%s\n", message);
    throwLogicalErrorWithMessage(message);
  }
}

YOGA_EXPORT void FBYGConfigSetExperimentalFeatureEnabled(
    const FBYGConfigRef config,
    const FBYGExperimentalFeature feature,
    const bool enabled) {
  config->setExperimentalFeatureEnabled(feature, enabled);
}

YOGA_EXPORT bool FBYGConfigIsExperimentalFeatureEnabled(
    const FBYGConfigRef config,
    const FBYGExperimentalFeature feature) {
  return config->isExperimentalFeatureEnabled(feature);
}

YOGA_EXPORT void FBYGConfigSetUseWebDefaults(
    const FBYGConfigRef config,
    const bool enabled) {
  config->setUseWebDefaults(enabled);
}

YOGA_EXPORT bool FBYGConfigGetUseLegacyStretchBehaviour(
    const FBYGConfigRef config) {
  return config->hasErrata(FBYGErrataStretchFlexBasis);
}

YOGA_EXPORT void FBYGConfigSetUseLegacyStretchBehaviour(
    const FBYGConfigRef config,
    const bool useLegacyStretchBehaviour) {
  if (useLegacyStretchBehaviour) {
    config->addErrata(FBYGErrataStretchFlexBasis);
  } else {
    config->removeErrata(FBYGErrataStretchFlexBasis);
  }
}

bool FBYGConfigGetUseWebDefaults(const FBYGConfigRef config) {
  return config->useWebDefaults();
}

YOGA_EXPORT void FBYGConfigSetContext(const FBYGConfigRef config, void* context) {
  config->setContext(context);
}

YOGA_EXPORT void* FBYGConfigGetContext(const FBYGConfigRef config) {
  return config->getContext();
}

YOGA_EXPORT void FBYGConfigSetErrata(FBYGConfigRef config, FBYGErrata errata) {
  config->setErrata(errata);
}

YOGA_EXPORT FBYGErrata FBYGConfigGetErrata(FBYGConfigRef config) {
  return config->getErrata();
}

YOGA_EXPORT void FBYGConfigSetCloneNodeFunc(
    const FBYGConfigRef config,
    const FBYGCloneNodeFunc callback) {
  config->setCloneNodeCallback(callback);
}
