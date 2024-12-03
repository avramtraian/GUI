/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <AT/ByteBuffer.h>
#include <AT/MemoryOperations.h>

namespace AT {

ByteBuffer ByteBuffer::from_initial_byte_count(usize initial_byte_count)
{
    ByteBuffer byte_buffer;
    byte_buffer.allocate_new(initial_byte_count);
    return byte_buffer;
}

ByteBuffer ByteBuffer::from_byte_span(ReadonlyByteSpan byte_span)
{
    ByteBuffer byte_buffer = ByteBuffer::from_initial_byte_count(byte_span.count());
    copy_memory(byte_buffer.bytes(), byte_span.elements(), byte_span.count());
    return byte_buffer;
}

ByteBuffer ByteBuffer::copy(const ByteBuffer& source_buffer)
{
    const ReadonlyByteSpan source_buffer_byte_span = source_buffer.byte_span();
    return ByteBuffer::from_byte_span(source_buffer_byte_span);
}

ByteBuffer::ByteBuffer()
    : m_bytes(nullptr)
    , m_byte_count(0)
{}

ByteBuffer::~ByteBuffer()
{
    free();
}

ByteBuffer::ByteBuffer(ByteBuffer&& other) noexcept
    : m_bytes(other.m_bytes)
    , m_byte_count(other.m_byte_count)
{}

ByteBuffer& ByteBuffer::operator=(ByteBuffer&& other) noexcept
{
    // Handle self-assignment case.
    if (this == &other)
        return *this;

    free();
    m_bytes = other.m_bytes;
    m_byte_count = other.m_byte_count;
    return *this;
}

void ByteBuffer::free()
{
    if (m_bytes != nullptr) {
        VERIFY(m_byte_count > 0);
        ::operator delete(m_bytes);
    }

    m_bytes = nullptr;
    m_byte_count = 0;
}

void ByteBuffer::allocate_new(usize new_byte_count)
{
    free();
    if (new_byte_count == 0)
        return;

    m_byte_count = new_byte_count;
    m_bytes = static_cast<ReadWriteBytes>(::operator new(m_byte_count));
}

void ByteBuffer::expand(usize new_byte_count)
{
    VERIFY(new_byte_count >= m_byte_count);
    set_byte_count(new_byte_count);
}

void ByteBuffer::expand_by(usize expansion_byte_count)
{
    const usize new_byte_count = m_byte_count + expansion_byte_count;
    expand(new_byte_count);
}

void ByteBuffer::shrink(usize new_byte_count)
{
    VERIFY(new_byte_count <= m_byte_count);
    set_byte_count(new_byte_count);
}

void ByteBuffer::shrink_by(usize shrinking_byte_count)
{
    VERIFY(shrinking_byte_count <= m_byte_count);
    const usize new_byte_count = m_byte_count - shrinking_byte_count;
    shrink(new_byte_count);
}

void ByteBuffer::ensure_byte_count(usize in_byte_count)
{
    if (m_byte_count >= in_byte_count)
        return;
    expand(in_byte_count);
}

void ByteBuffer::set_byte_count(usize in_byte_count)
{
    if (m_byte_count == in_byte_count)
        return;

    usize copy_byte_count = m_byte_count;
    if (copy_byte_count > in_byte_count)
        copy_byte_count = in_byte_count;

    const ReadWriteBytes new_bytes = static_cast<ReadWriteBytes>(::operator new(in_byte_count));
    copy_memory(new_bytes, m_bytes, copy_byte_count);

    free();
    m_bytes = new_bytes;
    m_byte_count = in_byte_count;
}

} // namespace AT
