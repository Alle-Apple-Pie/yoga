/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * clang-format off
 * @generated SignedSource<<66f3152eedefd8718b50d16ea931e255>>
 * generated by gentest/gentest-driver.ts from gentest/fixtures/YGFlexTest.html
 */

#include <gtest/gtest.h>
#include <yoga/Yoga.h>
#include "../util/TestUtil.h"

TEST(YogaTest, flex_basis_flex_grow_column) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 100);
  YGNodeStyleSetHeight(root, 100);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0, 1);
  YGNodeStyleSetFlexBasis(root_child0, 50);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child1, 1);
  YGNodeInsertChild(root, root_child1, 1);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(25, YGNodeLayoutGetHeight(root_child1));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(25, YGNodeLayoutGetHeight(root_child1));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_shrink_flex_grow_row) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 500);
  YGNodeStyleSetHeight(root, 500);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexShrink(root_child0, 1);
  YGNodeStyleSetWidth(root_child0, 500);
  YGNodeStyleSetHeight(root_child0, 100);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexShrink(root_child1, 1);
  YGNodeStyleSetWidth(root_child1, 500);
  YGNodeStyleSetHeight(root_child1, 100);
  YGNodeInsertChild(root, root_child1, 1);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child1));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child1));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_shrink_flex_grow_child_flex_shrink_other_child) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 500);
  YGNodeStyleSetHeight(root, 500);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexShrink(root_child0, 1);
  YGNodeStyleSetWidth(root_child0, 500);
  YGNodeStyleSetHeight(root_child0, 100);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child1, 1);
  YGNodeStyleSetFlexShrink(root_child1, 1);
  YGNodeStyleSetWidth(root_child1, 500);
  YGNodeStyleSetHeight(root_child1, 100);
  YGNodeInsertChild(root, root_child1, 1);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child1));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(250, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child1));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_basis_flex_grow_row) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 100);
  YGNodeStyleSetHeight(root, 100);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0, 1);
  YGNodeStyleSetFlexBasis(root_child0, 50);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child1, 1);
  YGNodeInsertChild(root, root_child1, 1);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(25, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child1));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(25, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(25, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child1));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_basis_flex_shrink_column) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 100);
  YGNodeStyleSetHeight(root, 100);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexShrink(root_child0, 1);
  YGNodeStyleSetFlexBasis(root_child0, 100);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexBasis(root_child1, 50);
  YGNodeInsertChild(root, root_child1, 1);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetHeight(root_child1));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetHeight(root_child1));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_basis_flex_shrink_row) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 100);
  YGNodeStyleSetHeight(root, 100);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexShrink(root_child0, 1);
  YGNodeStyleSetFlexBasis(root_child0, 100);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexBasis(root_child1, 50);
  YGNodeInsertChild(root, root_child1, 1);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child1));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root_child1));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_shrink_to_zero) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetHeight(root, 75);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetWidth(root_child0, 50);
  YGNodeStyleSetHeight(root_child0, 50);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexShrink(root_child1, 1);
  YGNodeStyleSetWidth(root_child1, 50);
  YGNodeStyleSetHeight(root_child1, 50);
  YGNodeInsertChild(root, root_child1, 1);

  YGNodeRef root_child2 = YGNodeNewWithConfig(config);
  YGNodeStyleSetWidth(root_child2, 50);
  YGNodeStyleSetHeight(root_child2, 50);
  YGNodeInsertChild(root, root_child2, 2);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child1));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child2));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetTop(root_child2));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child2));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetHeight(root_child2));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(75, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child1));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child2));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetTop(root_child2));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetWidth(root_child2));
  ASSERT_FLOAT_EQ(50, YGNodeLayoutGetHeight(root_child2));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_basis_overrides_main_size) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 100);
  YGNodeStyleSetHeight(root, 100);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0, 1);
  YGNodeStyleSetFlexBasis(root_child0, 50);
  YGNodeStyleSetHeight(root_child0, 20);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child1, 1);
  YGNodeStyleSetHeight(root_child1, 10);
  YGNodeInsertChild(root, root_child1, 1);

  YGNodeRef root_child2 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child2, 1);
  YGNodeStyleSetHeight(root_child2, 10);
  YGNodeInsertChild(root, root_child2, 2);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(60, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(60, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(20, YGNodeLayoutGetHeight(root_child1));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child2));
  ASSERT_FLOAT_EQ(80, YGNodeLayoutGetTop(root_child2));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child2));
  ASSERT_FLOAT_EQ(20, YGNodeLayoutGetHeight(root_child2));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(60, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(60, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(20, YGNodeLayoutGetHeight(root_child1));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child2));
  ASSERT_FLOAT_EQ(80, YGNodeLayoutGetTop(root_child2));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child2));
  ASSERT_FLOAT_EQ(20, YGNodeLayoutGetHeight(root_child2));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_grow_shrink_at_most) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 100);
  YGNodeStyleSetHeight(root, 100);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child0_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0_child0, 1);
  YGNodeStyleSetFlexShrink(root_child0_child0, 1);
  YGNodeInsertChild(root_child0, root_child0_child0, 0);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child0_child0));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0_child0));
  ASSERT_FLOAT_EQ(100, YGNodeLayoutGetWidth(root_child0_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetHeight(root_child0_child0));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}

