/** @file */

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../structs/include.h"

#define DelphiFunction(ret_type, func_name, ...) \
    __attribute__((regparm(3))) \
    ret_type (__stdcall *func_name)(__VA_ARGS__)


#include "ClassTools.cpp"
// #include "WBaseClass.cpp"
// #include "WObject.cpp"
// #include "WList.cpp"
