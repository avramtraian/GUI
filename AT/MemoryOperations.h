/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/API.h>
#include <AT/Types.h>

namespace AT {

AT_API void copy_memory(void* destination_buffer, const void* source_buffer, usize byte_count);

AT_API void set_memory(void* destination_buffer, u8 byte_value, usize byte_count);

AT_API void zero_memory(void* destination_buffer, usize byte_count);

} // namespace AT

using AT::copy_memory;
using AT::set_memory;
using AT::zero_memory;
