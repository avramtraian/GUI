/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/API.h>
#include <AT/Span.h>
#include <AT/Types.h>

namespace AT {

class ByteBuffer {
    AT_MAKE_NONCOPYABLE(ByteBuffer);

public:
    AT_API static ByteBuffer from_initial_byte_count(usize initial_byte_count);
    AT_API static ByteBuffer from_byte_span(ReadonlyByteSpan byte_span);

    AT_API static ByteBuffer copy(const ByteBuffer& source_buffer);

public:
    AT_API ByteBuffer();
    AT_API ~ByteBuffer();

    AT_API ByteBuffer(ByteBuffer&& other) noexcept;
    AT_API ByteBuffer& operator=(ByteBuffer&& other) noexcept;

public:
    NODISCARD ALWAYS_INLINE ReadWriteBytes bytes() { return m_bytes; }
    NODISCARD ALWAYS_INLINE ReadonlyBytes bytes() const { return m_bytes; }
    NODISCARD ALWAYS_INLINE ReadonlyBytes readonly_bytes() const { return m_bytes; }

    NODISCARD ALWAYS_INLINE usize byte_count() const { return m_byte_count; }
    NODISCARD ALWAYS_INLINE bool is_empty() const { return (m_byte_count == 0); }
    NODISCARD ALWAYS_INLINE bool has_bytes() const { return (m_byte_count > 0); }

    NODISCARD ALWAYS_INLINE ReadWriteByteSpan byte_span() { return ReadWriteByteSpan(m_bytes, m_byte_count); }
    NODISCARD ALWAYS_INLINE ReadonlyByteSpan byte_span() const { return ReadonlyByteSpan(m_bytes, m_byte_count); }
    NODISCARD ALWAYS_INLINE ReadonlyByteSpan readonly_byte_span() const { return ReadonlyByteSpan(m_bytes, m_byte_count); }

public:
    AT_API void free();

    AT_API void allocate_new(usize new_byte_count);

    AT_API void expand(usize new_byte_count);
    AT_API void expand_by(usize expansion_byte_count);

    AT_API void shrink(usize new_byte_count);
    AT_API void shrink_by(usize shrinking_byte_count);

    AT_API void ensure_byte_count(usize in_byte_count);

    AT_API void set_byte_count(usize in_byte_count);

private:
    ReadWriteBytes m_bytes;
    usize m_byte_count;
};

} // namespace AT

using AT::ByteBuffer;