TEST(YogaTest, flex_grow_less_than_factor_one) {
  YGConfigRef config = YGConfigNew();

  YGNodeRef root = YGNodeNewWithConfig(config);
  YGNodeStyleSetPositionType(root, YGPositionTypeAbsolute);
  YGNodeStyleSetWidth(root, 200);
  YGNodeStyleSetHeight(root, 500);

  YGNodeRef root_child0 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child0, 0.2f);
  YGNodeStyleSetFlexBasis(root_child0, 40);
  YGNodeInsertChild(root, root_child0, 0);

  YGNodeRef root_child1 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child1, 0.2f);
  YGNodeInsertChild(root, root_child1, 1);

  YGNodeRef root_child2 = YGNodeNewWithConfig(config);
  YGNodeStyleSetFlexGrow(root_child2, 0.4f);
  YGNodeInsertChild(root, root_child2, 2);
  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionLTR);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(200, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(200, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(132, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(132, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(200, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(92, YGNodeLayoutGetHeight(root_child1));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child2));
  ASSERT_FLOAT_EQ(224, YGNodeLayoutGetTop(root_child2));
  ASSERT_FLOAT_EQ(200, YGNodeLayoutGetWidth(root_child2));
  ASSERT_FLOAT_EQ(184, YGNodeLayoutGetHeight(root_child2));

  YGNodeCalculateLayout(root, YGUndefined, YGUndefined, YGDirectionRTL);

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root));
  ASSERT_FLOAT_EQ(200, YGNodeLayoutGetWidth(root));
  ASSERT_FLOAT_EQ(500, YGNodeLayoutGetHeight(root));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child0));
  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetTop(root_child0));
  ASSERT_FLOAT_EQ(200, YGNodeLayoutGetWidth(root_child0));
  ASSERT_FLOAT_EQ(132, YGNodeLayoutGetHeight(root_child0));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child1));
  ASSERT_FLOAT_EQ(132, YGNodeLayoutGetTop(root_child1));
  ASSERT_FLOAT_EQ(200, YGNodeLayoutGetWidth(root_child1));
  ASSERT_FLOAT_EQ(92, YGNodeLayoutGetHeight(root_child1));

  ASSERT_FLOAT_EQ(0, YGNodeLayoutGetLeft(root_child2));
  ASSERT_FLOAT_EQ(224, YGNodeLayoutGetTop(root_child2));
  ASSERT_FLOAT_EQ(200, YGNodeLayoutGetWidth(root_child2));
  ASSERT_FLOAT_EQ(184, YGNodeLayoutGetHeight(root_child2));

  YGNodeFreeRecursive(root);

  YGConfigFree(config);
}
