/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/YGValue.h>
#include <yoga/numeric/Comparison.h>

using namespace facebookyg;
using namespace facebookyg::yoga;

const FBYGValue FBYGValueZero = {0, FBYGUnitPoint};
const FBYGValue FBYGValueUndefined = {FBYGUndefined, FBYGUnitUndefined};
const FBYGValue FBYGValueAuto = {FBYGUndefined, FBYGUnitAuto};

bool FBYGFloatIsUndefined(const float value) {
  return yoga::isUndefined(value);
}
