/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Assertions.h>
#include <AT/Types.h>

namespace AT {

template<typename T>
class Span {
public:
    ALWAYS_INLINE constexpr Span()
        : m_elements(nullptr)
        , m_count(0)
    {}

    ALWAYS_INLINE constexpr Span(const Span& other)
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {}

    // NOTE(Traian): Implicitly convert from a non-const span to a const without.
    // This special constructor requires the span template parameter to not already be const.
    ALWAYS_INLINE constexpr Span(const Span<RemoveConst<T>>& other)
    requires (is_const<T>)
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {}

    ALWAYS_INLINE constexpr Span(Span&& other) noexcept
        : m_elements(other.m_elements)
        , m_count(other.m_count)
    {
        other.m_elements = nullptr;
        other.m_count = 0;
    }

    ALWAYS_INLINE constexpr Span(T* in_elements, usize in_count)
        : m_elements(in_elements)
        , m_count(in_count)
    {}

    ALWAYS_INLINE constexpr Span(RemoveConst<T>* in_elements, usize in_count)
    requires (is_const<T>)
        : m_elements(in_elements)
        , m_count(in_count)
    {}

    ALWAYS_INLINE constexpr Span& operator=(const Span& other)
    {
        m_elements = other.m_elements;
        m_count = other.m_count;
        return *this;
    }

    ALWAYS_INLINE constexpr Span& operator=(Span&& other) noexcept
    {
        m_elements = other.m_elements;
        m_count = other.m_count;
        other.m_elements = nullptr;
        other.m_count = 0;
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE T* elements() { return m_elements; }
    NODISCARD ALWAYS_INLINE const T* elements() const { return m_elements; }

    NODISCARD ALWAYS_INLINE usize count() const { return m_count; }
    NODISCARD ALWAYS_INLINE static constexpr usize element_size() { return sizeof(T); }
    NODISCARD ALWAYS_INLINE usize byte_count() const { return (m_count * sizeof(T)); }

public:
    NODISCARD ALWAYS_INLINE T& at(usize index)
    {
        VERIFY(index < m_count);
        return m_elements[index];
    }

    NODISCARD ALWAYS_INLINE const T& at(usize index) const
    {
        VERIFY(index < m_count);
        return m_elements[index];
    }

    NODISCARD ALWAYS_INLINE T& operator[](usize index) { return m_elements[index]; }
    NODISCARD ALWAYS_INLINE const T& operator[](usize index) const { return m_elements[index]; }

public:
    NODISCARD ALWAYS_INLINE Span slice(usize offset_index, usize in_count)
    {
        VERIFY(offset_index + in_count <= m_count);
        return Span(m_elements + offset_index, in_count);
    }

    NODISCARD ALWAYS_INLINE Span slice(usize offset_index)
    {
        VERIFY(offset_index <= m_count);
        return Span(m_elements + offset_index, m_count - offset_index);
    }

    // NOTE(Traian): 'range_min_index' is the index of the first element included in the slice, and
    // 'range_max_index' is the index of the first element past the slice (one past the last element
    // included in the slice).
    NODISCARD ALWAYS_INLINE Span slice_range(usize range_min_index, usize range_max_index)
    {
        VERIFY(range_min_index <= range_max_index);
        VERIFY(range_max_index <= m_count);
        return Span(m_elements + range_min_index, range_max_index - range_min_index);
    }

private:
    T* m_elements;
    usize m_count;
};

using ReadonlyByteSpan = Span<ReadonlyByte>;
using ReadWriteByteSpan = Span<WriteonlyByte>;
using WriteonlyByteSpan = Span<ReadWriteByte>;

} // namespace AT

using AT::ReadonlyByteSpan;
using AT::ReadWriteByteSpan;
using AT::Span;
using AT::WriteonlyByteSpan;
