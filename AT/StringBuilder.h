/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/API.h>
#include <AT/ByteBuffer.h>
#include <AT/Format.h>
#include <AT/String.h>

namespace AT {

class StringBuilder {
    AT_MAKE_NONCOPYABLE(StringBuilder);
    AT_MAKE_NONMOVABLE(StringBuilder);

public:
    template<typename... Args>
    NODISCARD ALWAYS_INLINE static String formatted(StringView format, Args&&... parameters)
    {
        StringBuilder builder;
        builder.formatted_implementation(format, forward<Args>(parameters)...);
        return builder.release_string();
    }

public:
    StringBuilder() = default;
    ~StringBuilder() = default;

    NODISCARD AT_API String release_string();

public:
    AT_API void append(StringView string_view);

    template<typename T>
    ALWAYS_INLINE void append_formatted(const T& value)
    {
        FormatStream stream;
        Formatter<T>::format(stream, value);
        append(stream.formatted_string_view());
    }

private:
    void ensure_append_byte_count(usize append_byte_count);

    template<typename CurrentParameter, typename... Args>
    ALWAYS_INLINE void formatted_implementation(StringView& format, const CurrentParameter& current_parameter, Args&&... parameters)
    {
        consume_until_format_specifier(format);
        // NOTE: The number of parameters passed to the `formatted` function doesn't match the
        //       number of format specifiers located in the format string.
        VERIFY(format.has_characters());
        consume_format_specifier(format);
        append_formatted(current_parameter);
        formatted_implementation(format, forward<Args>(parameters)...);
    }

    ALWAYS_INLINE void formatted_implementation(StringView& format)
    {
        consume_until_format_specifier(format);
        // NOTE: The number of parameters passed to the `formatted` function doesn't match the
        //       number of format specifiers located in the format string.
        VERIFY(format.is_empty());
    }

    AT_API void consume_until_format_specifier(StringView& cursor);
    AT_API void consume_format_specifier(StringView& cursor);

private:
    ByteBuffer m_characters_buffer;
    usize m_byte_count { 0 };
};

} // namespace AT

using AT::StringBuilder;
