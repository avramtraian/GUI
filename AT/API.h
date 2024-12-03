/**
 * Copyright (c) 2024 Traian Avram. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause.
 */

#pragma once

#include <AT/APISpecifiers.h>

#ifdef AT_BUILD_SHARED_LIBRARY
    #define AT_API AT_API_SPECIFIER_EXPORT
#else
    #ifdef AT_LINK_AS_SHARED_LIBRARY
        #define AT_API AT_API_SPECIFIER_IMPORT
    #else
        #define AT_API
    #endif // AT_LINK_AS_SHARED_LIBRARY
#endif // AT_BUILD_SHARED_LIBRARY
