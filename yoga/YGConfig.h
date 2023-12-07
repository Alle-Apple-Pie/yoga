/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <yoga/Yoga.h>

#include "BitUtils.h"
#include "Yoga-internal.h"

namespace facebook {
namespace yoga {

// Whether moving a node from config "a" to config "b" should dirty previously
// calculated layout results.
bool configUpdateInvalidatesLayout(FBYGConfigRef a, FBYGConfigRef b);

// Internal variants of log functions, currently used only by JNI bindings.
// TODO: Reconcile this with the public API
using LogWithContextFn = int (*)(
    FBYGConfigRef config,
    FBYGNodeRef node,
    FBYGLogLevel level,
    void* context,
    const char* format,
    va_list args);
using CloneWithContextFn = FBYGNodeRef (*)(
    FBYGNodeRef node,
    FBYGNodeRef owner,
    int childIndex,
    void* cloneContext);

using ExperimentalFeatureSet =
    facebook::yoga::detail::EnumBitset<FBYGExperimentalFeature>;

#pragma pack(push)
#pragma pack(1)
// Packed structure of <32-bit options to miminize size per node.
struct FBYGConfigFlags {
  bool useWebDefaults : 1;
  bool printTree : 1;
  bool cloneNodeUsesContext : 1;
  bool loggerUsesContext : 1;
};
#pragma pack(pop)

} // namespace yoga
} // namespace facebook

struct YOGA_EXPORT FBYGConfig {
  FBYGConfig(FBYGLogger logger);

  void setUseWebDefaults(bool useWebDefaults);
  bool useWebDefaults() const;

  void setShouldPrintTree(bool printTree);
  bool shouldPrintTree() const;

  void setExperimentalFeatureEnabled(
      FBYGExperimentalFeature feature,
      bool enabled);
  bool isExperimentalFeatureEnabled(FBYGExperimentalFeature feature) const;
  facebook::yoga::ExperimentalFeatureSet getEnabledExperiments() const;

  void setErrata(FBYGErrata errata);
  void addErrata(FBYGErrata errata);
  void removeErrata(FBYGErrata errata);
  FBYGErrata getErrata() const;
  bool hasErrata(FBYGErrata errata) const;

  void setPointScaleFactor(float pointScaleFactor);
  float getPointScaleFactor() const;

  void setContext(void* context);
  void* getContext() const;

  void setLogger(FBYGLogger logger);
  void setLogger(facebook::yoga::LogWithContextFn logger);
  void setLogger(std::nullptr_t);
  void log(FBYGConfig*, FBYGNode*, FBYGLogLevel, void*, const char*, va_list) const;

  void setCloneNodeCallback(FBYGCloneNodeFunc cloneNode);
  void setCloneNodeCallback(facebook::yoga::CloneWithContextFn cloneNode);
  void setCloneNodeCallback(std::nullptr_t);
  FBYGNodeRef cloneNode(
      FBYGNodeRef node,
      FBYGNodeRef owner,
      int childIndex,
      void* cloneContext) const;

private:
  union {
    facebook::yoga::CloneWithContextFn withContext;
    FBYGCloneNodeFunc noContext;
  } cloneNodeCallback_;
  union {
    facebook::yoga::LogWithContextFn withContext;
    FBYGLogger noContext;
  } logger_;

  facebook::yoga::FBYGConfigFlags flags_{};
  facebook::yoga::ExperimentalFeatureSet experimentalFeatures_{};
  FBYGErrata errata_ = FBYGErrataNone;
  float pointScaleFactor_ = 1.0f;
  void* context_ = nullptr;
};
