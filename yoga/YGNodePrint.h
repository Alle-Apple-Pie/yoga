/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifdef DEBUG

#pragma once

#include <string>

#include <yoga/Yoga.h>

namespace facebook {
namespace yoga {

void FBYGNodeToString(
    std::string& str,
    FBYGNodeRef node,
    FBYGPrintOptions options,
    uint32_t level);

} // namespace yoga
} // namespace facebook

#endif
