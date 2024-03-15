/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <yoga/Yoga.h>
#include <yoga/debug/AssertFatal.h>
#include <yoga/debug/Log.h>

using namespace facebookyg;
using namespace facebookyg::yoga;

FBYGConfigRef FBYGConfigNew(void) {
  return new yoga::Config(getDefaultLogger());
}

void FBYGConfigFree(const FBYGConfigRef config) {
  delete resolveRef(config);
}

FBYGConfigConstRef FBYGConfigGetDefault() {
  return &yoga::Config::getDefault();
}

void FBYGConfigSetUseWebDefaults(
    const FBYGConfigRef config,
    const bool enabled) {
  resolveRef(config)->setUseWebDefaults(enabled);
}

bool FBYGConfigGetUseWebDefaults(const FBYGConfigConstRef config) {
  return resolveRef(config)->useWebDefaults();
}

void FBYGConfigSetPointScaleFactor(
    const FBYGConfigRef config,
    const float pixelsInPoint) {
  yoga::assertFatalWithConfig(
      resolveRef(config),
      pixelsInPoint >= 0.0f,
      "Scale factor should not be less than zero");

  resolveRef(config)->setPointScaleFactor(pixelsInPoint);
}

float FBYGConfigGetPointScaleFactor(const FBYGConfigConstRef config) {
  return resolveRef(config)->getPointScaleFactor();
}

void FBYGConfigSetErrata(FBYGConfigRef config, FBYGErrata errata) {
  resolveRef(config)->setErrata(scopedEnum(errata));
}

FBYGErrata FBYGConfigGetErrata(FBYGConfigConstRef config) {
  return unscopedEnum(resolveRef(config)->getErrata());
}

void FBYGConfigSetLogger(const FBYGConfigRef config, FBYGLogger logger) {
  if (logger != nullptr) {
    resolveRef(config)->setLogger(logger);
  } else {
    resolveRef(config)->setLogger(getDefaultLogger());
  }
}

void FBYGConfigSetContext(const FBYGConfigRef config, void* context) {
  resolveRef(config)->setContext(context);
}

void* FBYGConfigGetContext(const FBYGConfigConstRef config) {
  return resolveRef(config)->getContext();
}

void FBYGConfigSetExperimentalFeatureEnabled(
    const FBYGConfigRef config,
    const FBYGExperimentalFeature feature,
    const bool enabled) {
  resolveRef(config)->setExperimentalFeatureEnabled(
      scopedEnum(feature), enabled);
}

bool FBYGConfigIsExperimentalFeatureEnabled(
    const FBYGConfigConstRef config,
    const FBYGExperimentalFeature feature) {
  return resolveRef(config)->isExperimentalFeatureEnabled(scopedEnum(feature));
}

void FBYGConfigSetCloneNodeFunc(
    const FBYGConfigRef config,
    const FBYGCloneNodeFunc callback) {
  resolveRef(config)->setCloneNodeCallback(callback);
}
