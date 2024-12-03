/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Defines.h>

// NOTE: Headers from the standard library.
#include <cstdarg>
#include <initializer_list>
#include <type_traits>

namespace AT {

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using s8 = signed char;
using s16 = signed short;
using s32 = signed int;
using s64 = signed long long;

using f32 = float;
using f64 = double;

using b8 = bool;
using b32 = int;

using usize = unsigned long long;
using ssize = signed long long;
using uintptr = unsigned long long;

using ReadonlyByte = const u8;
using WriteonlyByte = u8;
using ReadWriteByte = u8;

using ReadonlyBytes = ReadonlyByte*;
using WriteonlyBytes = WriteonlyByte*;
using ReadWriteBytes = ReadWriteByte*;

using NullptrType = decltype(nullptr);

namespace Implementation {
// clang-format off
template<typename T> struct RemoveReference      { using Type = T; };
template<typename T> struct RemoveReference<T&>  { using Type = T; };
template<typename T> struct RemoveReference<T&&> { using Type = T; };

template<typename T> struct RemoveConst          { using Type = T; };
template<typename T> struct RemoveConst<const T> { using Type = T; };

template<typename T> struct IsConst          { static constexpr bool value = false; };
template<typename T> struct IsConst<const T> { static constexpr bool value = true; };
// clang-format on
} // namespace Implementation

template<typename T>
using RemoveReference = typename Implementation::RemoveReference<T>::Type;

template<typename T>
using RemoveConst = typename Implementation::RemoveConst<T>::Type;

template<typename T>
static constexpr bool is_const = Implementation::IsConst<T>::value;

// Wrapper around 'https://en.cppreference.com/w/cpp/types/is_base_of'.
template<typename DerivedType, typename BaseType>
static constexpr bool is_derived_from = std::is_base_of_v<BaseType, DerivedType>;

// The STL equivalent of the move function. Same signature and behaviour.
// https://en.cppreference.com/w/cpp/utility/move
template<typename T>
NODISCARD ALWAYS_INLINE constexpr RemoveReference<T>&& move(T&& instance) noexcept
{
    return static_cast<RemoveReference<T>&&>(instance);
}

// The STL equivalent of the forward function. Same signature and behaviour.
// https://en.cppreference.com/w/cpp/utility/forward
template<typename T>
NODISCARD ALWAYS_INLINE constexpr T&& forward(RemoveReference<T>& instance) noexcept
{
    return static_cast<T&&>(instance);
}

// The STL equivalent of the forward function. Same signature and behaviour.
// https://en.cppreference.com/w/cpp/utility/forward
template<typename T>
NODISCARD ALWAYS_INLINE constexpr T&& forward(RemoveReference<T>&& instance) noexcept
{
    return static_cast<T&&>(instance);
}

#define AT_MAKE_NONCOPYABLE(type_name)    \
    type_name(const type_name&) = delete; \
    type_name& operator=(const type_name&) = delete

#define AT_MAKE_NONMOVABLE(type_name)         \
    type_name(type_name&&) noexcept = delete; \
    type_name& operator=(type_name&&) noexcept = delete

} // namespace AT

using AT::b32;
using AT::b8;
using AT::f32;
using AT::f64;
using AT::forward;
using AT::is_const;
using AT::move;
using AT::ReadonlyByte;
using AT::ReadonlyBytes;
using AT::ReadWriteByte;
using AT::ReadWriteBytes;
using AT::RemoveConst;
using AT::RemoveReference;
using AT::s16;
using AT::s32;
using AT::s64;
using AT::s8;
using AT::ssize;
using AT::u16;
using AT::u32;
using AT::u64;
using AT::u8;
using AT::uintptr;
using AT::usize;
using AT::WriteonlyByte;
using AT::WriteonlyBytes;
