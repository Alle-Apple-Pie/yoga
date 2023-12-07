/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>

#include "log.h"
#include "YGConfig.h"
#include "YGNode.h"

namespace facebook {
namespace yoga {
namespace detail {

namespace {

void vlog(
    FBYGConfig* config,
    FBYGNode* node,
    FBYGLogLevel level,
    void* context,
    const char* format,
    va_list args) {
  FBYGConfig* logConfig = config != nullptr ? config : FBYGConfigGetDefault();
  logConfig->log(logConfig, node, level, context, format, args);
}
} // namespace

YOGA_EXPORT void Log::log(
    FBYGNode* node,
    FBYGLogLevel level,
    void* context,
    const char* format,
    ...) noexcept {
  va_list args;
  va_start(args, format);
  vlog(
      node == nullptr ? nullptr : node->getConfig(),
      node,
      level,
      context,
      format,
      args);
  va_end(args);
}

void Log::log(
    FBYGConfig* config,
    FBYGLogLevel level,
    void* context,
    const char* format,
    ...) noexcept {
  va_list args;
  va_start(args, format);
  vlog(config, nullptr, level, context, format, args);
  va_end(args);
}

} // namespace detail
} // namespace yoga
} // namespace facebook
