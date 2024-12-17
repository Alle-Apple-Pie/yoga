/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bitset>

#include <yoga/Yoga.h>
#include <yoga/enums/Errata.h>
#include <yoga/enums/ExperimentalFeature.h>
#include <yoga/enums/LogLevel.h>

// Tag struct used to form the opaque FBYGConfigRef for the public C API
struct FBYGConfig {};

namespace facebookyg::yoga {

class Config;
class Node;

using ExperimentalFeatureSet = std::bitset<ordinalCount<ExperimentalFeature>()>;

// Whether moving a node from an old to new config should dirty previously
// calculated layout results.
bool configUpdateInvalidatesLayout(
    const Config& oldConfig,
    const Config& newConfig);

class FBYG_EXPORT Config : public ::FBYGConfig {
 public:
  explicit Config(FBYGLogger logger) : logger_{logger} {}

  void setUseWebDefaults(bool useWebDefaults);
  bool useWebDefaults() const;

  void setExperimentalFeatureEnabled(ExperimentalFeature feature, bool enabled);
  bool isExperimentalFeatureEnabled(ExperimentalFeature feature) const;
  ExperimentalFeatureSet getEnabledExperiments() const;

  void setErrata(Errata errata);
  void addErrata(Errata errata);
  void removeErrata(Errata errata);
  Errata getErrata() const;
  bool hasErrata(Errata errata) const;

  void setPointScaleFactor(float pointScaleFactor);
  float getPointScaleFactor() const;

  void setContext(void* context);
  void* getContext() const;

  void setLogger(FBYGLogger logger);
  void log(
      const yoga::Node* node,
      LogLevel logLevel,
      const char* format,
      va_list args) const;

  void setCloneNodeCallback(FBYGCloneNodeFunc cloneNode);
  FBYGNodeRef cloneNode(
      FBYGNodeConstRef node,
      FBYGNodeConstRef owner,
      size_t childIndex) const;

  static const Config& getDefault();

 private:
  FBYGCloneNodeFunc cloneNodeCallback_{nullptr};
  FBYGLogger logger_{};

  bool useWebDefaults_ : 1 = false;

  ExperimentalFeatureSet experimentalFeatures_{};
  Errata errata_ = Errata::None;
  float pointScaleFactor_ = 1.0f;
  void* context_ = nullptr;
};

inline Config* resolveRef(const FBYGConfigRef ref) {
  return static_cast<Config*>(ref);
}

inline const Config* resolveRef(const FBYGConfigConstRef ref) {
  return static_cast<const Config*>(ref);
}

} // namespace facebookyg::yoga
