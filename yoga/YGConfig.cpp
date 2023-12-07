/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "YGConfig.h"

using namespace facebook::yoga;

namespace facebook {
namespace yoga {
bool configUpdateInvalidatesLayout(FBYGConfigRef a, FBYGConfigRef b) {
  return a->getErrata() != b->getErrata() ||
      a->getEnabledExperiments() != b->getEnabledExperiments() ||
      a->getPointScaleFactor() != b->getPointScaleFactor() ||
      a->useWebDefaults() != b->useWebDefaults();
}
} // namespace yoga
} // namespace facebook

FBYGConfig::FBYGConfig(FBYGLogger logger) : cloneNodeCallback_{nullptr} {
  setLogger(logger);
}

void FBYGConfig::setUseWebDefaults(bool useWebDefaults) {
  flags_.useWebDefaults = useWebDefaults;
}

bool FBYGConfig::useWebDefaults() const {
  return flags_.useWebDefaults;
}

void FBYGConfig::setShouldPrintTree(bool printTree) {
  flags_.printTree = printTree;
}

bool FBYGConfig::shouldPrintTree() const {
  return flags_.printTree;
}

void FBYGConfig::setExperimentalFeatureEnabled(
    FBYGExperimentalFeature feature,
    bool enabled) {
  experimentalFeatures_.set(feature, enabled);
}

bool FBYGConfig::isExperimentalFeatureEnabled(
    FBYGExperimentalFeature feature) const {
  return experimentalFeatures_.test(feature);
}

ExperimentalFeatureSet FBYGConfig::getEnabledExperiments() const {
  return experimentalFeatures_;
}

void FBYGConfig::setErrata(FBYGErrata errata) {
  errata_ = errata;
}

void FBYGConfig::addErrata(FBYGErrata errata) {
  errata_ |= errata;
}

void FBYGConfig::removeErrata(FBYGErrata errata) {
  errata_ &= (~errata);
}

FBYGErrata FBYGConfig::getErrata() const {
  return errata_;
}

bool FBYGConfig::hasErrata(FBYGErrata errata) const {
  return (errata_ & errata) != FBYGErrataNone;
}

void FBYGConfig::setPointScaleFactor(float pointScaleFactor) {
  pointScaleFactor_ = pointScaleFactor;
}

float FBYGConfig::getPointScaleFactor() const {
  return pointScaleFactor_;
}

void FBYGConfig::setContext(void* context) {
  context_ = context;
}

void* FBYGConfig::getContext() const {
  return context_;
}

void FBYGConfig::setLogger(FBYGLogger logger) {
  logger_.noContext = logger;
  flags_.loggerUsesContext = false;
}

void FBYGConfig::setLogger(LogWithContextFn logger) {
  logger_.withContext = logger;
  flags_.loggerUsesContext = true;
}

void FBYGConfig::setLogger(std::nullptr_t) {
  setLogger(FBYGLogger{nullptr});
}

void FBYGConfig::log(
    FBYGConfig* config,
    FBYGNode* node,
    FBYGLogLevel logLevel,
    void* logContext,
    const char* format,
    va_list args) const {
  if (flags_.loggerUsesContext) {
    logger_.withContext(config, node, logLevel, logContext, format, args);
  } else {
    logger_.noContext(config, node, logLevel, format, args);
  }
}

void FBYGConfig::setCloneNodeCallback(FBYGCloneNodeFunc cloneNode) {
  cloneNodeCallback_.noContext = cloneNode;
  flags_.cloneNodeUsesContext = false;
}

void FBYGConfig::setCloneNodeCallback(CloneWithContextFn cloneNode) {
  cloneNodeCallback_.withContext = cloneNode;
  flags_.cloneNodeUsesContext = true;
}

void FBYGConfig::setCloneNodeCallback(std::nullptr_t) {
  setCloneNodeCallback(FBYGCloneNodeFunc{nullptr});
}

FBYGNodeRef FBYGConfig::cloneNode(
    FBYGNodeRef node,
    FBYGNodeRef owner,
    int childIndex,
    void* cloneContext) const {
  FBYGNodeRef clone = nullptr;
  if (cloneNodeCallback_.noContext != nullptr) {
    clone = flags_.cloneNodeUsesContext
        ? cloneNodeCallback_.withContext(node, owner, childIndex, cloneContext)
        : cloneNodeCallback_.noContext(node, owner, childIndex);
  }
  if (clone == nullptr) {
    clone = FBYGNodeClone(node);
  }
  return clone;
}
