/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <AT/MemoryOperations.h>

namespace AT {

void copy_memory(void* destination_buffer, const void* source_buffer, usize byte_count)
{
    const WriteonlyBytes destination = static_cast<WriteonlyBytes>(destination_buffer);
    const ReadonlyBytes source = static_cast<ReadonlyBytes>(source_buffer);

    // NOTE: By writing the code as simple and verbosely as possible the compiler might figure out that
    //       we are trying to perform a 'memcpy' operation and optimize it away.

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset)
        destination[byte_offset] = source[byte_offset];
}

void set_memory(void* destination_buffer, u8 byte_value, usize byte_count)
{
    const WriteonlyBytes destination = static_cast<WriteonlyBytes>(destination_buffer);

    // NOTE: By writing the code as simple and verbosely as possible the compiler might figure out that
    //       we are trying to perform a 'memset' operation and optimize it away.

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset)
        destination[byte_offset] = byte_value;
}

void zero_memory(void* destination_buffer, usize byte_count)
{
    const WriteonlyBytes destination = static_cast<WriteonlyBytes>(destination_buffer);

    // NOTE: By writing the code as simple and verbosely as possible the compiler might figure out that
    //       we are trying to perform a specialized 'memset' operation and optimize it away.

    for (usize byte_offset = 0; byte_offset < byte_count; ++byte_offset)
        destination[byte_offset] = 0;
}

} // namespace AT
