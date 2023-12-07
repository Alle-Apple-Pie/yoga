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
#define WIN_EXPORT __declspec(dllexport)
#else
#define WIN_EXPORT
#endif

#ifndef YOGA_EXPORT
#ifdef _MSC_VER
#define YOGA_EXPORT
#else
#define YOGA_EXPORT __attribute__((visibility("default")))
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
#define FBYG_DEFINE_ENUM_FLAG_OPERATORS(name)                       \
  extern "C++" {                                                  \
  constexpr inline name operator~(name a) {                       \
    return static_cast<name>(                                     \
        ~static_cast<std::underlying_type<name>::type>(a));       \
  }                                                               \
  constexpr inline name operator|(name a, name b) {               \
    return static_cast<name>(                                     \
        static_cast<std::underlying_type<name>::type>(a) |        \
        static_cast<std::underlying_type<name>::type>(b));        \
  }                                                               \
  constexpr inline name operator&(name a, name b) {               \
    return static_cast<name>(                                     \
        static_cast<std::underlying_type<name>::type>(a) &        \
        static_cast<std::underlying_type<name>::type>(b));        \
  }                                                               \
  constexpr inline name operator^(name a, name b) {               \
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

#ifdef __cplusplus
namespace facebook {
namespace yoga {
namespace enums {

template <typename T>
constexpr int count(); // can't use `= delete` due to a defect in clang < 3.9

namespace detail {
template <int... xs>
constexpr int n() {
  return sizeof...(xs);
}
} // namespace detail

} // namespace enums
} // namespace yoga
} // namespace facebook
#endif

#define FBYG_ENUM_DECL(NAME, ...)                               \
  typedef FBYG_ENUM_BEGIN(NAME){__VA_ARGS__} FBYG_ENUM_END(NAME); \
  WIN_EXPORT const char* NAME##ToString(NAME);

#ifdef __cplusplus
#define FBYG_ENUM_SEQ_DECL(NAME, ...)  \
  FBYG_ENUM_DECL(NAME, __VA_ARGS__)    \
  FBYG_EXTERN_C_END                    \
  namespace facebook {               \
  namespace yoga {                   \
  namespace enums {                  \
  template <>                        \
  constexpr int count<NAME>() {      \
    return detail::n<__VA_ARGS__>(); \
  }                                  \
  }                                  \
  }                                  \
  }                                  \
  FBYG_EXTERN_C_BEGIN
#else
#define FBYG_ENUM_SEQ_DECL FBYG_ENUM_DECL
#endif
