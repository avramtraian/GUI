/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Defines.h>

#if AT_PLATFORM_WINDOWS
    #if AT_COMPILER_MSVC
        #define AT_API_SPECIFIER_EXPORT __declspec(dllexport)
        #define AT_API_SPECIFIER_IMPORT __declspec(dllimport)
    #endif // AT_COMPILER_MSVC
#endif // AT_PLATFORM_WINDOWS

#ifndef AT_API_SPECIFIER_EXPORT
    #define AT_API_SPECIFIER_EXPORT
#endif // AT_API_SPECIFIER_EXPORT

#ifndef AT_API_SPECIFIER_IMPORT
    #define AT_API_SPECIFIER_IMPORT
#endif // AT_API_SPECIFIER_IMPORT
