/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <AT/String.h>
#include <AT/StringView.h>

namespace AT {

StringView StringView::from_utf8(const char* null_terminated_characters)
{
    const char* characters = null_terminated_characters;
    while (*null_terminated_characters++) {}
    const usize byte_count_including_null_terminator = null_terminated_characters - characters;
    return StringView::from_utf8(characters, byte_count_including_null_terminator - 1);
}

StringView::StringView(const String& string)
    : m_characters(string.characters())
    , m_byte_count(string.byte_count())
{}

StringView& StringView::operator=(const String& string)
{
    m_characters = string.characters();
    m_byte_count = string.byte_count();
    return *this;
}

bool StringView::operator==(const StringView& other) const
{
    if (m_byte_count != other.m_byte_count)
        return false;

    for (usize byte_offset = 0; byte_offset < m_byte_count; ++byte_offset) {
        if (m_characters[byte_offset] != other.m_characters[byte_offset])
            return false;
    }

    // NOTE: All bytes are matching, thus the strings must be equal.
    return true;
}

bool StringView::operator!=(const StringView& other) const
{
    const bool are_equal = (other == *this);
    return !are_equal;
}

} // namespace AT
