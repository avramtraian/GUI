/*
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <AT/LogStream.h>

#if AT_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
#endif // AT_PLATFORM_WINDOWS

namespace AT {

LogStream::LogStream(Type type)
    : m_type(type)
    , m_output_color(Color::White)
    , m_console_native_handle(nullptr)
{}

void LogStream::insert(StringView message)
{
#if AT_PLATFORM_WINDOWS
    if (m_console_native_handle == nullptr) {
        DWORD standard_handle = 0;
        if (m_type == Type::Default)
            standard_handle = STD_OUTPUT_HANDLE;
        else if (m_type == Type::Error)
            standard_handle = STD_ERROR_HANDLE;
        else
            VERIFY_NOT_REACHED();

        m_console_native_handle = GetStdHandle(standard_handle);
    }

    // TODO: Convert the string to UTF-16 before writing it to the console.
    //       This currently only supports ASCII encoded messages.
    DWORD number_of_bytes_written = 0;
    const bool result =
        WriteConsoleA(m_console_native_handle, message.characters(), static_cast<DWORD>(message.byte_count()), &number_of_bytes_written, nullptr);
    if (!result || number_of_bytes_written != message.byte_count()) {
        // NOTE: What should happen here? We can't log anything because that is the action that failed, but
        //       asserting when a log fails seems excessive. Think about it.
    }
#endif // AT_PLATFORM_WINDOWS
}

void LogStream::set_output_color(Color new_output_color)
{
    if (new_output_color != m_output_color) {
        m_output_color = new_output_color;

#if AT_PLATFORM_WINDOWS
        // TODO: Actually implement the output color changed by updating
        //       the platform console output.
#endif // AT_PLATFORM_WINDOWS
    }
}

static LogStream s_dbg_log_stream = LogStream(LogStream::Type::Default);
static LogStream s_warn_log_stream = LogStream(LogStream::Type::Default);
static LogStream s_error_log_stream = LogStream(LogStream::Type::Error);

void dbgln(const char* message)
{
    const StringView message_view = StringView::from_utf8(message);
    const String formatted_message = StringBuilder::formatted("(Debug): {}\n"sv, message_view);
    s_dbg_log_stream.insert(formatted_message);
}

void warnln(const char* message)
{
    const StringView message_view = StringView::from_utf8(message);
    const String formatted_message = StringBuilder::formatted("(Warn):  {}\n"sv, message_view);
    s_warn_log_stream.insert(formatted_message);
}

void errorln(const char* message)
{
    const StringView message_view = StringView::from_utf8(message);
    const String formatted_message = StringBuilder::formatted("(Error): {}\n"sv, message_view);
    s_error_log_stream.insert(formatted_message);
}

} // namespace AT
