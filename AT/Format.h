/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/API.h>
#include <AT/ByteBuffer.h>
#include <AT/String.h>

namespace AT {

class FormatStream {
    AT_MAKE_NONCOPYABLE(FormatStream);
    AT_MAKE_NONMOVABLE(FormatStream);

public:
    FormatStream() = default;
    ~FormatStream() = default;

    AT_API void push_codepoint(u32 codepoint);
    AT_API void push_unsigned_integer(u64 value);
    AT_API void push_signed_integer(s64 value);
    AT_API void push_floating_point_number(f64 value);
    AT_API void push_string(StringView string_view);

public:
    // NOTE: Ensures that the format stream will have enough memory allocated to store
    //       a push of the specified byte count. Represents a performance optimization.
    AT_API void ensure_push_byte_count(usize push_byte_count);

    NODISCARD ALWAYS_INLINE StringView formatted_string_view() const
    {
        const StringView view = StringView::from_utf8(ReadonlyByteSpan(m_formatted_buffer.bytes(), m_formatted_byte_count));
        return view;
    }

private:
    ByteBuffer m_formatted_buffer;
    usize m_formatted_byte_count { 0 };
};

template<typename T>
class Formatter {
public:
    static void format(FormatStream&, const T&)
    {
        // NOTE: You must specialize Formatter<T> in order to use the type T!
        static_assert(false);
    }
};

template<typename T>
requires (is_integer<T>)
class Formatter<T> {
public:
    static void format(FormatStream& stream, const T& value)
    {
        if (is_unsigned_integer<T>)
            stream.push_unsigned_integer(static_cast<u64>(value));
        else
            stream.push_signed_integer(static_cast<s64>(value));
    }
};

template<typename T>
requires (is_floating_point<T>)
class Formatter<T> {
public:
    static void format(FormatStream& stream, const T& value) { stream.push_floating_point_number(static_cast<f64>(value)); }
};

template<>
class Formatter<StringView> {
public:
    static void format(FormatStream& stream, const StringView& value) { stream.push_string(value); }
};

template<>
class Formatter<String> {
public:
    static void format(FormatStream& stream, const String& value) { stream.push_string(StringView(value)); }
};

template<>
class Formatter<char> {
public:
    static void format(FormatStream& stream, const char& value) { stream.push_codepoint(static_cast<u32>(value)); }
};

} // namespace AT

using AT::FormatStream;
using AT::Formatter;
