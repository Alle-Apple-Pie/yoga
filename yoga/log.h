/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <yoga/YGEnums.h>

struct FBYGNode;
struct FBYGConfig;

namespace facebook {
namespace yoga {

namespace detail {

struct Log {
  static void log(
      FBYGNode* node,
      FBYGLogLevel level,
      void*,
      const char* message,
      ...) noexcept;

  static void log(
      FBYGConfig* config,
      FBYGLogLevel level,
      void*,
      const char* format,
      ...) noexcept;
};

} // namespace detail
} // namespace yoga
} // namespace facebook
