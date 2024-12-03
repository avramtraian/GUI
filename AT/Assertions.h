/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/API.h>
#include <AT/Types.h>

namespace AT {

enum class AssertionKind : u8 {
    Assert,
    Verify,
    VerifyNotReached,
    TODO,
};

struct AssertionSourceLocation {
    const char* file { nullptr };
    const char* function { nullptr };
    u32 line_number { 0 };
};

AT_API void on_assertion_failed(AssertionKind kind, const char* expression, const AssertionSourceLocation& source_location);

} // namespace AT

using AT::AssertionKind;
using AT::AssertionSourceLocation;
using AT::on_assertion_failed;

#define ASSERT(...)                                                                            \
    if (!(__VA_ARGS__)) {                                                                      \
        ::AT::AssertionSourceLocation source_location;                                         \
        source_location.file = __FILE__;                                                       \
        source_location.function = AT_FUNCTION;                                                \
        source_location.line_number = __LINE__;                                                \
        ::AT::on_assertion_failed(::AT::AssertionKind::Assert, #__VA_ARGS__, source_location); \
        AT_PLATFORM_DEBUGBREAK;                                                                \
    }

#define VERIFY(...)                                                                            \
    if (!(__VA_ARGS__)) {                                                                      \
        ::AT::AssertionSourceLocation source_location;                                         \
        source_location.file = __FILE__;                                                       \
        source_location.function = AT_FUNCTION;                                                \
        source_location.line_number = __LINE__;                                                \
        ::AT::on_assertion_failed(::AT::AssertionKind::Verify, #__VA_ARGS__, source_location); \
        AT_PLATFORM_DEBUGBREAK;                                                                \
    }

#define VERIFY_NOT_REACHED()                                                                        \
    {                                                                                               \
        ::AT::AssertionSourceLocation source_location;                                              \
        source_location.file = __FILE__;                                                            \
        source_location.function = AT_FUNCTION;                                                     \
        source_location.line_number = __LINE__;                                                     \
        ::AT::on_assertion_failed(::AT::AssertionKind::VerifyNotReached, nullptr, source_location); \
        AT_PLATFORM_DEBUGBREAK;                                                                     \
    }

#define TODO()                                                                          \
    {                                                                                   \
        ::AT::AssertionSourceLocation source_location;                                  \
        source_location.file = __FILE__;                                                \
        source_location.function = AT_FUNCTION;                                         \
        source_location.line_number = __LINE__;                                         \
        ::AT::on_assertion_failed(::AT::AssertionKind::TODO, nullptr, source_location); \
        AT_PLATFORM_DEBUGBREAK;                                                         \
    }
