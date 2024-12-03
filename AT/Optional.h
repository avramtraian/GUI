/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Assertions.h>
#include <AT/Types.h>

namespace AT {

template<typename T>
class Optional {
public:
    ALWAYS_INLINE Optional()
        : m_value_buffer {}
        , m_has_value(false)
    {}

    ALWAYS_INLINE Optional(const Optional& other)
        : m_has_value(other.m_has_value)
    {
        if (m_has_value)
            new (m_value_buffer) T(other.raw_value());
    }

    ALWAYS_INLINE Optional(Optional&& other) noexcept
        : m_has_value(other.m_has_value)
    {
        if (m_has_value) {
            new (m_value_buffer) T(move(other.raw_value()));
            other.raw_value().~T();
            other.m_has_value = false;
        }
    }

    ALWAYS_INLINE Optional(const T& value)
        : m_has_value(true)
    {
        new (m_value_buffer) T(value);
    }

    ALWAYS_INLINE Optional(T&& value)
        : m_has_value(true)
    {
        new (m_value_buffer) T(move(value));
    }

    ALWAYS_INLINE ~Optional()
    {
        if (m_has_value)
            raw_value().~T();
    }

    ALWAYS_INLINE Optional& operator=(const Optional& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        if (other.m_has_value) {
            m_has_value = true;
            new (m_value_buffer) T(other.raw_value());
        }

        return *this;
    }

    ALWAYS_INLINE Optional& operator=(Optional&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        if (other.m_has_value) {
            m_has_value = true;
            new (m_value_buffer) T(move(other.raw_value()));
            other.raw_value().~T();
            other.m_has_value = false;
        }

        return *this;
    }

    ALWAYS_INLINE Optional& operator=(const T& value)
    {
        release();
        m_has_value = true;
        new (m_value_buffer) T(value);
        return *this;
    }

    ALWAYS_INLINE Optional& operator=(T&& value)
    {
        release();
        m_has_value = true;
        new (m_value_buffer) T(move(value));
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE bool has_value() const { return m_has_value; }

    NODISCARD ALWAYS_INLINE T& value()
    {
        VERIFY(has_value());
        return raw_value();
    }

    NODISCARD ALWAYS_INLINE const T& value() const
    {
        VERIFY(has_value());
        return raw_value();
    }

    // NOTE: Wrappers around the Optional::value() API.
    NODISCARD ALWAYS_INLINE T& operator*() { return value(); }
    NODISCARD ALWAYS_INLINE const T& operator*() const { return value(); }

    NODISCARD ALWAYS_INLINE T* operator->()
    {
        VERIFY(has_value());
        return reinterpret_cast<T*>(m_value_buffer);
    }

    NODISCARD ALWAYS_INLINE const T* operator->() const
    {
        VERIFY(has_value());
        return reinterpret_cast<const T*>(m_value_buffer);
    }

    NODISCARD ALWAYS_INLINE T& value_or(T& fallback_value)
    {
        if (m_has_value)
            return raw_value();
        return fallback_value;
    }

    NODISCARD ALWAYS_INLINE T value_or(const T& fallback_value) const
    {
        if (m_has_value)
            return raw_value();
        return fallback_value;
    }

public:
    ALWAYS_INLINE void release()
    {
        if (m_has_value) {
            raw_value().~T();
            m_has_value = false;
        }
    }

public:
    NODISCARD ALWAYS_INLINE bool operator==(const Optional& other) const
    {
        if (m_has_value != other.m_has_value)
            return false;

        if (m_has_value) {
            const bool values_are_equal = (raw_value() == other.raw_value());
            return values_are_equal;
        }

        // NOTE: Two empty optionals are always equal.
        return true;
    }

    NODISCARD ALWAYS_INLINE bool operator!=(const Optional& other) const
    {
        const bool are_equal = (*this == other);
        return !are_equal;
    }

private:
    NODISCARD ALWAYS_INLINE T& raw_value()
    {
        T* raw_value_ptr = reinterpret_cast<T*>(m_value_buffer);
        return *raw_value_ptr;
    }

    NODISCARD ALWAYS_INLINE const T& raw_value() const
    {
        const T* raw_value_ptr = reinterpret_cast<const T*>(m_value_buffer);
        return *raw_value_ptr;
    }

private:
    alignas(T) u8 m_value_buffer[sizeof(T)];
    bool m_has_value;
};

template<typename T>
class Optional<T&> {
public:
    ALWAYS_INLINE Optional()
        : m_value(nullptr)
    {}

    ALWAYS_INLINE Optional(const Optional& other)
        : m_value(other.m_value)
    {}

    ALWAYS_INLINE Optional(Optional&& other) noexcept
        : m_value(other.m_value)
    {
        other.m_value = nullptr;
    }

    ALWAYS_INLINE Optional(T& value)
        : m_value(&value)
    {}

    ALWAYS_INLINE ~Optional() = default;

    ALWAYS_INLINE Optional& operator=(const Optional& other)
    {
        m_value = other.m_value;
        return *this;
    }

    ALWAYS_INLINE Optional& operator=(Optional&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        m_value = other.m_value;
        other.m_value = nullptr;
        return *this;
    }

    ALWAYS_INLINE Optional& operator=(T& value)
    {
        m_value = &value;
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE bool has_value() const { return (m_value != nullptr); }

    NODISCARD ALWAYS_INLINE T& value()
    {
        VERIFY(has_value());
        return *m_value;
    }

    NODISCARD ALWAYS_INLINE const T& value() const
    {
        VERIFY(has_value());
        return *m_value;
    }

    // NOTE: Wrappers around the Optional::value() API.
    NODISCARD ALWAYS_INLINE T& operator*() { return value(); }
    NODISCARD ALWAYS_INLINE const T& operator*() const { return value(); }

    NODISCARD ALWAYS_INLINE T* operator->()
    {
        VERIFY(has_value());
        return m_value;
    }

    NODISCARD ALWAYS_INLINE const T* operator->() const
    {
        VERIFY(has_value());
        return m_value;
    }

    NODISCARD ALWAYS_INLINE T& value_or(T& fallback_value)
    {
        if (has_value())
            return *m_value;
        return fallback_value;
    }

    NODISCARD ALWAYS_INLINE T value_or(const T& fallback_value) const
    {
        if (has_value())
            return *m_value;
        return fallback_value;
    }

public:
    ALWAYS_INLINE void release()
    {
        // NOTE: The T& specialization of the Optional class is basically a
        //       glorified wrapper around a raw pointer.
        m_value = nullptr;
    }

public:
    NODISCARD ALWAYS_INLINE bool operator==(const Optional& other) const
    {
        const bool values_are_equal = (m_value == other.m_value);
        return values_are_equal;
    }

    NODISCARD ALWAYS_INLINE bool operator!=(const Optional& other) const
    {
        const bool values_are_equal = (m_value == other.m_value);
        return !values_are_equal;
    }

private:
    T* m_value;
};

} // namespace AT

using AT::Optional;
