/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#include <AT/Assertions.h>

namespace AT {

void on_assertion_failed(AssertionKind, const char*, const AssertionSourceLocation&)
{
    // TODO: Actually implement this function and print the assertion expression to the
    //       console or somewhere the user can find.
}

} // namespace AT
