# Copyright (c) 2020 vesoft inc. All rights reserved.
#
# This source code is licensed under Apache 2.0 License,
# attached with Common Clause Condition 1.0, found in the LICENSES directory.

nebula_add_library(
    context_obj OBJECT
    QueryContext.cpp
    QueryExpressionContext.cpp
    ExecutionContext.cpp
    Iterator.cpp
    Result.cpp
)


nebula_add_subdirectory(test)
