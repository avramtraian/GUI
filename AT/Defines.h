/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#ifdef _WIN32
    #define AT_PLATFORM_WINDOWS 1
    #ifdef _WIN64
        #define AT_PLATFORM_ARCH_32_BIT 0
        #define AT_PLATFORM_ARCH_64_BIT 1
    #else
        #define AT_PLATFORM_ARCH_32_BIT 1
        #define AT_PLATFORM_ARCH_64_BIT 0
    #endif // _WIN64
#else
    #define AT_PLATFORM_WINDOWS 0
#endif // _WIN32

#if defined(__clang__)
    #define AT_COMPILER_CLANG 1
    #define AT_COMPILER_MSVC  0
    #define AT_COMPILER_GCC   0
#elif defined(_MSC_BUILD)
    #define AT_COMPILER_CLANG 0
    #define AT_COMPILER_MSVC  1
    #define AT_COMPILER_GCC   0
#elif defined(__GNUC__)
    #define AT_COMPILER_CLANG 0
    #define AT_COMPILER_MSVC  0
    #define AT_COMPILER_GCC   1
#endif // _MSC_BUILD

#if AT_COMPILER_MSVC
    #define ALWAYS_INLINE          __forceinline
    #define AT_FUNCTION            __FUNCSIG__
    #define AT_PLATFORM_DEBUGBREAK __debugbreak()
#else
    #define ALWAYS_INLINE          __attribute__((always_inline)) inline
    #define AT_FUNCTION            __PRETTY_FUNCTION__
    #define AT_PLATFORM_DEBUGBREAK __builtin_trap()
#endif // AT_COMPILER_MSVC

#define NODISCARD    [[nodiscard]]
#define MAYBE_UNUSED [[maybe_unused]]
#define LIKELY       [[likely]]
#define UNLIKELY     [[unlikely]]
