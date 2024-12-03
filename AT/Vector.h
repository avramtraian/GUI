/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Assertions.h>
#include <AT/New.h>
#include <AT/Span.h>
#include <AT/Types.h>

namespace AT {
template<typename T>
class Vector {
public:
    static constexpr usize growth_factor_numerator = 3;
    static constexpr usize growth_factor_denominator = 2;
    static_assert(growth_factor_numerator > growth_factor_denominator);

public:
    NODISCARD ALWAYS_INLINE static Vector from_initial_capacity(usize initial_capacity)
    {
        Vector vector;
        vector.m_capacity = initial_capacity;
        vector.m_elements = allocate_memory(vector.m_capacity);
        return vector;
    }

    NODISCARD ALWAYS_INLINE static Vector from_span(Span<const T> span)
    {
        Vector vector = Vector::from_initial_capacity(span.count());
        vector.m_count = span.count();
        copy_elements(vector.m_elements, span.elements(), vector.m_count);
        return vector;
    }

    NODISCARD ALWAYS_INLINE static Vector from_template_element(usize in_count, const T& template_element)
    {
        Vector vector = Vector::from_initial_capacity(in_count);
        vector.m_count = in_count;
        for (usize index = 0; index < vector.m_count; ++index)
            new (vector.m_elements + index) T(template_element);
        return vector;
    }

public:
    ALWAYS_INLINE Vector()
        : m_elements(nullptr)
        , m_capacity(0)
        , m_count(0)
    {}

    ALWAYS_INLINE Vector(const Vector& other)
        : m_capacity(other.m_count)
        , m_count(other.m_count)
    {
        m_elements = allocate_memory(m_capacity);
        copy_elements(m_elements, other.m_elements, m_count);
    }

    ALWAYS_INLINE Vector(Vector&& other) noexcept
        : m_elements(other.m_elements)
        , m_capacity(other.m_capacity)
        , m_count(other.m_count)
    {
        other.m_elements = nullptr;
        other.m_capacity = 0;
        other.m_count = 0;
    }

    ALWAYS_INLINE ~Vector() { clear_and_shrink(); }

    ALWAYS_INLINE Vector& operator=(const Vector& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear();
        if (other.m_count > m_capacity) {
            free_memory(m_elements, m_capacity);
            m_capacity = other.m_count;
            m_elements = allocate_memory(m_capacity);
        }

        m_count = other.m_count;
        copy_elements(m_elements, other.m_elements, m_count);

        return *this;
    }

    ALWAYS_INLINE Vector& operator=(Vector&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        clear();

        m_elements = other.m_elements;
        m_capacity = other.m_capacity;
        m_count = other.m_count;

        other.m_elements = nullptr;
        other.m_capacity = 0;
        other.m_count = 0;

        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE T* elements() { return m_elements; }
    NODISCARD ALWAYS_INLINE const T* elements() const { return m_elements; }
    NODISCARD ALWAYS_INLINE usize capacity() const { return m_capacity; }
    NODISCARD ALWAYS_INLINE usize count() const { return m_count; }

    NODISCARD ALWAYS_INLINE bool is_empty() const { return (m_count == 0); }
    NODISCARD ALWAYS_INLINE bool has_elements() const { return (m_count > 0); }

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
    template<typename... Args>
    ALWAYS_INLINE void emplace(Args&&... args)
    {
        expand_elements_block_if_required(m_count + 1);
        new (m_elements + m_count) T(forward<Args>(args)...);
        ++m_count;
    }

    ALWAYS_INLINE void add(const T& element)
    {
        expand_elements_block_if_required(m_count + 1);
        new (m_elements + m_count) T(element);
        ++m_count;
    }

    ALWAYS_INLINE void add(T&& element)
    {
        expand_elements_block_if_required(m_count + 1);
        new (m_elements + m_count) T(move(element));
        ++m_count;
    }

public:
    ALWAYS_INLINE void remove_last()
    {
        VERIFY(m_count > 0);
        m_elements[--m_count].~T();
    }

    ALWAYS_INLINE void remove_last(usize remove_count)
    {
        VERIFY(m_count >= remove_count);
        for (usize remove_index = m_count - remove_count; remove_index < m_count; ++remove_index)
            m_elements[remove_index].~T();
        m_count -= remove_count;
    }

    ALWAYS_INLINE void remove_unordered(usize remove_index)
    {
        VERIFY(remove_index < m_count);
        m_elements[remove_index].~T();
        --m_count;

        if (remove_index != m_count) {
            new (m_elements + remove_index) T(move(m_elements[m_count]));
            m_elements[m_count].~T();
        }
    }

    ALWAYS_INLINE void remove_unordered(usize remove_index, usize remove_count)
    {
        VERIFY(remove_index + remove_count <= m_count);

        usize move_count = m_count - (remove_index + remove_count);
        if (move_count > remove_count)
            move_count = remove_count;

        const usize move_index = m_count - move_count;
        for (usize index = 0; index < move_count; ++index) {
            m_elements[remove_index + index].~T();
            new (m_elements + remove_index + index) T(m_elements[move_index + index]);
            m_elements[move_index + index].~T();
        }

        for (usize index = move_count; index < remove_count; ++index)
            m_elements[remove_index + index].~T();

        m_count -= remove_count;
    }

public:
    ALWAYS_INLINE void clear()
    {
        for (usize index = 0; index < m_count; ++index)
            m_elements[index].~T();
        m_count = 0;
    }

    ALWAYS_INLINE void clear_and_shrink()
    {
        clear();
        free_memory(m_elements, m_capacity);
        m_elements = nullptr;
        m_capacity = 0;
    }

private:
    NODISCARD ALWAYS_INLINE static T* allocate_memory(usize in_count)
    {
        const usize allocation_size = in_count * sizeof(T);
        void* memory_block = ::operator new(allocation_size);
        return static_cast<T*>(memory_block);
    }

    ALWAYS_INLINE static void free_memory(T* in_elements, usize in_capacity)
    {
        MAYBE_UNUSED const usize allocation_size = in_capacity * sizeof(T);
        ::operator delete(in_elements);
    }

    ALWAYS_INLINE static void copy_elements(T* destination_elements, const T* source_elements, usize in_count)
    {
        for (usize index = 0; index < in_count; ++index)
            new (destination_elements + index) T(source_elements[index]);
    }

    ALWAYS_INLINE static void move_elements(T* destination_elements, T* source_elements, usize in_count)
    {
        for (usize index = 0; index < in_count; ++index) {
            new (destination_elements + index) T(move(source_elements[index]));
            source_elements[index].~T();
        }
    }

private:
    ALWAYS_INLINE void expand_elements_block(usize new_capacity)
    {
        VERIFY(new_capacity > m_capacity);
        T* new_elements = allocate_memory(new_capacity);
        move_elements(new_elements, m_elements, m_count);
        free_memory(m_elements, m_capacity);
        m_elements = new_elements;
        m_capacity = new_capacity;
    }

    ALWAYS_INLINE void expand_elements_block_if_required(usize required_capacity)
    {
        if (m_capacity >= required_capacity)
            return;

        // NOTE: By default, the vector capacity follows a geometric series with the ratio given
        // by the growth factor.
        usize new_capacity = (m_count * growth_factor_numerator) / growth_factor_denominator;
        if (new_capacity < required_capacity)
            new_capacity = required_capacity;

        expand_elements_block(new_capacity);
    }

private:
    T* m_elements;
    usize m_capacity;
    usize m_count;
};

} // namespace AT

using AT::Vector;
