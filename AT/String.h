/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/StringView.h>
#include <AT/Types.h>

namespace AT {

class String {
public:
    struct HeapBufferHeader {
        u32 reference_count { 0 };
    };

    static constexpr usize inline_capacity = sizeof(void*);
    static_assert(inline_capacity > 0);

public:
    AT_API String();
    AT_API ~String();

    AT_API String(const String& other);
    AT_API String(String&& other) noexcept;
    AT_API String(StringView string_view);

    AT_API String& operator=(const String& other);
    AT_API String& operator=(String&& other) noexcept;
    AT_API String& operator=(StringView string_view);

public:
    NODISCARD ALWAYS_INLINE bool is_stored_inline() const { return (m_byte_count <= inline_capacity); }
    NODISCARD ALWAYS_INLINE bool is_empty() const { return (m_byte_count <= 1); }

    // NOTE: Returns the number of bytes the string occupies, excluding the null-termination character.
    NODISCARD ALWAYS_INLINE usize byte_count() const
    {
        VERIFY(m_byte_count > 0);
        return m_byte_count - 1;
    }

    // NOTE: It is guaranteed to always return a value strictly greater than zero, as the null-termination
    //       character is always be present.
    NODISCARD ALWAYS_INLINE usize byte_count_including_null_terminator() const
    {
        VERIFY(m_byte_count > 0);
        return m_byte_count;
    }

    NODISCARD ALWAYS_INLINE const char* characters() const
    {
        if (is_stored_inline())
            return m_inline_buffer;
        return heap_buffer_characters();
    }

public:
    ALWAYS_INLINE void clear();

private:
    NODISCARD static void* allocate_heap_buffer(usize characters_byte_count);
    static void free_heap_buffer(void* heap_buffer, usize characters_byte_count);

private:
    NODISCARD ALWAYS_INLINE char* heap_buffer_characters() { return static_cast<char*>(m_heap_buffer) + sizeof(HeapBufferHeader); }
    NODISCARD ALWAYS_INLINE const char* heap_buffer_characters() const { return static_cast<const char*>(m_heap_buffer) + sizeof(HeapBufferHeader); }

    void increment_reference_count();
    NODISCARD bool decrement_reference_count();

private:
    union {
        void* m_heap_buffer;
        char m_inline_buffer[inline_capacity];
    };
    usize m_byte_count;
};

} // namespace AT

using AT::String;
