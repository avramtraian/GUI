/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Assertions.h>
#include <AT/Types.h>

namespace AT {

template<typename T>
class OwnPtr {
    AT_MAKE_NONCOPYABLE(OwnPtr);

    template<typename Q>
    friend OwnPtr<Q> adopt_own(Q*);

public:
    ALWAYS_INLINE OwnPtr()
        : m_instance(nullptr)
    {}

    ALWAYS_INLINE OwnPtr(OwnPtr&& other) noexcept
        : m_instance(other.m_instance)
    {
        other.m_instance = nullptr;
    }

    ALWAYS_INLINE OwnPtr(NullptrType)
        : m_instance(nullptr)
    {}

    ALWAYS_INLINE ~OwnPtr() { release(); }

    ALWAYS_INLINE OwnPtr& operator=(OwnPtr&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        m_instance = other.m_instance;
        other.m_instance = nullptr;
        return *this;
    }

    ALWAYS_INLINE OwnPtr& operator=(NullptrType)
    {
        release();
        return *this;
    }

public:
    NODISCARD ALWAYS_INLINE bool is_valid() const { return (m_instance != nullptr); }

    NODISCARD ALWAYS_INLINE T* get()
    {
        VERIFY(is_valid());
        return m_instance;
    }

    NODISCARD ALWAYS_INLINE const T* get() const
    {
        VERIFY(is_valid());
        return m_instance;
    }

    // NOTE: Wrappers around OwnPtr::get() API.
    NODISCARD ALWAYS_INLINE T* operator->() { return get(); }
    NODISCARD ALWAYS_INLINE const T* operator->() const { return get(); }

    NODISCARD ALWAYS_INLINE T& deref()
    {
        VERIFY(is_valid());
        return *m_instance;
    }

    NODISCARD ALWAYS_INLINE const T& deref() const
    {
        VERIFY(is_valid());
        return *m_instance;
    }

    // NOTE: Wrappers around OwnPtr::deref() API.
    NODISCARD ALWAYS_INLINE T& operator*() { return deref(); }
    NODISCARD ALWAYS_INLINE const T& operator*() const { return deref(); }

public:
    NODISCARD ALWAYS_INLINE bool operator==(const OwnPtr& other) const
    {
        const bool instances_are_equal = (m_instance == other.m_instance);
        return instances_are_equal;
    }

    NODISCARD ALWAYS_INLINE bool operator!=(const OwnPtr& other) const
    {
        const bool instances_are_equal = (m_instance == other.m_instance);
        return !instances_are_equal;
    }

public:
    ALWAYS_INLINE void release()
    {
        if (m_instance) {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    template<typename Q>
    NODISCARD ALWAYS_INLINE OwnPtr<Q> as()
    {
        Q* casted_instance = static_cast<Q*>(m_instance);
        OwnPtr<Q> casted_own_ptr = OwnPtr<Q>(casted_instance);
        m_instance = nullptr;
        return casted_own_ptr;
    }

    template<typename Q>
    NODISCARD ALWAYS_INLINE OwnPtr<Q> as() const
    {
        Q* casted_instance = static_cast<Q*>(m_instance);
        OwnPtr<Q> casted_own_ptr = OwnPtr<Q>(casted_instance);
        m_instance = nullptr;
        return casted_own_ptr;
    }

    NODISCARD ALWAYS_INLINE T* leak_and_release()
    {
        T* instance = m_instance;
        m_instance = nullptr;
        return instance;
    }

private:
    ALWAYS_INLINE explicit OwnPtr(T* instance)
        : m_instance(instance)
    {}

private:
    T* m_instance;
};

template<typename T>
NODISCARD ALWAYS_INLINE OwnPtr<T> adopt_own(T* instance)
{
    OwnPtr<T> own_ptr = OwnPtr<T>(instance);
    return own_ptr;
}

template<typename T, typename... Args>
NODISCARD ALWAYS_INLINE OwnPtr<T> create_own(Args&&... args)
{
    T* instance = new T(forward<Args>(args)...);
    return adopt_own(instance);
}

} // namespace AT

using AT::adopt_own;
using AT::create_own;
using AT::OwnPtr;
