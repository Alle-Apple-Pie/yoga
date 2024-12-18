/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifdef __cplusplus
#include <type_traits>
#endif

#ifdef __cplusplus
#define FBYG_EXTERN_C_BEGIN extern "C" {
#define FBYG_EXTERN_C_END }
#else
#define FBYG_EXTERN_C_BEGIN
#define FBYG_EXTERN_C_END
#endif

#if defined(__cplusplus)
#define FBYG_DEPRECATED(message) [[deprecated(message)]]
#elif defined(_MSC_VER)
#define FBYG_DEPRECATED(message) __declspec(deprecated(message))
#else
#define FBYG_DEPRECATED(message) __attribute__((deprecated(message)))
#endif

#ifdef _WINDLL
#define FBYG_EXPORT __declspec(dllexport)
#elif !defined(_MSC_VER)
#define FBYG_EXPORT __attribute__((visibility("default")))
#else
#define FBYG_EXPORT
#endif

#ifdef __OBJC__
#if __has_include(<Foundation/Foundation.h>)
#import <Foundation/Foundation.h>
#endif
#endif

#ifdef NS_ENUM
// Cannot use NSInteger as NSInteger has a different size than int (which is the
// default type of a enum). Therefor when linking the Yoga C library into obj-c
// the header is a mismatch for the Yoga ABI.
#define FBYG_ENUM_BEGIN(name) NS_ENUM(int, name)
#define FBYG_ENUM_END(name)
#else
#define FBYG_ENUM_BEGIN(name) enum name
#define FBYG_ENUM_END(name) name
#endif

#ifdef __cplusplus
#define FBYG_DEFINE_ENUM_FLAG_OPERATORS(name)                     \
  extern "C++" {                                                  \
  constexpr name operator~(name a) {                              \
    return static_cast<name>(                                     \
        ~static_cast<std::underlying_type<name>::type>(a));       \
  }                                                               \
  constexpr name operator|(name a, name b) {                      \
    return static_cast<name>(                                     \
        static_cast<std::underlying_type<name>::type>(a) |        \
        static_cast<std::underlying_type<name>::type>(b));        \
  }                                                               \
  constexpr name operator&(name a, name b) {                      \
    return static_cast<name>(                                     \
        static_cast<std::underlying_type<name>::type>(a) &        \
        static_cast<std::underlying_type<name>::type>(b));        \
  }                                                               \
  constexpr name operator^(name a, name b) {                      \
    return static_cast<name>(                                     \
        static_cast<std::underlying_type<name>::type>(a) ^        \
        static_cast<std::underlying_type<name>::type>(b));        \
  }                                                               \
  inline name& operator|=(name& a, name b) {                      \
    return reinterpret_cast<name&>(                               \
        reinterpret_cast<std::underlying_type<name>::type&>(a) |= \
        static_cast<std::underlying_type<name>::type>(b));        \
  }                                                               \
  inline name& operator&=(name& a, name b) {                      \
    return reinterpret_cast<name&>(                               \
        reinterpret_cast<std::underlying_type<name>::type&>(a) &= \
        static_cast<std::underlying_type<name>::type>(b));        \
  }                                                               \
  inline name& operator^=(name& a, name b) {                      \
    return reinterpret_cast<name&>(                               \
        reinterpret_cast<std::underlying_type<name>::type&>(a) ^= \
        static_cast<std::underlying_type<name>::type>(b));        \
  }                                                               \
  }
#else
#define FBYG_DEFINE_ENUM_FLAG_OPERATORS(name)
#endif

#define FBYG_ENUM_DECL(NAME, ...)                                 \
  typedef FBYG_ENUM_BEGIN(NAME){__VA_ARGS__} FBYG_ENUM_END(NAME); \
  FBYG_EXPORT const char* NAME##ToString(NAME);
