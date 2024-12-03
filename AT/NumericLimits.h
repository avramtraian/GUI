/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/Types.h>

namespace AT {

template<typename T>
class NumericLimits {
public:
    NODISCARD ALWAYS_INLINE static constexpr T min()
    {
        // NOTE: You must specialize NumericLimits<T> in order to access this API!
        static_assert(false);
        return T();
    }

    NODISCARD ALWAYS_INLINE static constexpr T max()
    {
        // NOTE: You must specialize NumericLimits<T> in order to access this API!
        static_assert(false);
        return T();
    }
};

#define AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(type, min_limit, max_limit) \
    template<>                                                                   \
    class NumericLimits<type> {                                                  \
    public:                                                                      \
        NODISCARD ALWAYS_INLINE static constexpr type min()                      \
        {                                                                        \
            return min_limit;                                                    \
        }                                                                        \
        NODISCARD ALWAYS_INLINE static constexpr type max()                      \
        {                                                                        \
            return max_limit;                                                    \
        }                                                                        \
    };

// clang-format off
AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(u8,  0, 0xFF);
AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(u16, 0, 0xFFFF);
AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(u32, 0, 0xFFFFFFFF);
AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(u64, 0, 0xFFFFFFFFFFFFFFFF);

AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(s8,  -0x80,               0x7F);
AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(s16, -0x8000,             0x7FFF);
AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(s32, -0x80000000,         0x7FFFFFFF);
AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE(s64, -0x8000000000000000, 0x7FFFFFFFFFFFFFFF);
// clang-format on

#undef AT_DECLARE_NUMERIC_LIMITS_FOR_PRIMITIVE_TYPE

} // namespace AT

using AT::NumericLimits;
