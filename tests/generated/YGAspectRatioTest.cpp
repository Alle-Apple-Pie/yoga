/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * clang-format off
 * @generated SignedSource<<0894aa78d01d5194e4c042491128cd1c>>
 * generated by gentest/gentest-driver.ts from gentest/fixtures/YGAspectRatioTest.html
 */

#include <gtest/gtest.h>
#include <yoga/Yoga.h>
#include "../util/TestUtil.h"

TEST(YogaTest, aspect_ratio_does_not_stretch_cross_axis_dim) {
  GTEST_SKIP();

  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 300);
  YGNodeStyleSetHeight(root, 300);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetOverflow(root_child0, YGOverflowScroll);
  YGNodeStyleSetFlexGrow(root_child0, 1);
  YGNodeStyleSetFlexShrink(root_child0, 1);
  YGNodeStyleSetFlexBasisPercent(root_child0, 0);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child0_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexDirection(root_child0_child0, YGFlexDirectionRow);
  YGNodeInsertChild(root_child0, root_child0_child0, 0);

  YGNodeRef root_child0_child0_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0_child0_child0, 2);
  YGNodeStyleSetFlexShrink(root_child0_child0_child0, 1);
  YGNodeStyleSetFlexBasisPercent(root_child0_child0_child0, 0);
  YGNodeStyleSetAspectRatio(root_child0_child0_child0, 1 / 1);
  YGNodeInsertChild(root_child0_child0, root_child0_child0_child0, 0);

  YGNodeRef root_child0_child0_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetWidth(root_child0_child0_child1, 5);
  YGNodeInsertChild(root_child0_child0, root_child0_child0_child1, 1);

  YGNodeRef root_child0_child0_child2 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0_child0_child2, 1);
  YGNodeStyleSetFlexShrink(root_child0_child0_child2, 1);
  YGNodeStyleSetFlexBasisPercent(root_child0_child0_child2, 0);
  YGNodeInsertChild(root_child0_child0, root_child0_child0_child2, 2);

  YGNodeRef root_child0_child0_child2_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0_child0_child2_child0, 1);
  YGNodeStyleSetFlexShrink(root_child0_child0_child2_child0, 1);
  YGNodeStyleSetFlexBasisPercent(root_child0_child0_child2_child0, 0);
  YGNodeStyleSetAspectRatio(root_child0_child0_child2_child0, 1 / 1);
  YGNodeInsertChild(root_child0_child0_child2, root_child0_child0_child2_child0, 0);

  YGNodeRef root_child0_child0_child2_child0_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetWidth(root_child0_child0_child2_child0_child0, 5);
  YGNodeInsertChild(root_child0_child0_child2_child0, root_child0_child0_child2_child0_child0, 0);

  YGNodeRef root_child0_child0_child2_child0_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0_child0_child2_child0_child1, 1);
  YGNodeStyleSetFlexShrink(root_child0_child0_child2_child0_child1, 1);
  YGNodeStyleSetFlexBasisPercent(root_child0_child0_child2_child0_child1, 0);
  YGNodeStyleSetAspectRatio(root_child0_child0_child2_child0_child1, 1 / 1);
  YGNodeInsertChild(root_child0_child0_child2_child0, root_child0_child0_child2_child0_child1, 1);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetWidth(root_child0_child0));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetWidth(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child0));

  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetLeft(root_child0_child0_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child1));
  ASSERT_FLOAT_EQ(5, YGNodeLayoutGetWidth(root_child0_child0_child1));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child1));

  ASSERT_FLOAT_EQ(202, YGNodeLayoutGetLeft(root_child0_child0_child2));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child2));
  ASSERT_FLOAT_EQ(98, YGNodeLayoutGetWidth(root_child0_child0_child2));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child2));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0_child2_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child2_child0));
  ASSERT_FLOAT_EQ(98, YGNodeLayoutGetWidth(root_child0_child0_child2_child0));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child2_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0_child2_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child2_child0_child0));
  ASSERT_FLOAT_EQ(5, YGNodeLayoutGetWidth(root_child0_child0_child2_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child0_child0_child2_child0_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0_child2_child0_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child2_child0_child1));
  ASSERT_FLOAT_EQ(98, YGNodeLayoutGetWidth(root_child0_child0_child2_child0_child1));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child2_child0_child1));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetWidth(root_child0_child0));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0));

  ASSERT_FLOAT_EQ(103, YGNodeLayoutGetLeft(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetWidth(root_child0_child0_child0));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child0));

  ASSERT_FLOAT_EQ(98, YGNodeLayoutGetLeft(root_child0_child0_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child1));
  ASSERT_FLOAT_EQ(5, YGNodeLayoutGetWidth(root_child0_child0_child1));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child1));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0_child2));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child2));
  ASSERT_FLOAT_EQ(98, YGNodeLayoutGetWidth(root_child0_child0_child2));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child2));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0_child2_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child2_child0));
  ASSERT_FLOAT_EQ(98, YGNodeLayoutGetWidth(root_child0_child0_child2_child0));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child2_child0));

  ASSERT_FLOAT_EQ(93, YGNodeLayoutGetLeft(root_child0_child0_child2_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child2_child0_child0));
  ASSERT_FLOAT_EQ(5, YGNodeLayoutGetWidth(root_child0_child0_child2_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child0_child0_child2_child0_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0_child2_child0_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0_child2_child0_child1));
  ASSERT_FLOAT_EQ(98, YGNodeLayoutGetWidth(root_child0_child0_child2_child0_child1));
  ASSERT_FLOAT_EQ(197, YGNodeLayoutGetHeight(root_child0_child0_child2_child0_child1));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, zero_aspect_ratio_behaves_like_auto) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 300);
  YGNodeStyleSetHeight(root, 300);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetWidth(root_child0, 50);
  YGNodeStyleSetAspectRatio(root_child0, 0 / 1);
  YGNodeInsertChild(root, root_child0, 0);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child0));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(300, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child0));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}
