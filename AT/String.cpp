/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <AT/MemoryOperations.h>
#include <AT/New.h>
#include <AT/NumericLimits.h>
#include <AT/String.h>

namespace AT {

String::String()
    : m_byte_count(1)
{
    // Set the null-termination character.
    m_inline_buffer[0] = 0;
}

String::~String()
{
    clear();
}

String::String(const String& other)
    : m_byte_count(other.m_byte_count)
{
    if (is_stored_inline()) {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else {
        m_heap_buffer = other.m_heap_buffer;
        increment_reference_count();
    }
}

String::String(String&& other) noexcept
    : m_byte_count(other.m_byte_count)
{
    if (is_stored_inline()) {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else {
        m_heap_buffer = other.m_heap_buffer;
        other.m_heap_buffer = nullptr;
    }

    other.m_byte_count = 1;
    zero_memory(other.m_inline_buffer, inline_capacity);
}

String::String(StringView string_view)
    : m_byte_count(string_view.byte_count() + 1)
{
    char* destination_buffer = m_inline_buffer;

    if (!is_stored_inline()) {
        m_heap_buffer = allocate_heap_buffer(m_byte_count);
        increment_reference_count();
        destination_buffer = heap_buffer_characters();
    }

    copy_memory(destination_buffer, string_view.characters(), string_view.byte_count());
    destination_buffer[m_byte_count - 1] = 0;
}

String& String::operator=(const String& other)
{
    // Handle self-assignment case.
    if (this == &other)
        return *this;

    clear();
    m_byte_count = other.m_byte_count;

    if (is_stored_inline()) {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else {
        m_heap_buffer = other.m_heap_buffer;
        increment_reference_count();
    }

    return *this;
}

String& String::operator=(String&& other) noexcept
{
    // Handle self-assignment case.
    if (this == &other)
        return *this;

    clear();
    m_byte_count = other.m_byte_count;

    if (is_stored_inline()) {
        copy_memory(m_inline_buffer, other.m_inline_buffer, m_byte_count);
    }
    else {
        m_heap_buffer = other.m_heap_buffer;
        other.m_heap_buffer = nullptr;
    }

    other.m_byte_count = 1;
    zero_memory(other.m_inline_buffer, inline_capacity);

    return *this;
}

String& String::operator=(StringView string_view)
{
    clear();
    m_byte_count = string_view.byte_count() + 1;
    char* destination_buffer = m_inline_buffer;

    if (!is_stored_inline()) {
        m_heap_buffer = allocate_heap_buffer(m_byte_count);
        increment_reference_count();
        destination_buffer = heap_buffer_characters();
    }

    copy_memory(destination_buffer, string_view.characters(), string_view.byte_count());
    destination_buffer[m_byte_count - 1] = 0;

    return *this;
}

void String::clear()
{
    if (!is_stored_inline() && decrement_reference_count())
        free_heap_buffer(m_heap_buffer, m_byte_count);

    m_byte_count = 1;
    zero_memory(m_inline_buffer, inline_capacity);
}

void* String::allocate_heap_buffer(usize characters_byte_count)
{
    // NOTE: Sanity check.
    VERIFY(characters_byte_count > inline_capacity);

    const usize allocation_size = sizeof(HeapBufferHeader) + characters_byte_count;
    void* heap_buffer = ::operator new(allocation_size);
    new (heap_buffer) HeapBufferHeader();
    return heap_buffer;
}

void String::free_heap_buffer(void* heap_buffer, usize characters_byte_count)
{
    // NOTE: Sanity check.
    VERIFY(characters_byte_count > inline_capacity);

    MAYBE_UNUSED const usize allocation_size = sizeof(HeapBufferHeader) + characters_byte_count;
    HeapBufferHeader* header = static_cast<HeapBufferHeader*>(heap_buffer);
    VERIFY(header->reference_count == 0);
    header->~HeapBufferHeader();
    ::operator delete(heap_buffer);
}

void String::increment_reference_count()
{
    HeapBufferHeader* header = static_cast<HeapBufferHeader*>(m_heap_buffer);
    VERIFY(header->reference_count != NumericLimits<decltype(header->reference_count)>::max());
    header->reference_count++;
}

bool String::decrement_reference_count()
{
    HeapBufferHeader* header = static_cast<HeapBufferHeader*>(m_heap_buffer);
    VERIFY(header->reference_count > 0);
    header->reference_count--;
    return (header->reference_count == 0);
}

} // namespace AT
