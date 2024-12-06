#
# Copyright (c) 2024 Traian Avram. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause.
#

set(AT_FRAMEWORK_SOURCE_FILES
    API.h
    APISpecifiers.h
    Array.h
    Assertions.cpp
    Assertions.h
    ByteBuffer.cpp
    ByteBuffer.h
    Defines.h
    Format.cpp
    Format.h
    MemoryOperations.cpp
    MemoryOperations.h
    New.h
    NumericLimits.h
    Optional.h
    OwnPtr.h
    RefPtr.h
    Span.h
    String.cpp
    String.h
    StringBuilder.cpp
    StringBuilder.h
    StringView.cpp
    StringView.h
    Types.h
    Vector.h
)

add_library(AT-Framework SHARED ${AT_FRAMEWORK_SOURCE_FILES})
target_compile_definitions(AT-Framework PRIVATE "AT_BUILD_SHARED_LIBRARY")
target_compile_definitions(AT-Framework PUBLIC "AT_LINK_AS_SHARED_LIBRARY")
target_include_directories(AT-Framework PRIVATE ${CMAKE_SOURCE_DIR})
