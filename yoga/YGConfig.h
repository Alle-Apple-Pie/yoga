/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include <yoga/YGEnums.h>
#include <yoga/YGMacros.h>

FBYG_EXTERN_C_BEGIN

typedef struct FBYGNode* FBYGNodeRef;
typedef const struct FBYGNode* FBYGNodeConstRef;

/**
 * Handle to a mutable Yoga configuration.
 */
typedef struct FBYGConfig* FBYGConfigRef;

/**
 * Handle to an immutable Yoga configuration.
 */
typedef const struct FBYGConfig* FBYGConfigConstRef;

/**
 * Allocates a set of configuration options. The configuration may be applied to
 * multiple nodes (i.e. a single global config), or can be applied more
 * granularly per-node.
 */
FBYG_EXPORT FBYGConfigRef FBYGConfigNew(void);

/**
 * Frees the associated Yoga configuration.
 */
FBYG_EXPORT void FBYGConfigFree(FBYGConfigRef config);

/**
 * Returns the default config values set by Yoga.
 */
FBYG_EXPORT FBYGConfigConstRef FBYGConfigGetDefault(void);

/**
 * Yoga by default creates new nodes with style defaults different from flexbox
 * on web (e.g. `YGFlexDirectionColumn` and `YGPositionRelative`).
 * `UseWebDefaults` instructs Yoga to instead use a default style consistent
 * with the web.
 */
FBYG_EXPORT void FBYGConfigSetUseWebDefaults(FBYGConfigRef config, bool enabled);

/**
 * Whether the configuration is set to use web defaults.
 */
FBYG_EXPORT bool FBYGConfigGetUseWebDefaults(FBYGConfigConstRef config);

/**
 * Yoga will by default round final layout positions and dimensions to the
 * nearst point. `pointScaleFactor` controls the density of the grid used for
 * layout rounding (e.g. to round to the closest display pixel).
 *
 * May be set to 0.0f to avoid rounding the layout results.
 */
FBYG_EXPORT void FBYGConfigSetPointScaleFactor(
    FBYGConfigRef config,
    float pixelsInPoint);

/**
 * Get the currently set point scale factor.
 */
FBYG_EXPORT float FBYGConfigGetPointScaleFactor(FBYGConfigConstRef config);

/**
 * Configures how Yoga balances W3C conformance vs compatibility with layouts
 * created against earlier versions of Yoga.
 *
 * By default Yoga will prioritize W3C conformance. `Errata` may be set to ask
 * Yoga to produce specific incorrect behaviors. E.g. `YGConfigSetErrata(config,
 * FBYGErrataStretchFlexBasis)`.
 *
 * FBYGErrata is a bitmask, and multiple errata may be set at once. Predefined
 * constants exist for convenience:
 * 1. FBYGErrataNone: No errata
 * 2. FBYGErrataClassic: Match layout behaviors of Yoga 1.x
 * 3. FBYGErrataAll: Match layout behaviors of Yoga 1.x, including
 * `UseLegacyStretchBehaviour`
 */
FBYG_EXPORT void FBYGConfigSetErrata(FBYGConfigRef config, FBYGErrata errata);

/**
 * Get the currently set errata.
 */
FBYG_EXPORT FBYGErrata FBYGConfigGetErrata(FBYGConfigConstRef config);

/**
 * Function pointer type for FBYGConfigSetLogger.
 */
typedef int (*FBYGLogger)(
    FBYGConfigConstRef config,
    FBYGNodeConstRef node,
    FBYGLogLevel level,
    const char* format,
    va_list args);

/**
 * Set a custom log function for to use when logging diagnostics or fatal.
 * errors.
 */
FBYG_EXPORT void FBYGConfigSetLogger(FBYGConfigRef config, FBYGLogger logger);

/**
 * Sets an arbitrary context pointer on the config which may be read from during
 * callbacks.
 */
FBYG_EXPORT void FBYGConfigSetContext(FBYGConfigRef config, void* context);

/**
 * Gets the currently set context.
 */
FBYG_EXPORT void* FBYGConfigGetContext(FBYGConfigConstRef config);

/**
 * Function pointer type for FBYGConfigSetCloneNodeFunc.
 */
typedef FBYGNodeRef (*FBYGCloneNodeFunc)(
    FBYGNodeConstRef oldNode,
    FBYGNodeConstRef owner,
    size_t childIndex);

/**
 * Enable an experimental/unsupported feature in Yoga.
 */
FBYG_EXPORT void FBYGConfigSetExperimentalFeatureEnabled(
    FBYGConfigRef config,
    FBYGExperimentalFeature feature,
    bool enabled);

/**
 * Whether an experimental feature is set.
 */
FBYG_EXPORT bool FBYGConfigIsExperimentalFeatureEnabled(
    FBYGConfigConstRef config,
    FBYGExperimentalFeature feature);

/**
 * Sets a callback, called during layout, to create a new mutable Yoga node if
 * Yoga must write to it and its owner is not its parent observed during layout.
 */
FBYG_EXPORT void FBYGConfigSetCloneNodeFunc(
    FBYGConfigRef config,
    FBYGCloneNodeFunc callback);

FBYG_EXTERN_C_END
