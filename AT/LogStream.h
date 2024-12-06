/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/API.h>
#include <AT/StringBuilder.h>
#include <AT/StringView.h>

namespace AT {

class LogStream {
    AT_MAKE_NONCOPYABLE(LogStream);
    AT_MAKE_NONMOVABLE(LogStream);

public:
    enum class Type : u8 {
        Default,
        Error,
    };

    enum class Color : u8 {
        White,
    };

public:
    AT_API LogStream(Type type);
    ~LogStream() = default;

    AT_API void insert(StringView message);

    NODISCARD ALWAYS_INLINE Color output_color() const { return m_output_color; }
    AT_API void set_output_color(Color new_output_color);

private:
    Type m_type;
    Color m_output_color;
    void* m_console_native_handle;
};

AT_API void dbgln(const char* message);
AT_API void warnln(const char* message);
AT_API void errorln(const char* message);

template<typename... Args>
ALWAYS_INLINE void dbgln(const char* message, Args&&... args)
{
    const String formatted_message = StringBuilder::formatted(StringView::from_utf8(message), forward<Args>(args)...);
    dbgln(formatted_message.characters());
}

template<typename... Args>
ALWAYS_INLINE void warnln(const char* message, Args&&... args)
{
    const String formatted_message = StringBuilder::formatted(StringView::from_utf8(message), forward<Args>(args)...);
    warnln(formatted_message.characters());
}

template<typename... Args>
ALWAYS_INLINE void errorln(const char* message, Args&&... args)
{
    const String formatted_message = StringBuilder::formatted(StringView::from_utf8(message), forward<Args>(args)...);
    errorln(formatted_message.characters());
}

} // namespace AT
