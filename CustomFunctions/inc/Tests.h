#pragma once
#include <string>
#include "..\inc\DateTime.h"
#include <unordered_map>

namespace Tests {
    std::unordered_map<wchar_t, uint64_t> timetesttemp;
    void TimeTestsStartTest(wchar_t* testname);
    uint64_t TimeTestsEndTest(wchar_t* testname);
    std::wstring TimeTestsEndTestString(wchar_t* testname);
    const wchar_t* TimeTestsEndTestChar(wchar_t* testname);
};