/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Assertions.h>
#include <AT/Span.h>
#include <AT/Types.h>

namespace AT {

template<typename T, usize C>
requires (C > 0)
class Array {
public:
    ALWAYS_INLINE constexpr Array() = default;
    ALWAYS_INLINE constexpr ~Array() = default;

    ALWAYS_INLINE constexpr Array(const Array& other)
    {
        for (usize index = 0; index < C; ++index)
            m_elements[index] = other.m_elements[index];
    }

    ALWAYS_INLINE constexpr Array(Array&& other) noexcept
    {
        for (usize index = 0; index < C; ++index)
            m_elements[index] = move(other.m_elements[index]);
    }

    ALWAYS_INLINE constexpr Array& operator=(const Array& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        for (usize index = 0; index < C; ++index)
            m_elements[index] = other.m_elements[index];
        return *this;
    }

    ALWAYS_INLINE constexpr Array& operator=(Array&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        for (usize index = 0; index < C; ++index)
            m_elements[index] = move(other.m_elements[index]);
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE T* elements() { return m_elements; }
    NODISCARD ALWAYS_INLINE const T* elements() const { return m_elements; }
    NODISCARD ALWAYS_INLINE static constexpr usize count() { return C; }

    NODISCARD ALWAYS_INLINE Span<T> span() { return Span<T>(m_elements, C); }
    NODISCARD ALWAYS_INLINE Span<const T> span() const { return Span<const T>(m_elements, C); }
    NODISCARD ALWAYS_INLINE Span<const T> readonly_span() const { return Span<const T>(m_elements, C); }

public:
    NODISCARD ALWAYS_INLINE T& at(usize index)
    {
        VERIFY(index < C);
        return m_elements[index];
    }

    NODISCARD ALWAYS_INLINE const T& at(usize index) const
    {
        VERIFY(index < C);
        return m_elements[index];
    }

    // NOTE: Wrappers around the Array::at() API.
    NODISCARD ALWAYS_INLINE T& operator[](usize index) { return at(index); }
    NODISCARD ALWAYS_INLINE const T& operator[](usize index) const { return at(index); }

private:
    T m_elements[C];
};

} // namespace AT

using AT::Array;
