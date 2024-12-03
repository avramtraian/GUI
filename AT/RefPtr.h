/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Assertions.h>
#include <AT/Types.h>

namespace AT {

class RefCounted {
    AT_MAKE_NONCOPYABLE(RefCounted);
    AT_MAKE_NONMOVABLE(RefCounted);

public:
    ALWAYS_INLINE RefCounted()
        : m_reference_count(0)
    {}
    virtual ~RefCounted() = default;

public:
    NODISCARD ALWAYS_INLINE u64 reference_count() const { return m_reference_count; }

    ALWAYS_INLINE void increment_reference_count()
    {
        VERIFY(m_reference_count != NumericLimits<decltype(m_reference_count)>::max());
        ++m_reference_count;
    }

    // Returns whether the reference count hits zero after the decrement operation.
    // The return value of this value should always be checked and handled.
    NODISCARD ALWAYS_INLINE bool decrement_reference_count()
    {
        VERIFY(m_reference_count > 0);
        --m_reference_count;
        return (m_reference_count == 0);
    }

public:
    u64 m_reference_count;
};

template<typename T>
requires (is_derived_from<T, RefCounted>)
class RefPtr {
    template<typename Q>
    friend RefPtr<Q> adopt_ref(Q*);

public:
    ALWAYS_INLINE RefPtr()
        : m_instance(nullptr)
    {}

    ALWAYS_INLINE RefPtr(const RefPtr& other)
        : m_instance(other.m_instance)
    {
        if (m_instance)
            increment_reference_count();
    }

    ALWAYS_INLINE RefPtr(RefPtr&& other) noexcept
        : m_instance(other.m_instance)
    {
        other.m_instance = nullptr;
    }

    ALWAYS_INLINE RefPtr(NullptrType)
        : m_instance(nullptr)
    {}

    ALWAYS_INLINE ~RefPtr() { release(); }

    ALWAYS_INLINE RefPtr& operator=(const RefPtr& other)
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        m_instance = other.m_instance;
        if (m_instance)
            increment_reference_count();

        return *this;
    }

    ALWAYS_INLINE RefPtr& operator=(RefPtr&& other) noexcept
    {
        // Handle self-assignment case.
        if (this == &other)
            return *this;

        release();
        m_instance = other.m_instance;
        other.m_instance = nullptr;
        return *this;
    }

    ALWAYS_INLINE RefPtr& operator=(NullptrType)
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
    NODISCARD ALWAYS_INLINE bool operator==(const RefPtr& other) const
    {
        const bool instances_are_equal = (m_instance == other.m_instance);
        return instances_are_equal;
    }

    NODISCARD ALWAYS_INLINE bool operator!=(const RefPtr& other) const
    {
        const bool instances_are_equal = (m_instance == other.m_instance);
        return !instances_are_equal;
    }

public:
    ALWAYS_INLINE void release()
    {
        if (m_instance) {
            if (decrement_reference_count())
                delete m_instance;
            m_instance = nullptr;
        }
    }

    template<typename Q>
    NODISCARD ALWAYS_INLINE RefPtr<Q> as()
    {
        Q* casted_instance = static_cast<Q*>(m_instance);
        return RefPtr<Q>(casted_instance);
    }

    template<typename Q>
    NODISCARD ALWAYS_INLINE RefPtr<Q> as() const
    {
        Q* casted_instance = static_cast<Q*>(m_instance);
        return RefPtr<Q>(casted_instance);
    }

private:
    ALWAYS_INLINE explicit RefPtr(T* instance)
        : m_instance(instance)
    {
        if (m_instance)
            increment_reference_count();
    }

    ALWAYS_INLINE void increment_reference_count()
    {
        RefCounted* ref_counted = static_cast<RefCounted*>(m_instance);
        ref_counted->increment_reference_count();
    }

    NODISCARD ALWAYS_INLINE bool decrement_reference_count()
    {
        RefCounted* ref_counted = static_cast<RefCounted*>(m_instance);
        return ref_counted->decrement_reference_count();
    }

private:
    T* m_instance;
};

template<typename T>
NODISCARD ALWAYS_INLINE RefPtr<T> adopt_ref(T* instance)
{
    RefPtr<T> ref_ptr = RefPtr<T>(instance);
    return ref_ptr;
}

template<typename T, typename... Args>
NODISCARD ALWAYS_INLINE RefPtr<T> create_ref(Args&&... args)
{
    T* instance = new T(forward<Args>(args)...);
    return adopt_ref(instance);
}

} // namespace AT

using AT::adopt_ref;
using AT::create_ref;
using AT::RefCounted;
using AT::RefPtr;
