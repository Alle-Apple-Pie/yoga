/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>
#include <yoga/debug/AssertFatal.h>
#include <yoga/enums/Edge.h>
#include <yoga/node/Node.h>

using namespace facebookyg;
using namespace facebookyg::yoga;

namespace {

template <auto LayoutMember>
float getResolvedLayoutProperty(
    const FBYGNodeConstRef nodeRef,
    const Edge edge) {
  const auto node = resolveRef(nodeRef);
  yoga::assertFatalWithNode(
      node,
      edge <= Edge::End,
      "Cannot get layout properties of multi-edge shorthands");

  if (edge == Edge::Start) {
    if (node->getLayout().direction() == Direction::RTL) {
      return (node->getLayout().*LayoutMember)(PhysicalEdge::Right);
    } else {
      return (node->getLayout().*LayoutMember)(PhysicalEdge::Left);
    }
  }

  if (edge == Edge::End) {
    if (node->getLayout().direction() == Direction::RTL) {
      return (node->getLayout().*LayoutMember)(PhysicalEdge::Left);
    } else {
      return (node->getLayout().*LayoutMember)(PhysicalEdge::Right);
    }
  }

  return (node->getLayout().*LayoutMember)(static_cast<PhysicalEdge>(edge));
}

} // namespace

float FBYGNodeLayoutGetLeft(const FBYGNodeConstRef node) {
  return resolveRef(node)->getLayout().position(PhysicalEdge::Left);
}

float FBYGNodeLayoutGetTop(const FBYGNodeConstRef node) {
  return resolveRef(node)->getLayout().position(PhysicalEdge::Top);
}

float FBYGNodeLayoutGetRight(const FBYGNodeConstRef node) {
  return resolveRef(node)->getLayout().position(PhysicalEdge::Right);
}

float FBYGNodeLayoutGetBottom(const FBYGNodeConstRef node) {
  return resolveRef(node)->getLayout().position(PhysicalEdge::Bottom);
}

float FBYGNodeLayoutGetWidth(const FBYGNodeConstRef node) {
  return resolveRef(node)->getLayout().dimension(Dimension::Width);
}

float FBYGNodeLayoutGetHeight(const FBYGNodeConstRef node) {
  return resolveRef(node)->getLayout().dimension(Dimension::Height);
}

FBYGDirection FBYGNodeLayoutGetDirection(const FBYGNodeConstRef node) {
  return unscopedEnum(resolveRef(node)->getLayout().direction());
}

bool FBYGNodeLayoutGetHadOverflow(const FBYGNodeConstRef node) {
  return resolveRef(node)->getLayout().hadOverflow();
}

float FBYGNodeLayoutGetMargin(FBYGNodeConstRef node, FBYGEdge edge) {
  return getResolvedLayoutProperty<&LayoutResults::margin>(
      node, scopedEnum(edge));
}

float FBYGNodeLayoutGetBorder(FBYGNodeConstRef node, FBYGEdge edge) {
  return getResolvedLayoutProperty<&LayoutResults::border>(
      node, scopedEnum(edge));
}

float FBYGNodeLayoutGetPadding(FBYGNodeConstRef node, FBYGEdge edge) {
  return getResolvedLayoutProperty<&LayoutResults::padding>(
      node, scopedEnum(edge));
}
